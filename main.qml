import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3

import "style.js" as Style

ApplicationWindow {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Gallerius")

    WelcomeScreen {
        x: width / 2
        y: height / 2
        width: parent.width / 2
        height: parent.height / 2
        Component.onCompleted: open()
    }

    header: Rectangle {
        height: childrenRect.height
        color: "white"
        BreadCrumb {}

        RoundButton {
            anchors.right: parent.right
            icon.source: "qrc:///assets/filter.png"
//            flat: true
            checkable: true
            onToggled: _model.showAll = !checked

        }
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

    footer: ProgressBar {
        to: _gallery.mediaCount
        value: _gallery.progressValue
        visible: to != value
    }
}
