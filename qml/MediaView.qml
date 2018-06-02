import QtQuick 2.0
import QtMultimedia 5.8
import Gallerius 1.0

Item {
    id: root
    property bool filter: false
    property var path: ""
    property var type: Media.NoType

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.9
    }

    Component {
        id: imageComponent
        Image {
            anchors.centerIn: parent
            source: root.path
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
            source: root.path
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
        sourceComponent: {
            switch (root.type) {
            case Media.Image: imageComponent; break;
            case Media.Video: imageComponent; break;
            default: null
            }
        }
    }
}
