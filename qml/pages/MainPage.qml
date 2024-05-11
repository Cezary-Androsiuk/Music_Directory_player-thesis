import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/Music_directory_player/qml/components"
import "qrc:/Music_directory_player/qml/delegates"

Page {

    property int delegateHeight: 60
    property int delegateWidth: width

    property int itemIndex: 1
    property int itemCount: Backend.songs.length

    header: MainPageHeader{}

    ScrollView{
        id: scrollView
        anchors.fill: parent
        visible: Backend.personalization.rootDirectory !== ""
        ListView{
            model: Backend.songs
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            delegate: Item{
                width: delegateWidth - 15 /*scrollbar offset*/
                height: delegateHeight

                ListButtonField{
                    delegate_text: modelData.title
                    onUserClickedElement: {
                        // just_used_id = + modelData.id;
                        // just_used_title = modelData.title;

                        // backend.database.loadEditPlaylistSongModel(+modelData.id)
                    }
                    onUserClickedPlay: {
                        // console.log("play: " + modelData.title)
                    }
                }
            }
        }
    }

    ScrollCurrentSongMarker{
        dltDisplay: scrollView.contentHeight > scrollView.height
    }

    footer: MainPageFooter{}
}
