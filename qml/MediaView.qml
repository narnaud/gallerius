import QtQuick 2.0
import QtMultimedia 5.8
import Gallerius 1.0

Item {
    id: root
    property bool filter: false
    property string imagePath: ""
    property string videoPath: ""
    property int type: Media.NoType

    function togglePlay() {
        if (type != Media.Video)
            return;
        if (video.isPlaying)
            video.pause()
        else
            video.play()
    }

    onImagePathChanged: {
        video.stop()
        image.visible = true
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.9
    }

    Image {
        id: image
        anchors.centerIn: parent
        source: root.imagePath
        width: (parent.width < sourceSize.width) ? parent.width : sourceSize.width
        height: (parent.height < sourceSize.width) ? parent.height : sourceSize.height
        fillMode: Image.PreserveAspectFit
    }

    Video {
        id: video
        anchors.fill: parent
        source: root.videoPath
        property bool isPlaying: false
        visible: root.type == Media.Video

        onPlaying: {
            image.visible = false
            isPlaying = true
        }
        onPaused: isPlaying = false
        onStopped: isPlaying = false

        MouseArea {
            anchors.fill: parent
            onClicked: togglePlay()
        }
    }

    // Filtering button
    Image {
        anchors.centerIn: parent
        visible: parent.filter

        source: "qrc:///assets/eye-slash.png"
    }
}
