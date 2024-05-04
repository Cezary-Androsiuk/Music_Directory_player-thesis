import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    id: leftTextInList
    anchors{
        top: parent.top
        left: parent.left
        right: delegate_anchor_right
        bottom: parent.bottom
    }

    property string delegate_text: ""
    property var delegate_anchor_right: null
    property int delegate_right_margin: 10

    Text{
        anchors{
            verticalCenter: parent.verticalCenter
            left: parent.left
            leftMargin: 20
            right: parent.right
            rightMargin: delegate_right_margin
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

            ToolTip{
                visible: containsMouse
                text: delegate_text
                delay: 400
            }
        }
    }
}
