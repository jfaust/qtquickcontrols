/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qtmenu_p.h"
#include "qtmenuitemcontainer_p.h"
#include "qtaction_p.h"
#include "qtmenupopupwindow_p.h"

#include <qdebug.h>
#include <qabstractitemmodel.h>
#include <qcursor.h>
#include <private/qguiapplication_p.h>
#include <QtGui/qpa/qplatformtheme.h>
#include <QtGui/qpa/qplatformmenu.h>
#include <qquickitem.h>

QT_BEGIN_NAMESPACE

/*!
  \class QtMenu
  \internal
 */

/*!
  \qmltype MenuPrivate
  \instantiates QtMenu
  \internal
  \inqmlmodule QtQuick.Controls 1.0
 */

/*!
    \qmlproperty list<Object> Menu::items
    \default

    The list of items in the menu.

    \l Menu only accepts objects of type \l Menu, \l MenuItem, and \l MenuSeparator
    as children. It also supports \l Instantiator objects as long as the insertion is
    being done manually using \l insertItem().

    \code
    Menu {
        id: recentFilesMenu

        Instantiator {
            model: recentFilesModel
            MenuItem {
                text: model.fileName
            }
            onObjectAdded: recentFilesMenu.insertItem(index, object)
            onObjectRemoved: recentFilesMenu.removeItem(object)
        }

        MenuSeparator {
            visible: recentFilesModel.count > 0
        }

        MenuItem {
            text: "Clear menu"
            enabled: recentFilesModel.count > 0
            onTriggered: recentFilesModel.clear()
        }
    \endcode

    Note that in this case, the \c index parameter passed to \l insertItem() is relative
    to the position of the \l Instatiator in the menu, as opposed to absolute position
    in the menu.

    \sa MenuItem, MenuSeparator
*/

/*!
    \qmlproperty bool Menu::visible

    Whether the menu should be visible. This is only enabled when the menu is used as
    a submenu. Its value defaults to \c true.
*/

/*!
    \qmlproperty enumeration Menu::type

    This property is read-only and constant, and its value is \l MenuItemType.Menu.
*/

/*!
    \qmlproperty string Menu::title

    Title for the menu as a submenu or in a menubar.

    Mnemonics are supported by prefixing the shortcut letter with \&.
    For instance, \c "\&File" will bind the \c Alt-F shortcut to the
    \c "File" menu. Note that not all platforms support mnemonics.

    Its value defaults to the empty string.
*/

/*!
    \qmlproperty bool Menu::enabled

    Whether the menu is enabled, and responsive to user interaction as a submenu.
    Its value defaults to \c true.
*/

/*!
    \qmlproperty url Menu::iconSource

    Sets the icon file or resource url for the menu icon as a submenu.
    Defaults to the empty URL.

    \sa iconName
*/

/*!
    \qmlproperty string Menu::iconName

    Sets the icon name for the menu icon. This will pick the icon
    with the given name from the current theme. Only works as a submenu.

    Its value defaults to the empty string.

    \sa iconSource
*/

/*!
    \qmlmethod void Menu::popup()

    Opens this menu under the mouse cursor.
    It can block on some platforms, so test it accordingly.
*/

/*!
    \qmlmethod MenuItem Menu::addItem(string text)

    Adds an item to the menu. Returns the newly created \l MenuItem.
*/

/*!
    \qmlmethod MenuSeparator Menu::addSeparator()

    Adds a separator to the menu. Returns the newly created \l MenuSeparator.
*/

/*!
    \qmlmethod Menu Menu::addMenu(string title)
    Adds a submenu to the menu. Returns the newly created \l Menu.
*/

/*!
    \qmlmethod void Menu::insertItem(int before, object item)

    Inserts the \c item at the index \c before in the current menu.
    In this case, \c item can be either a \l MenuItem, a \l MenuSeparator,
    or a \l Menu.

    \sa removeItem()
*/

/*!
    \qmlmethod void Menu::removeItem(item)

    Removes the \c item from the menu.
    In this case, \c item can be either a \l MenuItem, a \l MenuSeparator,
    or a \l Menu.

    \sa insertItem()
*/

