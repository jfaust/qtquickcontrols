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
import QtQuick.Controls.Styles 1.0
import QtQuick.Controls.Private 1.0

Style {
    property Component panel: Item {
        id: style

        property rect upRect
        property rect downRect

        property int topMargin: edit.anchors.topMargin
        property int leftMargin: 2 + edit.anchors.leftMargin
        property int rightMargin: 2 + edit.anchors.rightMargin
        property int bottomMargin: edit.anchors.bottomMargin
        property int horizontalTextAlignment: Qt.AlignLeft
        property int verticalTextAlignment: Qt.AlignVCenter

        property color foregroundColor: syspal.text
        property color backgroundColor: syspal.base
        property color selectionColor: syspal.highlight
        property color selectedTextColor: syspal.highlightedText

        SystemPalette {
            id: syspal
            colorGroup: control.enabled ? SystemPalette.Active : SystemPalette.Disabled
        }

        width: 100
        height: styleitem.implicitHeight

        implicitWidth: styleitem.implicitWidth
        implicitHeight: styleitem.implicitHeight

        Item {
            id: edit
            anchors.fill: parent
            Rectangle {
                color: "white"
                anchors.fill: parent
                anchors.margins: -1
            }
            FocusFrame {
                focusMargin:-6
                visible: spinbox.activeFocus && styleitem.styleHint("focuswidget")
            }
        }

        function updateRect() {
            style.upRect = styleitem.subControlRect("up");
            style.downRect = styleitem.subControlRect("down");
            var inputRect = styleitem.subControlRect("edit");
            edit.anchors.topMargin = inputRect.y
            edit.anchors.leftMargin = inputRect.x
            edit.anchors.rightMargin = style.width - inputRect.width - edit.anchors.leftMargin
            edit.anchors.bottomMargin = style.height - inputRect.height - edit.anchors.topMargin
        }

        Component.onCompleted: updateRect()
        onWidthChanged: updateRect()
        onHeightChanged: updateRect()

        StyleItem {
            id: styleitem
            elementType: "spinbox"
            anchors.fill: parent
            sunken: (control.__downEnabled && control.__downPressed) || (control.__upEnabled && control.__upPressed)
            hover: __containsMouse
            hints: control.styleHints
            hasFocus: control.activeFocus
            enabled: control.enabled
            value: (control.__upPressed ? 1 : 0)           |
                   (control.__downPressed ? 1<<1 : 0) |
                   (control.__upEnabled ? (1<<2) : 0)      |
                   (control.__downEnabled ? (1<<3) : 0)
            contentWidth: control.__contentWidth
            contentHeight: control.__contentHeight
        }
    }
}
