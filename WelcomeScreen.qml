import QtQuick 2.10
import QtQuick.Controls 2.3
import Qt.labs.platform 1.0

Item {
    Column {
        spacing: 20
        anchors.centerIn: parent
        Button {
            text: qsTr("Open New Directory")
            onClicked: folderDialog.open()
        }
        Button {
            text: qsTr("Open Last Directory")
            enabled: _savedRootPath.toString() !== ""
            onClicked: _gallery.rootPath = _savedRootPath
        }
    }

    FolderDialog {
        id: folderDialog
        currentFolder: _savedRootPath.toString() === ""
                       ? StandardPaths.standardLocations(StandardPaths.PicturesLocation)[0]
                       : _savedRootPath

        onAccepted: _gallery.rootPath = folder
    }
}
