/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the Qt Components project on Qt Labs.
**
** No Commercial Usage
** This file contains pre-release code and may not be distributed.
** You may use this file in accordance with the terms and conditions contained
** in the Technology Preview License Agreement accompanying this package.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
****************************************************************************/

#ifndef QWINDOW_H
#define QWINDOW_H

#include <QtGui/QApplication>
#include <QtDeclarative>

class GraphicsView : public QGraphicsView {
    Q_OBJECT
public:
    GraphicsView(QGraphicsScene *scene) : QGraphicsView(scene) {}
protected:
    void showEvent(QShowEvent * /*event*/) {
        emit visibilityChanged();
    }

    void hideEvent(QHideEvent * /*event*/) {
        emit visibilityChanged();
    }

signals:
    void visibilityChanged();
};

class QWindow : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QDeclarativeListProperty<QObject> data READ data DESIGNABLE false)
    Q_PROPERTY(int x READ x WRITE setX NOTIFY positionChanged)
    Q_PROPERTY(int y READ y WRITE setY NOTIFY positionChanged)
    Q_PROPERTY(int height READ height WRITE setHeight NOTIFY sizeChanged)
    Q_PROPERTY(int width READ width WRITE setWidth NOTIFY sizeChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibilityChanged)
    Q_PROPERTY(bool windowDecoration READ windowDecoration WRITE setWindowDecoration NOTIFY windowDecorationChanged)

    Q_CLASSINFO("DefaultProperty", "data")

    QGraphicsScene scene;
    GraphicsView view;

    QDeclarativeListProperty<QObject> data()
    {
        return QDeclarativeListProperty<QObject>(view.scene(), 0, data_append, data_count, data_at, data_clear);
    }
    static void data_append(QDeclarativeListProperty<QObject> *, QObject *);
    static int data_count(QDeclarativeListProperty<QObject> *);
    static QObject *data_at(QDeclarativeListProperty<QObject> *, int);
    static void data_clear(QDeclarativeListProperty<QObject> *);

    bool eventFilter(QObject *, QEvent *ev) {
        switch(ev->type()) {
            case QEvent::Resize:
                emit sizeChanged();
                break;
            case QEvent::Move:
                emit positionChanged();
                break;
            default:
                break;
        }
        return false;
    }

    int x() { return view.x(); }
    int y() { return view.y(); }
    int height() { return view.height(); }
    int width() { return view.width(); }
    bool isVisible() { return view.isVisible(); }


    void setX(int x) { view.move(x, y()); }
    void setY(int y) { view.move(x(), y); }
    void setHeight(int height) { view.resize(width(), height); }
    void setWidth(int width) { view.resize(width, height()); }
    void setVisible(bool visible) { view.setVisible(visible); }

    void setWindowDecoration(bool s) {
        qDebug() << "foo" << s << __TIME__;
        view.setWindowFlags(s ? view.windowFlags() & ~Qt::FramelessWindowHint
                              : view.windowFlags() | Qt::FramelessWindowHint);
        emit windowDecorationChanged();
    }
    bool windowDecoration() {
        return !(view.windowFlags() & Qt::FramelessWindowHint);
    }


signals:
    void sizeChanged();
    void positionChanged();
    void visibilityChanged();
    void windowDecorationChanged();

public:
    QWindow() : view(&scene) {
        connect(&view, SIGNAL(visibilityChanged()), this, SIGNAL(visibilityChanged()));
//        view.show();
        view.installEventFilter(this);
    }

};

#endif // QWINDOW_H