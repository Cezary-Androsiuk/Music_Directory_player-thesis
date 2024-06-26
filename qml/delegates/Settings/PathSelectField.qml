import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import Qt5Compat.GraphicalEffects

import "qrc:/Music_directory_player/qml/delegates/Settings/Common" // LeftText
import "qrc:/Music_directory_player/qml/components" // ImageButton

Item {
    id: pathSelectField
    anchors.fill: parent

    property string dltText: ""
    property string dltDesc: ""
    required property var dltValue

    LeftText{
        id: textElement
        dltAnchorRight: textRightElement.left
        dltRightMargin: 15
        dltText: parent.dltText
        dltDesc: parent.dltDesc
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

            ImageButton{
                dltDescription: "Select " + dltText
                // I CAN' DECIDE WHICH IS BETTER
                // ios static
                dltImageIdle: dltImageHover
                dltImageHover: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder_ios.png")

                // ios interactive
                // dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/folder_ios.png")
                // dltImageHover: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder_ios.png")

                // windows static
                // dltImageIdle: dltImageHover
                // dltImageHover: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder_windows.png")

                // windows interactive
                // dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/folder_windows.png")
                // dltImageHover: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder_windows.png")

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
