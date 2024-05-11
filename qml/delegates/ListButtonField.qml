import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt5Compat.GraphicalEffects

Item{
    id: listButtonField
    anchors.fill: parent

    property string delegate_text: ""

    signal userClickedElement()
    signal userClickedPlay()

    TabButton{
        id: elementButton
        anchors{
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: {
                playSongButton.left
            }
        }

        ToolTip{
            visible: elementButton.hovered && (text.contentWidth > text.width)
            text: delegate_text
            delay: 400
        }


        onClicked: userClickedElement()

        Text{
            id: text
            anchors{
                verticalCenter: parent.verticalCenter
                left: parent.left
                leftMargin: 20
                right: parent.right
                rightMargin: 10
            }
            text: delegate_text
            font.pixelSize: 15
            color: root.color_accent1
            verticalAlignment: Text.AlignVCenter

            // elide: Text.ElideRight
            clip: true
        }
    }

    TabButton{
        id: playSongButton
        height: parent.height
        width: height

        anchors{
            top: parent.top
            right: parent.right
        }

        text: "▶" //(index !== 2 ? "▶" : "")
        font.pixelSize: 30
        onClicked: userClickedPlay()
    }
}
