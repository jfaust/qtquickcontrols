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

#include "qwheelarea_p.h"

QT_BEGIN_NAMESPACE

// On Mac OS X, the scrolling speed in Safari is roughly 2.5 times faster
// than in TextEdit (the native app). The former is using high-resolution
// pixel-based delta values as they are, which is fine for a typical web
// content, whereas the latter evidently makes scrolling slower to make it
// feel natural and more precise for typical document type of content.
// => we'll compromise between the two for now, and pick an arbitrary value
//    to make the pixel-based scrolling speed something between the two
static const qreal pixelDeltaAdjustment = 0.5;

// The default scroll speed for typical angle-based mouse wheels. The value
// comes originally from QTextEdit, which sets 20px steps by default.
static const qreal defaultScrollSpeed = 20.0;

QWheelArea::QWheelArea(QQuickItem *parent)
    : QQuickItem(parent),
      m_horizontalMinimumValue(0),
      m_horizontalMaximumValue(0),
      m_verticalMinimumValue(0),
      m_verticalMaximumValue(0),
      m_horizontalValue(0),
      m_verticalValue(0),
      m_verticalDelta(0),
      m_horizontalDelta(0),
      m_scrollSpeed(defaultScrollSpeed)
{

}

QWheelArea::~QWheelArea()
{

}

void QWheelArea::wheelEvent(QWheelEvent *we)
{
    QPoint numPixels = we->pixelDelta();
    QPoint numDegrees = we->angleDelta() / 8;

    if (!numPixels.isNull()) {
        setHorizontalDelta(numPixels.x() * pixelDeltaAdjustment);
        setVerticalDelta(numPixels.y() * pixelDeltaAdjustment);
    } else if (!numDegrees.isNull()) {
        setHorizontalDelta(numDegrees.x() / 15.0 * m_scrollSpeed);
        setVerticalDelta(numDegrees.y() / 15.0 * m_scrollSpeed);
    }

    we->accept();
}

void QWheelArea::setHorizontalMinimumValue(qreal value)
{
    m_horizontalMinimumValue = value;
}

qreal QWheelArea::horizontalMinimumValue() const
{
    return m_horizontalMinimumValue;
}

void QWheelArea::setHorizontalMaximumValue(qreal value)
{
    m_horizontalMaximumValue = value;
}

qreal QWheelArea::horizontalMaximumValue() const
{
    return m_horizontalMaximumValue;
}

void QWheelArea::setVerticalMinimumValue(qreal value)
{
    m_verticalMinimumValue = value;
}

qreal QWheelArea::verticalMinimumValue() const
{
    return m_verticalMinimumValue;
}

void QWheelArea::setVerticalMaximumValue(qreal value)
{
    m_verticalMaximumValue = value;
}

qreal QWheelArea::verticalMaximumValue() const
{
    return m_verticalMaximumValue;
}

void QWheelArea::setHorizontalValue(qreal value)
{
    value = qBound<qreal>(m_horizontalMinimumValue, value, m_horizontalMaximumValue);

    if (value != m_horizontalValue) {
        m_horizontalValue = value;
        emit horizontalValueChanged();
    }
}

qreal QWheelArea::horizontalValue() const
{
    return m_horizontalValue;
}

void QWheelArea::setVerticalValue(qreal value)
{
    value = qBound<qreal>(m_verticalMinimumValue, value, m_verticalMaximumValue);

    if (value != m_verticalValue) {
        m_verticalValue = value;
        emit verticalValueChanged();
    }
}

qreal QWheelArea::verticalValue() const
{
    return m_verticalValue;
}

void QWheelArea::setVerticalDelta(qreal value)
{
    m_verticalDelta = value;
    setVerticalValue(m_verticalValue - m_verticalDelta);

    emit verticalWheelMoved();
}

qreal QWheelArea::verticalDelta() const
{
    return m_verticalDelta;
}

void QWheelArea::setHorizontalDelta(qreal value)
{
    m_horizontalDelta = value;
    setHorizontalValue(m_horizontalValue - m_horizontalDelta);

    emit horizontalWheelMoved();
}

qreal QWheelArea::horizontalDelta() const
{
    return m_horizontalDelta;
}

void QWheelArea::setScrollSpeed(qreal value)
{
    if (value != m_scrollSpeed) {
        m_scrollSpeed = value;
        emit scrollSpeedChanged();
    }
}

qreal QWheelArea::scrollSpeed() const
{
    return m_scrollSpeed;
}

QT_END_NAMESPACE
