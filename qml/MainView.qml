import QtQuick 2.10
import QtQuick.Controls 2.3
import Gallerius 1.0

import "style.js" as Style

Item {
    id: mainView
    Shortcut {
        enabled: mainView.visible
        sequences: ["Ctrl+Up", "Ctrl+Left"]
        onActivated: _gallery.cdUp()
    }

    Shortcut {
        enabled: mainView.visible
        sequence: "Left"
        onActivated: {
            if (media.visible && grid.currentIndex != 0) {
                if (_model.isDirectory(grid.currentIndex - 1))
                    return
            }
            grid.moveCurrentIndexLeft()
        }
    }

    Shortcut {
        enabled: mainView.visible
        sequence: "Right"
        onActivated: grid.moveCurrentIndexRight()
    }

    Shortcut {
        enabled: mainView.visible && !media.visible
        sequence: "Up"
        onActivated: grid.moveCurrentIndexUp()
    }

    Shortcut {
        enabled: mainView.visible && !media.visible
        sequence: "Down"
        onActivated: grid.moveCurrentIndexDown()
    }

    Shortcut {
        enabled: mainView.visible
        sequence: "Space"
        onActivated: {
            if (media.visible) {
                media.togglePlay()
            }
            else if (grid.currentIndex !== -1) {
                if (grid.currentItem.type === Media.Dir)
                    _gallery.setPath(grid.currentItem.path)
                else
                    media.visible = true
            }
        }
    }

    Shortcut {
        enabled: mainView.visible
        sequence: "F"
        onActivated: _model.filter = !_model.filter
    }

    Shortcut {
        enabled: mainView.visible
        sequence: "Ctrl+Space"
        onActivated: {
            if (grid.currentIndex !== -1) {
                _gallery.toggleMediaFilter(grid.currentIndex)
            }
        }
    }

    Shortcut {
        enabled: mainView.visible
        sequence: "Esc"
        onActivated: media.visible = false
    }


    Header {
        id: header
        anchors.top: parent.top
        width: parent.width
        z: 1
    }

    GridView {
        id: grid
        anchors.top: header.bottom
        anchors.bottom: footer.top
        width: parent.width

        model: _model
        delegate: GalleryDelegate {}

        cellWidth: Style.imageSize + Style.margin
        cellHeight: Style.imageSize + Style.margin
        snapMode: GridView.SnapToRow
        interactive: false

        ScrollBar.vertical: ScrollBar { id: scrollBar }

        Connections {
            // Ensure that the selection is cleared when changing the path
            target: _gallery
            onPathChanged: grid.currentIndex = 0
        }
    }

    Footer {
        id: footer
        anchors.bottom: parent.bottom
        width: parent.width
    }

    MediaView {
        id: media
        anchors.fill: parent
        visible: false
        z: 10

        filter: grid.currentItem.filter
        type: grid.currentItem !== null ? grid.currentItem.type : Media.NoType
        imagePath: {
            if (grid.currentItem === null) return ""
            switch (grid.currentItem.type) {
            case Media.Video: return grid.currentItem.thumbnail
            case Media.Image: return grid.currentItem.path
            default: return ""
            }
        }
        videoPath: {
            if (grid.currentItem === null || grid.currentItem.type !== Media.Video) return ""
            return grid.currentItem.path
        }
    }
}

