import QtQuick 2.10
import QtQuick.Controls 2.2
import Gallerius 1.0

import "style.js" as Style

Item {
    width: Style.imageSize + Style.margin
    height: Style.imageSize + Style.margin

    signal mediaClicked(int index);

    MouseArea {
        id: mouseArea
        hoverEnabled: true
        anchors.fill: parent
        onClicked: {
            if (model.type === Media.Dir)
                _gallery.setPath(model.filePath)
            else
                mediaClicked(model.index)
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
        opacity: model.filter ? (mouseArea.containsMouse ? 0.75 : 0.25) : 1.0

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

    Image {
        anchors {
            right: parent.right
            top: parent.top
            margins: 5
        }
        visible: mouseArea.containsMouse

        source: model.filter ? "qrc:///assets/eye-slash.png" : "qrc:///assets/eye.png"

        MouseArea {
            anchors.centerIn: parent
            height: 30
            width: 30
            cursorShape: Qt.PointingHandCursor
            onClicked: _gallery.toggleMediaFilter(model.index)
        }
    }
}