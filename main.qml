import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3

import "style.js" as Style

ApplicationWindow {
    visible: true
    width: 1280
    height: 920
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
            checkable: true
            checked: _model.filter
            onToggled: _model.filter = checked

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
        delegate: GalleryDelegate {
            onMediaClicked: {
                mediaViewer.currentIndex = _mediaModel.mapRowFromSource(index)
                mediaViewer.visible = true
            }
        }

        cellWidth: Style.imageSize + Style.margin
        cellHeight: Style.imageSize + Style.margin

        snapMode: GridView.SnapToRow

        focus: !mediaViewer.visible

        ScrollBar.vertical: ScrollBar { id: scrollBar }
    }

    MediaViewer {
        id: mediaViewer
        anchors.fill: parent
        visible: false
        focus: visible
    }

    footer: ProgressBar {
        to: _gallery.mediaCount
        value: _gallery.progressValue
        visible: to != value
    }
}
