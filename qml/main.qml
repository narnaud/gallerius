import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import Gallerius 1.0

import "style.js" as Style

Window {
    visible: true
    width: 1280
    height: 920
    title: qsTr("Gallerius")

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
