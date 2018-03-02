import QtQuick 2.10
import QtQuick.Window 2.10

Window {
    visible: true
    width: 1024
    height: 768
    title: qsTr("Gallerius")

    WelcomeScreen {
        anchors.fill: parent
        visible: _gallery.rootPath.toString() === ""
    }
}
