import QtQuick 2.0

Item {
    width: ListView.view.width
    height: ListView.view.height

    signal clicked()

    Rectangle {
        anchors.fill: parent
        color: model.filter ? "white" : "black"
        opacity: 0.9
    }

    Image {
        anchors.centerIn: parent
        source: model.filePath
        width: (parent.width < sourceSize.width) ? parent.width : sourceSize.width
        height: (parent.height < sourceSize.width) ? parent.height : sourceSize.height
        fillMode: Image.PreserveAspectFit
    }
    MouseArea {
        anchors.fill: parent
        onClicked: parent.clicked()
    }
}
