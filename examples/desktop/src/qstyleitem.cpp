/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the examples of the Qt Toolkit.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOTgall
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qstyleitem.h"

#include <QtGui/QPainter>
#include <QtGui/QStyle>
#include <QtGui/QStyleOption>
#include <QtGui/QApplication>


QStyleItem::QStyleItem(QDeclarativeItem *parent)
    : QDeclarativeItem(parent),
    m_sunken(false),
    m_raised(false),
    m_active(false),
    m_enabled(true),
    m_selected(false),
    m_focus(false),
    m_on(false),
    m_horizontal(true)
{
    setFlag(QGraphicsItem::ItemHasNoContents, false);
    setCacheMode(QGraphicsItem::DeviceCoordinateCache);
}

void QStyleItem::initStyleOption(QStyleOption *opt) const
{
    if (m_enabled)
        opt->state |= QStyle::State_Enabled;
    if (m_active)
        opt->state |= QStyle::State_Active;
    if (m_sunken)
        opt->state |= QStyle::State_Sunken;
    if (m_raised)
        opt->state |= QStyle::State_Raised;
    if (m_selected)
        opt->state |= QStyle::State_Selected;
    if (m_focus)
        opt->state |= QStyle::State_HasFocus;
    if (m_on)
        opt->state |= QStyle::State_On;
    if (m_hover)
        opt->state |= QStyle::State_MouseOver;
    if (m_horizontal)
        opt->state |= QStyle::State_Horizontal;

    opt->rect = QRect(x(), y(), width(), height());
}


void QStyleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
{
    if (m_type == QLatin1String("button")) {
        QStyle::ControlElement control = QStyle::CE_PushButton;
        QStyleOptionButton opt;
        initStyleOption(&opt);
        qApp->style()->drawControl(control, &opt, painter, 0);
    } else if (m_type == QLatin1String("checkbox")) {
        QStyle::ControlElement control = QStyle::CE_CheckBox;
        QStyleOptionButton opt;
        initStyleOption(&opt);
        qApp->style()->drawControl(control, &opt, painter, 0);
    } else if (m_type == QLatin1String("radiobutton")) {
        QStyle::ControlElement control = QStyle::CE_RadioButton;
        QStyleOptionButton opt;
        initStyleOption(&opt);
        qApp->style()->drawControl(control, &opt, painter, 0);
    } else if (m_type == QLatin1String("edit")) {
        QStyle::PrimitiveElement control = QStyle::PE_FrameLineEdit;
        QStyleOptionFrameV3 opt;
        initStyleOption(&opt);
        qApp->style()->drawPrimitive(control, &opt, painter, 0);
    } else if (m_type == QLatin1String("slidergroove")) {
        QStyle::ComplexControl control = QStyle::CC_Slider;
        QStyleOptionSlider opt;
        opt.minimum = 0;
        opt.maximum = 100;
        opt.subControls |= (QStyle::SC_SliderGroove);
        opt.activeSubControls = QStyle::SC_SliderHandle;
        initStyleOption(&opt);
        qApp->style()->drawComplexControl(control, &opt, painter, 0);
    } else if (m_type == QLatin1String("combobox")) {
        QStyle::ComplexControl control = QStyle::CC_ComboBox;
        QStyleOptionSlider opt;
        opt.minimum = 0;
        opt.maximum = 100;
        opt.subControls |= (QStyle::SC_SliderGroove);
        opt.activeSubControls = QStyle::SC_SliderHandle;
        initStyleOption(&opt);
        qApp->style()->drawComplexControl(control, &opt, painter, 0);
    } else if (m_type == QLatin1String("slider")) {
        QStyle::ComplexControl control = QStyle::CC_Slider;
        QStyleOptionSlider opt;
        opt.minimum = minimum();
        opt.maximum = maximum();
        opt.sliderPosition = value();
        opt.subControls |= (QStyle::SC_SliderGroove);
        opt.activeSubControls = QStyle::SC_SliderHandle;
        initStyleOption(&opt);
        qApp->style()->drawComplexControl(control, &opt, painter, 0);
    }
}