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
    \qmltype ButtonStyle
    \internal
    \inqmlmodule QtQuick.Controls.Styles 1.0
    \brief provides custom styling for Button
*/

Style {
    id: buttonstyle
    property font font
    property color backgroundColor: "lightGray"
    property color foregroundColor: "black"

    property Component label: Item {
        Text {
            id: textitem
            anchors.centerIn: parent
            renderType: Text.NativeRendering
            text: control.text
            color: buttonstyle.foregroundColor
            font: buttonstyle.font
        }
    }

    property Component background: Rectangle {
        implicitWidth: 80
        implicitHeight: 21
        gradient: Gradient {
            GradientStop {color: control.pressed ? Qt.lighter(buttonstyle.backgroundColor, 1.1) :
                                                   Qt.lighter(buttonstyle.backgroundColor, 1.8)  ; position: 0}
            GradientStop {color: control.pressed ? Qt.lighter(buttonstyle.backgroundColor, 1.1) :
                                                   buttonstyle.backgroundColor ; position: 1.4}
        }
        border.color: Qt.darker(buttonstyle.backgroundColor, 1.4)
        radius: 3
        antialiasing: true
    }

    property Component panel: Item {
        property Item controlref: control
        anchors.fill: parent

        implicitWidth: backgroundLoader.implicitWidth
        implicitHeight: backgroundLoader.implicitHeight

        Loader {
            id: backgroundLoader
            anchors.fill: parent
            sourceComponent: background
            property Item control: controlref
            property Item label: labelLoader.item
        }

        Loader {
            id: labelLoader
            sourceComponent: label
            anchors.fill: parent
            property Item control: controlref
        }
    }
}

