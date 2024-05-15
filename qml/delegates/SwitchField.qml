import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Item{
    id: switchField
    anchors.fill: parent

    property string dltText: ""
    property bool dltValue: false
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
            enabled: dltEnabled
            checked: dltValue

            onCheckedChanged: {
                dltValue = checked
            }
        }

    }
}