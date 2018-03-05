import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3

import "style.js" as Style

Window {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Gallerius")

    WelcomeScreen {
        anchors.fill: parent
        z: 100
        visible: _gallery.rootPath.toString() === ""
    }

    Connections {
        // Ensure that the selection is cleared when changing the path
        target: _gallery
        onPathChanged: gridView.currentIndex = -1
    }

    GridView {
        id: gridView

        anchors.fill: parent
        model: _model
        delegate: MediaDelegate {}

        cellWidth: Style.imageSize + Style.margin
        cellHeight: Style.imageSize + Style.margin

        focus: _gallery.rootPath.toString() !== ""

        ScrollBar.vertical: ScrollBar { id: scrollBar }
    }
    ProgressBar {
        anchors {
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
        to: _gallery.mediaCount
        value: _gallery.progressValue
        visible: to != value
    }
}