QtMenu::QtMenu(QObject *parent)
    : QtMenuText(parent),
      m_itemsCount(0),
      m_selectedIndex(-1),
      m_parentWindow(0),
      m_minimumWidth(0),
      m_popupWindow(0),
      m_menuContentItem(0),
      m_popupVisible(false),
      m_containersCount(0)
{
    connect(this, SIGNAL(__textChanged()), this, SIGNAL(titleChanged()));

    m_platformMenu = QGuiApplicationPrivate::platformTheme()->createPlatformMenu();
    if (m_platformMenu) {
        connect(m_platformMenu, SIGNAL(aboutToHide()), this, SLOT(__closeMenu()));
        if (platformItem())
            platformItem()->setMenu(m_platformMenu);
    }
}

QtMenu::~QtMenu()
{
    delete m_platformMenu;
    m_platformMenu = 0;
}

void QtMenu::updateText()
{
    if (m_platformMenu)
        m_platformMenu->setText(this->text());
    QtMenuText::updateText();
}

void QtMenu::setMinimumWidth(int w)
{
    if (w == m_minimumWidth)
        return;

    m_minimumWidth = w;
    if (m_platformMenu)
        m_platformMenu->setMinimumWidth(w);
}

void QtMenu::setFont(const QFont &arg)
{
    if (m_platformMenu)
        m_platformMenu->setFont(arg);
}

void QtMenu::setSelectedIndex(int index)
{
    if (m_selectedIndex == index)
        return;

    m_selectedIndex = index;
    emit __selectedIndexChanged();
}

void QtMenu::updateSelectedIndex()
{
    if (QtMenuItem *menuItem = qobject_cast<QtMenuItem*>(sender())) {
        int index = indexOfMenuItem(menuItem);
        setSelectedIndex(index);
    }
}

QtMenuItems QtMenu::menuItems()
{
    return QtMenuItems(this, 0, &QtMenu::append_menuItems, &QtMenu::count_menuItems,
                       &QtMenu::at_menuItems, &QtMenu::clear_menuItems);
}

QQuickWindow *QtMenu::findParentWindow()
{
    if (!m_parentWindow) {
        QQuickItem *parentAsItem = qobject_cast<QQuickItem *>(parent());
        m_parentWindow = visualItem() ? visualItem()->window() :    // Menu as menu item case
                         parentAsItem ? parentAsItem->window() : 0; //Menu as context menu/popup case
    }
    return m_parentWindow;
}

void QtMenu::popup()
{
    QPoint mousePos = QCursor::pos();
    if (QQuickWindow *parentWindow = findParentWindow())
        mousePos = parentWindow->mapFromGlobal(mousePos);

    __popup(mousePos.x(), mousePos.y());
}

void QtMenu::__popup(qreal x, qreal y, int atItemIndex)
{
    if (popupVisible())
        __closeMenu();

    setPopupVisible(true);

    QtMenuBase *atItem = menuItemAtIndex(atItemIndex);

    QQuickWindow *parentWindow = findParentWindow();

    if (m_platformMenu) {
        QPointF screenPosition(x, y);
        if (visualItem())
            screenPosition = visualItem()->mapToScene(screenPosition);
        m_platformMenu->showPopup(parentWindow, screenPosition.toPoint(), atItem ? atItem->platformItem() : 0);
    } else {
        m_popupWindow = new QtMenuPopupWindow();
        if (visualItem())
            m_popupWindow->setParentItem(visualItem());
        else
            m_popupWindow->setParentWindow(parentWindow);
        m_popupWindow->setMenuContentItem(m_menuContentItem);
        m_popupWindow->setItemAt(atItem ? atItem->visualItem() : 0);

        connect(m_popupWindow, SIGNAL(visibleChanged(bool)), this, SLOT(windowVisibleChanged(bool)));

        m_popupWindow->setPosition(x, y);
        m_popupWindow->show();
    }
}

void QtMenu::setMenuContentItem(QQuickItem *item)
{
    if (m_menuContentItem != item)
        m_menuContentItem = item;
}

