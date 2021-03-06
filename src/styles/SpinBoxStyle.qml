/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Quick Controls module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
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
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
import QtQuick 2.1
import QtQuick.Controls 1.0

/*!
    \qmltype SpinBoxStyle
    \internal
    \inqmlmodule QtQuick.Controls.Styles 1.0
    \brief provides custom styling for SpinBox
*/

Style {
    id: spinboxStyle

    property int topMargin: 0
    property int leftMargin: 4
    property int rightMargin: 12
    property int bottomMargin: 0

    property color foregroundColor: syspal.text
    property color backgroundColor: syspal.base
    property color selectionColor: syspal.highlight
    property color selectedTextColor: syspal.highlightedText

    property Component upControl: Rectangle {
        anchors.centerIn: parent
        implicitWidth: 12
        gradient: Gradient {
            GradientStop {color: control.__upPressed ? "lightgray" : "white" ; position: 0}
            GradientStop {color: control.__upPressed ? "lightgray" : "lightgray" ; position: 1}
        }
        border.color: Qt.darker(backgroundColor, 2)
        Image {
            source: "images/arrow-up.png"
            anchors.centerIn: parent
        }
    }

    property Component downControl: Rectangle {
        implicitWidth: 12
        gradient: Gradient {
            GradientStop {color: control.__downPressed ? "lightgray" : "white" ; position: 0}
            GradientStop {color: control.__downPressed ? "lightgray" : "lightgray" ; position: 1}
        }
        border.color: Qt.darker(backgroundColor, 2)
        Image {
            source: "images/arrow-down.png"
            anchors.centerIn: parent
        }
    }

    property Component background: Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop{color: Qt.darker(backgroundColor, 1.1) ; position: 0}
            GradientStop{color: Qt.lighter(backgroundColor, 1.2) ; position: 1}
        }
        radius: 3
        antialiasing: true
        border.color: Qt.darker(backgroundColor, 2)
    }


    property Component panel:  Item {
        id: styleitem
        implicitWidth: 100
        implicitHeight: 21

        property color foregroundColor: spinboxStyle.foregroundColor
        property color backgroundColor: spinboxStyle.backgroundColor
        property color selectionColor: spinboxStyle.selectionColor
        property color selectedTextColor: spinboxStyle.selectedTextColor

        property int leftMargin: spinboxStyle.leftMargin
        property int rightMargin: spinboxStyle.rightMargin
        property int topMargin: spinboxStyle.topMargin
        property int bottomMargin: spinboxStyle.bottomMargin

        property rect upRect: Qt.rect(width - upControlLoader.implicitWidth, 0, upControlLoader.implicitWidth, height / 2 + 1)
        property rect downRect: Qt.rect(width - downControlLoader.implicitWidth, height / 2, downControlLoader.implicitWidth, height / 2)

        property int horizontalTextAlignment: Qt.AlignLeft
        property int verticalTextAlignment: Qt.AlignVCenter

        property SpinBox cref: control

        SystemPalette {
            id: syspal
            colorGroup: control.enabled ? SystemPalette.Active : SystemPalette.Disabled
        }

        Loader {
            id: backgroundLoader
            anchors.fill: parent
            sourceComponent: background
            property SpinBox control: cref
        }

        Loader {
            id: upControlLoader
            x: upRect.x
            y: upRect.y
            width: upRect.width
            height: upRect.height
            sourceComponent: upControl
            property SpinBox control: cref
        }

        Loader {
            id: downControlLoader
            x: downRect.x
            y: downRect.y
            width: downRect.width
            height: downRect.height
            sourceComponent: downControl
            property SpinBox control: cref
        }
    }
}
