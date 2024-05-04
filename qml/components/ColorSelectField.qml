import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs

Item{
    id: colorSelectField
    anchors.fill: parent

    property string delegate_text: ""
    property var delegate_color: null

    signal colorChoosed()
    property var choosed_color: null
    // reacting on delegate_color changed wasn't update when user changed theme
    //      so I changes local vairalble "choosed_color" and emit signal colorChoosed
    //      and thats wokrs and when app_theme (that responds for a color_accent2)
    //      is changed accent2 color is also changed

    Text{
        id: textElement
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
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        width: parent.width * 0.25

        Rectangle{
            id: colorPicker
            color: "transparent"
            anchors{
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 32
            }
            width: 54
            height: colorSelectField.height * 3/5

            border.color: {
                delegate_color
            }
            border.width: 2

            Rectangle{
                id: colorArea
                color: parent.border.color
                anchors{
                    margins: parent.border.width
                    top: parent.top
                    bottom: colorIDText.top
                    left: parent.left
                    right: parent.right
                }
                height: parent.height * 3/6
            }
            Text{
                id: colorIDText
                color: parent.border.color
                font.pixelSize: 10
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                    bottomMargin: parent.border.width
                }
                text: parent.border.color
            }
            MouseArea{
                anchors.fill: parent
                onClicked: colorDialog.open()
            }
        }
        ColorDialog {
            id: colorDialog
            title: "Please choose a color"
            onAccepted: {
                // delegate_color = colorDialog.selectedColor
                choosed_color = colorDialog.selectedColor
                colorChoosed()
            }
            onRejected: {
                colorDialog.close()
            }
        }
    }
}
