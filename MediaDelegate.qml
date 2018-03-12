import QtQuick 2.0
import QtMultimedia 5.8
import Gallerius 1.0

Item {
    width: ListView.view.width
    height: ListView.view.height

    signal clicked()

    Rectangle {
        anchors.fill: parent
        color: model.filter ? "white" : "black"
        opacity: 0.9
    }

    MouseArea {
        anchors.fill: parent
        onClicked: parent.clicked()
    }

    Component {
        id: imageComponent
        Image {
            anchors.centerIn: parent
            source: model.filePath
            width: (parent.width < sourceSize.width) ? parent.width : sourceSize.width
            height: (parent.height < sourceSize.width) ? parent.height : sourceSize.height
            fillMode: Image.PreserveAspectFit
        }
    }

    Component {
        id: videoComponent
        Video {
            id: video
            anchors.fill: parent
            source: model.filePath
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    video.play()
                }
            }
        }
    }

    Loader {
        anchors.fill: parent
        sourceComponent: model.type === Media.Image ? imageComponent : videoComponent
    }
}
