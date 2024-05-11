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

    property color dltIdleColor: root.color_element_idle
    property color dltHoverColor: root.color_element_hover
    property color dltPressColor: root.color_element_press

    property bool dltDarkThemeRefresh: root.dark_theme
    onDltDarkThemeRefreshChanged: {
        // force to refresh because can't find reason why this isn't
        //    allways refreshing after changing dark_theme state
        // i think that is caused by some interaction with containsMouse
        colorOverlay.color = dltIdleColor
    }

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
