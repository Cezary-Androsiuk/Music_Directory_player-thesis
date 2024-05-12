import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Item{
    id: integerField
    anchors.fill: parent

    property string dltText: ""
    property var dltValue: null
    property bool dltEnabled: true

    LeftTextInList{
        id: textElement
        dltAnchorRight: textRightElement.left
        dltRightMargin: 10
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
        width: parent.width * 0.4

        TextField{
            anchors.fill: parent
            font.pixelSize: 15
            clip: true

            enabled: dltEnabled

            validator: IntValidator{}

            Component.onCompleted: {
                // init component
                // this will be destroyed after scroll
                text = dltValue;
            }

            onEditingFinished: {
                dltValue = text;
            }

            ToolTip.visible: hovered && (width < (contentWidth * 1.3))
            ToolTip.text: text
        }
    }
}

