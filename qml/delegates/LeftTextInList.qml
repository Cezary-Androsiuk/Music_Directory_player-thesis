import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Item {
    // can be used as extension for delegates where text is pinned to the left site and the longer ones can
    //     go out from the right site
    id: leftTextInList
    anchors{
        top: parent.top
        left: parent.left
        right: dltAnchorRight
        bottom: parent.bottom
    }

    property string dltText: ""
    property int dltPixelSize: 15
    required property var dltAnchorRight
    property int dltRightMargin: 10

    Item{
        anchors{
            leftMargin: 20
            left: parent.left
            top: parent.top
            bottom: parent.bottom
            right: parent.right
            rightMargin: dltRightMargin
        }
        clip: true
        Text{
            id: text
            width: parent.width
            height: parent.height
            x: 0
            y: 0

            text: dltText
            color: root.color_accent1
            font.pixelSize: 15
            verticalAlignment: Text.AlignVCenter
        }
        MouseArea{
            id: msArea
            anchors.fill: parent
            hoverEnabled:  true

            ToolTip{
                visible: parent.containsMouse && (text.contentWidth >= text.width)
                text: dltText
                delay: 1200
            }
        }
        Rectangle{
            id: endTextMask
            anchors{
                top: parent.top
                bottom: parent.bottom
                right: parent.right
            }
            width: parent.width * 0.1
            visible: (text.contentWidth >= text.width)

            gradient: Gradient{
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 1.0; color: root.color_background }
            }
        }

    }


}
