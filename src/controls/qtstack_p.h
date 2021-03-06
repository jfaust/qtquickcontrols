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

#ifndef QTSTACK_P_H
#define QTSTACK_P_H

#include <QtQuick/qquickitem.h>

QT_BEGIN_NAMESPACE

class QtStack : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int index READ index NOTIFY indexChanged)
    Q_PROPERTY(int __index READ index WRITE setIndex NOTIFY indexChanged)
    Q_PROPERTY(Status status READ status NOTIFY statusChanged)
    Q_PROPERTY(Status __status READ status WRITE setStatus NOTIFY statusChanged)
    Q_PROPERTY(QQuickItem* pageStack READ pageStack NOTIFY pageStackChanged)
    Q_PROPERTY(QQuickItem* __stackView READ pageStack WRITE setStackView NOTIFY pageStackChanged)
    Q_ENUMS(Status)

public:
    QtStack(QObject *object = 0);

    static QtStack *qmlAttachedProperties(QObject *object);

    int index() const;
    void setIndex(int index);

    enum Status {
        Inactive = 0,
        Deactivating = 1,
        Activating = 2,
        Active = 3
    };

    Status status() const;
    void setStatus(Status status);

    QQuickItem *pageStack() const;
    void setStackView(QQuickItem *pageStack);

signals:
    void statusChanged();
    void pageStackChanged();
    void indexChanged();

private:
    int m_index;
    Status m_status;
    QQuickItem *m_pageStack;
};

QT_END_NAMESPACE

QML_DECLARE_TYPE(QtStack)
QML_DECLARE_TYPEINFO(QtStack, QML_HAS_ATTACHED_PROPERTIES)

#endif // QTSTACK_P_H
