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
        onClicked: parent.GridView.view.currentIndex = model.index
    }

    Rectangle {
        id: image
        anchors.centerIn: parent
        width: Style.imageSize
        height: Style.imageSize
        radius: 20

        color: model.type === Media.Dir ? "red" : "orange"

        Text {
            anchors.centerIn: parent
            text: model.fileName
        }
    }

    DropShadow {
        anchors.fill: image
        horizontalOffset: 3
        verticalOffset: 3
        radius: 8.0
        samples: 17
        color: "#80000000"
        source: image
    }
}
