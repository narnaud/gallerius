import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import Gallerius 1.0

import "style.js" as Style

Item {
    implicitHeight: childrenRect.height

    ProgressBar {
        width: parent.width
        to: _gallery.mediaCount
        value: _gallery.progressValue
        visible: to != value
    }
}
