import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/Music_directory_player/qml/components"
import "qrc:/Music_directory_player/qml/popups"

Item{
    id: pageHeader
    height: 40

    property var rootDirectory: Backend.personalization.rootDirectory
    property string localFileRootDirectory: {
        var path = "" + rootDirectory;
        path = path.replace("file:///", "");
        if(path.charAt(path.length - 1) !== '/')
            path += "/";
        path;
    }
    property bool showRefreshListButton: Backend.personalization.showRefreshListButton

    function refreshClicked(){
        p_confirmRefresh.open();
    }

    function shuffleClicked(){
        Backend.playlist.shufflePlaylistSongs();
    }

    function settingsClicked(){
        mainStackView.push(Qt.resolvedUrl("qrc:/Music_directory_player/qml/pages/Settings.qml"));
    }

    Popup2{
        id: p_confirmRefresh
        dltText: "Are you sure to reload songs? This will stop song and reset player."
        dltTextLB: "Cancel"
        dltTextRB: "Reload"

        onDltClickedRB: Backend.loadSongs()
    }

    Connections{
        target: Backend
        function onLoadingSongsStarted(){
            refreshField.enabled = false;
            shuffleField.enabled = false;
        }
        function onLoadingSongsFinished(){
            refreshField.enabled = true;
            shuffleField.enabled = true;
        }
    }

    Item{
        id: refreshField
        anchors{
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: height
        // enabled will be controlled by connections (if loading in progress)
        visible: showRefreshListButton

        ImageButton{
            dltDescription: "Refresh List"
            dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/refresh.png")
            dltImageHover: dltImageIdle
            onUserClicked: refreshClicked()
        }
    }

    Item{
        id: structureField
        anchors{
            top: parent.top
            bottom: parent.bottom
            left: {
                if(showRefreshListButton)
                    refreshField.right
                else
                    parent.left
            }
            right: shuffleField.left
            leftMargin: 10
            rightMargin: 10
        }
        clip: true

        Text{
            id: rootDirectoryText
            width: parent.width
            height: parent.height
            x: {
                // following code makes that text will stop at right site
                // and start disappearing on the left side
                var offset = contentWidth - width + 5;
                if(offset < 0) offset = 0;
                -offset;
            }
            y: 0

            text: localFileRootDirectory

            color: root.color_accent1
            font.pixelSize: 14
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft

            Rectangle{
                id: endTextMask
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                }
                width: parent.width * 0.1
                visible: (rootDirectoryText.contentWidth >= rootDirectoryText.width)

                gradient: Gradient{
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: root.color_background }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }
        }
        MouseArea{
            anchors.fill: parent
            hoverEnabled: true

            ToolTip{
                visible: parent.containsMouse &&
                         (rootDirectoryText.contentWidth >= rootDirectoryText.width)
                text: localFileRootDirectory
                delay: 1200
            }
        }
    }

    Item{
        id: shuffleField
        anchors{
            top: parent.top
            bottom: parent.bottom
            right: optionsField.left
        }
        width: height
        // enabled will be controlled by connections (if loading in progress)
        ImageButton{
            dltDescription: "Shuffle Songs"
            dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/shuffle.png")
            dltImageHover: dltImageIdle
            onUserClicked: shuffleClicked()
        }
    }

    Item{
        id: optionsField
        anchors{
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        width: height //* 2
        ImageButton{
            dltDescription: "Settings"
            dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/settings.png")
            dltImageHover: dltImageIdle
            onUserClicked: settingsClicked()
        }
    }

    ThinSeparator{ anchors.bottom: parent.bottom }
}
