import QtQuick 2.10
import QtQuick.Controls 2.3
import Gallerius 1.0

import "style.js" as Style

Rectangle {
    implicitHeight: childrenRect.height

    color: "white"

    Row {
        Item {
            width: childrenRect.width
            height: childrenRect.height
            RoundButton {
                id: homeButton
                icon.source: "qrc:///assets/home.png"
                onClicked: _gallery.path = _gallery.rootPath
            }
            Image {
                visible: _gallery.pathList.length !== 0
                anchors.left: homeButton.right
                anchors.verticalCenter: homeButton.verticalCenter
                source: "qrc:///assets/angle-right.png"
            }
        }

        Repeater {
            model: _gallery.pathList

            Item {
                width: childrenRect.width
                height: childrenRect.height
                RoundButton {
                    id: button
                    text: modelData
                    font.bold: true
                    font.pixelSize: 16
                    onClicked: {
                        var path = _gallery.rootPath.toString()
                        for (var i = 0; i <= index; i++)
                            path += '/' + _gallery.pathList[i];
                        _gallery.path = path
                    }
                }

                Image {
                    visible: _gallery.pathList.length !== index + 1
                    anchors.left: button.right
                    anchors.verticalCenter: button.verticalCenter
                    source: "qrc:///assets/angle-right.png"
                }
            }
        }
    }

    RoundButton {
        anchors.right: parent.right
        icon.source: "qrc:///assets/filter.png"
        checkable: true
        checked: _model.filter
        onToggled: _model.filter = checked
    }
}
