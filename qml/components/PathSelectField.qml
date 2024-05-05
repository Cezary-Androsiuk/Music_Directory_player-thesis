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
                dltDescription: "Select File\nCurrent File: " + delegate_value
                dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/folder.png")
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

            currentFolder: {
                if(delegate_value === "")
                {
                    if(Qt.platform.os === "windows")
                        "file:///C:/"
                    else if(Qt.platform.os === "linux")
                        "file:///"
                }
                else
                    "file:///" + delegate_value
            }


            onAccepted: {
                var path = "" + folderDialog.selectedFolder;
                delegate_value = path.replace("file:///", "")

            }
            onRejected: folderDialog.close()
        }

    }
}
