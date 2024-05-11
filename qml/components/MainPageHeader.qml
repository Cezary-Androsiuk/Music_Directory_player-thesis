import QtQuick 2.15

import "qrc:/Music_directory_player/qml/delegates"

Item{
    id: pageHeader
    height: 40


    function refreshClicked(){

    }

    function shuffleClicked(){

    }

    function settingsClicked(){
        mainStackView.push(Qt.resolvedUrl("qrc:/Music_directory_player/qml/pages/Settings.qml"))
    }


    Item{
        id: refreshField
        anchors{
            top: parent.top
            left: parent.left
            bottom: parent.bottom
        }
        width: height

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
            left: refreshField.right
            right: shuffleField.left
        }

        DirComboBox{
            dltRootDirectory: {
                // allways will contain "file://" prefix
                var path = "" + Backend.personalization.rootDirectory;
                path = path.replace("file:///", "");
                path;

            }

            dltModel: Backend.directoryStructure
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