void QtMenu::setPopupVisible(bool v)
{
    if (m_popupVisible != v) {
        m_popupVisible = v;
        emit popupVisibleChanged();
    }
}

void QtMenu::__closeMenu()
{
    setPopupVisible(false);
    if (m_popupWindow)
        m_popupWindow->setVisible(false);
    m_parentWindow = 0;
    emit __menuClosed();
}

void QtMenu::__dismissMenu()
{
    QtMenuPopupWindow *topMenuWindow = m_popupWindow;
    while (topMenuWindow) {
        QtMenuPopupWindow *pw = qobject_cast<QtMenuPopupWindow *>(topMenuWindow->transientParent());
        if (!pw)
            topMenuWindow->dismissMenu();
        topMenuWindow = pw;
    }
}

void QtMenu::windowVisibleChanged(bool v)
{
    if (!v) {
        if (qobject_cast<QtMenuPopupWindow *>(m_popupWindow->transientParent())) {
            m_popupWindow->transientParent()->setMouseGrabEnabled(true);
            m_popupWindow->transientParent()->setKeyboardGrabEnabled(true);
        }
        m_popupWindow->deleteLater();
        m_popupWindow = 0;
        __closeMenu();
    }
}

void QtMenu::itemIndexToListIndex(int itemIndex, int *listIndex, int *containerIndex) const
{
    *listIndex = -1;
    QtMenuItemContainer *container = 0;
    while (itemIndex >= 0 && ++*listIndex < m_menuItems.count())
        if ((container = qobject_cast<QtMenuItemContainer *>(m_menuItems[*listIndex])))
            itemIndex -= container->items().count();
        else
            --itemIndex;

    if (container)
        *containerIndex = container->items().count() + itemIndex;
    else
        *containerIndex = -1;
}

int QtMenu::itemIndexForListIndex(int listIndex) const
{
    int index = 0;
    int i = 0;
    while (i < listIndex && i < m_menuItems.count())
        if (QtMenuItemContainer *container = qobject_cast<QtMenuItemContainer *>(m_menuItems[i++]))
            index += container->items().count();
        else
            ++index;

    return index;
}

QtMenuBase *QtMenu::nextMenuItem(QtMenu::MenuItemIterator *it) const
{
    if (it->containerIndex != -1) {
        QtMenuItemContainer *container = qobject_cast<QtMenuItemContainer *>(m_menuItems[it->index]);
        if (++it->containerIndex < container->items().count())
            return container->items()[it->containerIndex];
    }

    if (++it->index < m_menuItems.count()) {
        if (QtMenuItemContainer *container = qobject_cast<QtMenuItemContainer *>(m_menuItems[it->index])) {
            it->containerIndex = 0;
            return container->items()[0];
        } else {
            it->containerIndex = -1;
            return m_menuItems[it->index];
        }
    }

    return 0;
}

QtMenuBase *QtMenu::menuItemAtIndex(int index) const
{
    if (0 <= index && index < m_itemsCount) {
        if (!m_containersCount) {
            return m_menuItems[index];
        } else if (m_containersCount == 1 && m_menuItems.count() == 1) {
            QtMenuItemContainer *container = qobject_cast<QtMenuItemContainer *>(m_menuItems[0]);
            return container->items()[index];
        } else {
            int containerIndex;
            int i;
            itemIndexToListIndex(index, &i, &containerIndex);
            if (containerIndex != -1) {
                QtMenuItemContainer *container = qobject_cast<QtMenuItemContainer *>(m_menuItems[i]);
                return container->items()[containerIndex];
            } else {
                return m_menuItems[i];
            }
        }
    }

    return 0;
}

bool QtMenu::contains(QtMenuBase *item)
{
    if (item->container())
        return item->container()->items().contains(item);

    return m_menuItems.contains(item);
}

