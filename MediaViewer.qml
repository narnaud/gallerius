import QtQuick 2.10
import QtQuick.Controls 2.3

FocusScope {
    property alias currentIndex: view.currentIndex

    Rectangle {
        anchors.fill: parent
        color: view.currentItem.filter ? "white" : "black"
        opacity: 0.9
    }

    MouseArea {
        anchors.fill: parent
        onClicked: parent.visible = false
    }

    ListView {
        id: view
        anchors.fill: parent
        anchors.margins: 50
        clip: true

        focus: true
        model: _mediaModel
        delegate: MediaDelegate {}

        orientation: ListView.Horizontal
        highlightMoveDuration: 0
        highlightRangeMode: ListView.StrictlyEnforceRange
        snapMode: ListView.SnapOneItem
    }
}
