import QtQuick 2.10
import QtGraphicalEffects 1.0
import QtQuick.Controls 2.2
import Gallerius 1.0

import "style.js" as Style

Item {
    width: Style.imageSize + Style.margin
    height: Style.imageSize + Style.margin

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: {
            if (model.type === Media.Dir)
                _gallery.setPath("file:///" + model.filePath)
        }
    }

    Rectangle {
        anchors.fill: parent
        radius: 10
        color: "lightgrey"
        visible: mouseArea.containsMouse
    }

    Image {
        id: image
        anchors.centerIn: parent
        width: Style.imageSize
        height: Style.imageSize
        fillMode: Image.PreserveAspectFit
        source: model.type === Media.Dir ? "qrc:///assets/folder.png" : model.thumbnail
        opacity: model.excluded ? (mouseArea.containsMouse ? 0.75 : 0.25) : 1.0

        Text {
            anchors.centerIn: parent
            width: parent.width
            text: model.fileName
            elide: Text.ElideLeft
            visible: model.thumbnail === ""
            color: model.type === Media.Dir ? "white" : "grey"
            font.bold: true
            font.pixelSize: parent.width / 15
            horizontalAlignment: Qt.AlignHCenter
        }
    }

    DropShadow {
        anchors.fill: image
        horizontalOffset: mouseArea.pressed ? 6 : 3
        verticalOffset: mouseArea.pressed ? 6 : 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: image
        visible: !model.excluded
    }

    Image {
        anchors {
            right: parent.right
            top: parent.top
            margins: 5
        }
        visible: mouseArea.containsMouse

        source: model.excluded ? "qrc:///assets/eye-slash.png" : "qrc:///assets/eye.png"
    }
}
