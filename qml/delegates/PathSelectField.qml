import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import Qt5Compat.GraphicalEffects

Item {
    id: pathSelectField
    anchors.fill: parent

    property string dltText: ""
    property string dltDesc: ""
    required property var dltValue

    LeftTextInList{
        id: textElement
        dltAnchorRight: textRightElement.left
        dltRightMargin: 15
        dltText: parent.dltText
    }

    Item{
        id: textRightElement
        anchors{
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            rightMargin: 20
        }
        width: height

        Item{
            id: folderDialogButton
            anchors{
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                rightMargin: 10
            }

            // height: folderDialogText.height
            width: height //* 0.7

            FlatButton{
                dltDescription: "Select " + dltText
                dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder.png")//Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/folder.png")
                dltImageHover: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder.png")
                onUserClicked: {
                    folderDialog.open()
                }
            }
        }

        FolderDialog {
            id: folderDialog
            title: "Select a Path"
            options: FolderDialog.ShowDirsOnly

            currentFolder: dltValue

            onAccepted:  dltValue = folderDialog.selectedFolder;
            onRejected: folderDialog.close()
        }

    }
}
