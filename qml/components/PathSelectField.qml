import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs
import Qt5Compat.GraphicalEffects

Item {
    id: pathSelectField
    anchors.fill: parent

    property var delegate_text: null
    property string delegate_value: ""
    property bool delegate_enabled: true

    Text{
        anchors{
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 20
            right: textRightElement.left
            rightMargin: 10
        }
        text: delegate_text
        color: root.color_accent1
        font.pixelSize: 15
        verticalAlignment: Text.AlignVCenter

        // elide: Text.ElideRight
        clip: true

        MouseArea{
            anchors.fill: parent
            hoverEnabled:  parent.contentWidth > parent.width

            ToolTip.visible: containsMouse
            ToolTip.text: delegate_text
        }
    }

    Item{
        id: textRightElement
        anchors{
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            rightMargin: 20
        }
        width: parent.width * 0.4

        TextField{
            id: folderDialogText
            anchors{
                top: parent.top
                left: parent.left
                bottom: parent.bottom
                right: folderDialogButton.left
            }
            x: -10

            font.pixelSize: 15
            clip: true

            enabled: delegate_enabled

            text: delegate_value;

            onEditingFinished: {
                text = delegate_value;
            }

            ToolTip.visible: hovered && (width < (contentWidth * 1.3))
            ToolTip.text: text
        }

        Item{
            id: folderDialogButton
            anchors{
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }

            // height: folderDialogText.height
            width: height * 0.7

            FlatButton{
                dltDescription: "Select File"
                dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/folder.png")
                dltImageHover: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder.png")
                onUserClicked: {
                    folderDialog.open()
                }
            }

        //     Image{
        //         id: img
        //         fillMode: Image.PreserveAspectFit
        //         anchors{
        //             fill: parent
        //             topMargin: 15
        //             margins: parent.width * 0.15
        //         }

        //         source: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/folder.png")
        //     }

        //     ColorOverlay {
        //         anchors.fill: img
        //         source: img
        //         color: root.dark_theme ? rgb(96,96,96) : rgb(158,158,158)
        //         opacity: delegate_enabled ? 1.0 : 0.4
        //     }

        //     MouseArea{
        //         anchors.fill: parent
        //         hoverEnabled: true

        //         enabled: delegate_enabled

        //         onEntered: {
        //             img.source =  Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/opened_folder.png")
        //         }
        //         onExited: {
        //             img.source =  Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/folder.png")
        //         }
        //         onPressed: folderDialog.open()
        //     }

        //     // ToolTip.visible: hovered
        //     // ToolTip.text: "Select File"
        }

        FolderDialog {
            id: folderDialog
            title: "Select a Path"
            options: FolderDialog.ShowDirsOnly

            currentFolder: delegate_value

            onAccepted: {
                folderDialogText.text = folderDialog.selectedFolder;
                delegate_value = folderDialog.selectedFolder
            }
            onRejected: folderDialog.close()
        }

    }
}
