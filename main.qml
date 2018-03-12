import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3

import "style.js" as Style

Window {
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

    Item {
        id: header
        anchors.top: parent.top
        width: parent.width
        height: childrenRect.height
        clip: true

        BreadCrumb {}
        RoundButton {
            anchors.right: parent.right
            icon.source: "qrc:///assets/filter.png"
            checkable: true
            checked: _model.filter
            onToggled: _model.filter = checked

        }
    }

    GridView {
        id: gridView
        anchors.top: header.bottom
        anchors.bottom: footer.top
        width: parent.width

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

        Connections {
            // Ensure that the selection is cleared when changing the path
            target: _gallery
            onPathChanged: gridView.currentIndex = -1
        }
    }

    MediaViewer {
        id: mediaViewer
        anchors.fill: parent
        focus: visible
        visible: false
    }

    Item {
        id: footer
        anchors.bottom: parent.bottom
        width: parent.width
        height: childrenRect.height
        ProgressBar {
            width: parent.width
            to: _gallery.mediaCount
            value: _gallery.progressValue
            visible: to != value
        }
    }

}
