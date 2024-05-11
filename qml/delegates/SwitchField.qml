import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Item{
    id: switchField
    anchors.fill: parent

    property string delegate_text: ""
    property bool delegate_value: false
    property bool delegate_enabled: true

    Text{
        id: textElement
        anchors{
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 20
            // right: textRightElement.left
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

        Switch{
            anchors{
                verticalCenter: parent.verticalCenter
                right: parent.right
                rightMargin: 20
            }
            enabled: delegate_enabled
            checked: delegate_value

            onCheckedChanged: {
                delegate_value = checked
            }
        }

    }
}
