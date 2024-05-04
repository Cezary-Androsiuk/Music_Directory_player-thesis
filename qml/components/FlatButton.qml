import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import Qt5Compat.GraphicalEffects

Item{
    id: deleteButton
    anchors.fill: parent

    required property string dltDescription
    required property string dltImageIdle
    required property string dltImageHover

    // bellow is required for refresh while changing theme
    property color dltIdleColor: root.color_element_idle
    property color dltHoverColor: root.color_element_hover
    property color dltPressColor: root.color_element_press

    // onDltIdleColorChanged: {
    //     console.log("color idle changed" + this)
    // }
    // onDltHoverColorChanged: {
    //     console.log("color hover changed" + this)
    // }
    // onDltPressColorChanged: {
    //     console.log("color press changed" + this)
    // }

    property bool msDown: false

    signal userClicked()

    Image{
        id: img
        fillMode: Image.PreserveAspectFit
        anchors{
            fill: parent
            margins: {
                var pw = parent.width * 0.20
                var ph = parent.height * 0.20
                if(pw < ph)
                    pw
                else
                    ph
            }
        }

        source: dltImageIdle
    }

    ColorOverlay {
        id: colorOverlay
        anchors.fill: img
        source: img
        color: dltIdleColor
    }

    MouseArea{
        id: msArea
        anchors.fill: parent
        hoverEnabled: true

        onEntered: {
            img.source = dltImageHover;
            colorOverlay.color = dltHoverColor
            msDown = msArea.containsMouse
        }
        onExited: {
            img.source = dltImageIdle;
            colorOverlay.color = dltIdleColor
            msDown = false
        }
        onPressed: {
            colorOverlay.color = dltPressColor
            msDown = true
        }
        onReleased: {
            colorOverlay.color = msArea.containsMouse ? dltHoverColor : dltIdleColor
            if(msDown === true)
                userClicked()
        }

        ToolTip{
            visible: msArea.containsMouse && Backend.personalization.showTooltips
            text: dltDescription
            delay: 1200

        }
    }

}
