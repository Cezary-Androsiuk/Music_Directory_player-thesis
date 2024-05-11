import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Item{
    id: stringField
    anchors.fill: parent

    property string delegate_text: ""
    property var delegate_value: null
    property bool delegate_enabled: true

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
            anchors.fill: parent
            font.pixelSize: 15
            clip: true

            enabled: delegate_enabled


            Component.onCompleted: {
                // init component
                // this will be destroyed after scroll
                text = delegate_value;
            }

            onEditingFinished: {
                delegate_value = text;
            }

            ToolTip.visible: hovered && (width < (contentWidth * 1.3))
            ToolTip.text: text
        }
    }
}
