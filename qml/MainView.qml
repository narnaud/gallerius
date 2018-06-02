import QtQuick 2.10
import QtQuick.Controls 2.3
import Gallerius 1.0

import "style.js" as Style

Item {
    onVisibleChanged: if (visible) grid.focus = true

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

        // The grid always keep the focus
        onFocusChanged:  focus = true

        ScrollBar.vertical: ScrollBar { id: scrollBar }

        Connections {
            // Ensure that the selection is cleared when changing the path
            target: _gallery
            onPathChanged: grid.currentIndex = 0
        }

        Keys.onSpacePressed: {
            if (currentItem.type === Media.Dir)
                _gallery.setPath(currentItem.path)
        }
    }

    Footer {
        id: footer
        anchors.bottom: parent.bottom
        width: parent.width
    }
}

