import QtQuick 2.10
import QtQuick.Controls 2.3

Row {
    Item {
        width: childrenRect.width
        height: childrenRect.height
        RoundButton {
            id: homeButton
            icon.source: "qrc:///assets/home.png"
//            flat: true
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
//                flat: true
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