int QtMenu::indexOfMenuItem(QtMenuBase *item) const
{
    if (!item)
        return -1;
    if (item->container()) {
        int containerIndex = m_menuItems.indexOf(item->container());
        if (containerIndex == -1)
            return -1;
        int index = item->container()->items().indexOf(item);
        return index == -1 ? -1 : itemIndexForListIndex(containerIndex) + index;
    } else {
        int index = m_menuItems.indexOf(item);
        return index == -1 ? -1 : itemIndexForListIndex(index);
    }
}

QtMenuItem *QtMenu::addItem(QString title)
{
    QtMenuItem *item = new QtMenuItem(this);
    item->setText(title);
    insertItem(m_itemsCount, item);
    return item;
}

QtMenuSeparator *QtMenu::addSeparator()
{
    QtMenuSeparator *item = new QtMenuSeparator(this);
    insertItem(m_itemsCount, item);
    return item;
}

void QtMenu::insertItem(int index, QtMenuBase *menuItem)
{
    if (!menuItem)
        return;
    int itemIndex;
    if (m_containersCount) {
        QtMenuItemContainer *container = menuItem->parent() != this ? m_containers[menuItem->parent()] : 0;
        if (container) {
            container->insertItem(index, menuItem);
            itemIndex = itemIndexForListIndex(m_menuItems.indexOf(container)) + index;
        } else {
            itemIndex = itemIndexForListIndex(index);
            m_menuItems.insert(itemIndex, menuItem);
        }
    } else {
        itemIndex = index;
        m_menuItems.insert(index, menuItem);
    }

    setupMenuItem(menuItem, itemIndex);
    emit itemsChanged();
}

void QtMenu::removeItem(QtMenuBase *menuItem)
{
    if (!menuItem)
        return;
    menuItem->setParentMenu(0);

    QtMenuItemContainer *container = menuItem->parent() != this ? m_containers[menuItem->parent()] : 0;
    if (container)
        container->removeItem(menuItem);
    else
        m_menuItems.removeOne(menuItem);

    --m_itemsCount;
    emit itemsChanged();
}

void QtMenu::clear()
{
    m_containers.clear();
    m_containersCount = 0;

    while (!m_menuItems.empty())
        delete m_menuItems.takeFirst();
    m_itemsCount = 0;
}

void QtMenu::setupMenuItem(QtMenuBase *item, int platformIndex)
{
    item->setParentMenu(this);
    if (m_platformMenu) {
        QPlatformMenuItem *before = 0;
        if (platformIndex != -1)
            before = m_platformMenu->menuItemAt(platformIndex);
        m_platformMenu->insertMenuItem(item->platformItem(), before);
    }
    ++m_itemsCount;
}

void QtMenu::append_menuItems(QtMenuItems *list, QObject *o)
{
    if (QtMenu *menu = qobject_cast<QtMenu *>(list->object)) {
        Q_ASSERT(o->parent() == menu);

        if (QtMenuBase *menuItem = qobject_cast<QtMenuBase *>(o)) {
            menu->m_menuItems.append(menuItem);
            menu->setupMenuItem(menuItem);
        } else {
            QtMenuItemContainer *menuItemContainer = new QtMenuItemContainer(menu);
            menu->m_menuItems.append(menuItemContainer);
            menu->m_containers.insert(o, menuItemContainer);
            menuItemContainer->setParentMenu(menu);
            ++menu->m_containersCount;
            foreach (QObject *child, o->children()) {
                if (QtMenuBase *item = qobject_cast<QtMenuBase *>(child)) {
                    menuItemContainer->insertItem(-1, item);
                    menu->setupMenuItem(item);
                }
            }
        }
    }
}

int QtMenu::count_menuItems(QtMenuItems *list)
{
    if (QtMenu *menu = qobject_cast<QtMenu *>(list->object))
        return menu->m_itemsCount;

    return 0;
}

QObject *QtMenu::at_menuItems(QtMenuItems *list, int index)
{
    if (QtMenu *menu = qobject_cast<QtMenu *>(list->object))
        return menu->menuItemAtIndex(index);

    return 0;
}

void QtMenu::clear_menuItems(QtMenuItems *list)
{
    if (QtMenu *menu = qobject_cast<QtMenu *>(list->object))
        menu->clear();
}

QT_END_NAMESPACE
