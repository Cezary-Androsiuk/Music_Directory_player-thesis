import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/Music_directory_player/qml/delegates"

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
        Backend.rootDirectoryChanged()
    }

    function shuffleClicked(){
        Backend.playlist.shufflePlaylistSongs();
    }

    function settingsClicked(){
        mainStackView.push(Qt.resolvedUrl("qrc:/Music_directory_player/qml/pages/Settings.qml"));
    }


    Item{
        id: refreshField
        anchors{
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: height

        visible: showRefreshListButton

        FlatButton{
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
        }

        Item{
            anchors{
                fill: parent
                leftMargin: 10
                rightMargin: 10
            }
            clip: true

            Text{
                id: __text
                width: parent.width
                height: parent.height
                x: {
                    var offset = contentWidth - width + 5;
                    if(offset < 0) offset = 0;
                    -offset;
                }
                y: 0

                text: localFileRootDirectory

                color: root.color_element_idle
                font.pixelSize: 14
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignLeft
            }
            MouseArea{
                anchors.fill: parent
                hoverEnabled: true

                ToolTip{
                    visible: parent.containsMouse && (__text.contentWidth >= __text.width)
                    text: localFileRootDirectory
                    delay: 1200
                }
            }
            Rectangle{
                id: endTextMask
                anchors{
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                }
                width: parent.width * 0.1
                visible: (__text.contentWidth >= __text.width)

                gradient: Gradient{
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: root.color_background }
                    GradientStop { position: 1.0; color: "transparent" }
                }
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

        FlatButton{
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
        FlatButton{
            dltDescription: "Settings"
            dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/settings.png")
            dltImageHover: dltImageIdle
            onUserClicked: settingsClicked()
        }
    }

    LongSeparator{ anchors.bottom: parent.bottom }
}
