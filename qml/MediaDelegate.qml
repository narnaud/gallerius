import QtQuick 2.0
import QtMultimedia 5.8
import Gallerius 1.0

Item {
    width: ListView.view.width
    height: ListView.view.height

    property bool filter: model.filter

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
            property bool isPlaying: false

            onPlaying: {
                thumbnail.visible = false
                isPlaying = true
            }
            onPaused: isPlaying = false
            onStopped: isPlaying = false

            MouseArea {
                anchors.fill: parent
                onClicked: parent.isPlaying ? video.pause() : video.play()
            }
            Image {
                id: thumbnail
                anchors.centerIn: parent
                source: model.thumbnail

            }
        }
    }

    Loader {
        anchors.fill: parent
        sourceComponent: model.type === Media.Image ? imageComponent : videoComponent
    }
}