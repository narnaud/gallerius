import QtQuick 2.10
import QtQuick.Controls 2.3

ListView {
    id: view
    anchors.fill: parent
    focus: true
    orientation: ListView.Horizontal

    model: _mediaModel
    delegate: MediaDelegate {}

    highlightMoveDuration: 0
    highlightRangeMode: ListView.StrictlyEnforceRange
    snapMode: ListView.SnapOneItem
}
