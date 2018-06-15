import QtQuick 2.10
import Gallerius 1.0

import "style.js" as Style

Item {
    property var type: model.type
    property var path: model.filePath
    property var thumbnail: model.thumbnail

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
        color: "transparent"
        border.color: "grey"
        border.width: 1
        visible: parent.GridView.isCurrentItem
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

    // Filtering button
    Image {
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.margins: 5
        visible: model.filter || mouseArea.containsMouse || parent.GridView.isCurrentItem

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
