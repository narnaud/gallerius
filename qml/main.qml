import QtQuick 2.10
import QtQuick.Window 2.10
import Gallerius 1.0

import "style.js" as Style

Window {
    id: root

    visible: true
    width: 1280
    height: 920
    title: qsTr("Gallerius")

    Shortcut {
        sequence: "F11"
        onActivated: (root.visibility === Window.FullScreen) ? root.showNormal() : root.showFullScreen()
    }

    WelcomeScreen {
        id: welcome
        anchors.fill: parent

        onPathChanged: {
            _gallery.rootPath = path
            visible = false
            mainView.visible = true
        }
    }

    MainView {
        id: mainView
        anchors.fill: parent
        visible: false
    }
}
