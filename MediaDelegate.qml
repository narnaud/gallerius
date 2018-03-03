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

    Image {
        id: image
        anchors.centerIn: parent
        width: Style.imageSize
        height: Style.imageSize
        fillMode: Image.PreserveAspectFit
        source: model.type === Media.Image ? model.image : ""

        Text {
            anchors.centerIn: parent
            text: model.fileName
            visible: model.type !== Media.Image
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
    }
}
