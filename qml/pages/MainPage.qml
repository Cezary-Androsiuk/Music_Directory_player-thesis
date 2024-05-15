import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/Music_directory_player/qml/components"
import "qrc:/Music_directory_player/qml/delegates"
import "qrc:/Music_directory_player/qml/popups"

Page {

    property int delegateHeight: 60
    property int delegateWidth: width

    // property int currentSongIndex: {
    //     if(Backend.player.currentSong !== null)
    //         Backend.player.currentSong.id
    //     else
    //         -1
    // }
    // property int nextSongIndex: {
    //     if(Backend.player.nextSong !== null)
    //         Backend.player.nextSong.id
    //     else
    //         -1
    // }
    // property int songsCount: Backend.playlist.songs.length

    property int currentSongIndex: Backend.player.currentSong.listIndex
    property int nextSongIndex: Backend.player.nextSong.listIndex

    Connections{
        target: Backend.player
        function onCurrentSongChanged(){
            console.log("current song changed -> " + currentSongIndex)
        }
        function onNextSongChanged(){
            console.log("next song changed -> " + nextSongIndex)
        }

    }
    Item{
        Component.onCompleted: {
            // on start load all songs
            Backend.loadSongs()
        }
    }

    Connections{
        target: Backend
        function onLoadingSongsInProgress(songsLoaded, filesLoaded, filesTotal){
            scrollView.visible = false
            p_loadingSong.textMessage =
                    " songs found: " + songsLoaded + ", files loaded: " + filesLoaded + "/" + filesTotal
            if(!p_loadingSong.opened)
                p_loadingSong.open()
        }

        function onLoadingSongsFinished(){
            scrollView.visible = true
            p_loadingSong.close()
        }

        function onSongsLoadError(desc){
            p_songsLoadError.open()
            p_songsLoadError.textDescription = desc
        }

        function onLoadProtectorLimited(limit){
            p_protectorLimited.open()
            p_protectorLimited.textMessage = "Loading of songs stopped because "+
                    limit+" files were checked and the limit was reached (Load Protector)"
        }
    }

    PopupLoading{
        id: p_loadingSong
        textMessage: "loading songs"
        onClickedMB: Backend.cancelLoadingSongs()
    }

    Popup2{
        id: p_songsLoadError
        textMessage: "Can't load Songs!"
        textLB: "Retry"
        textRB: "Ok"
        jea: true

        onClickedLB: Backend.loadSongs()
    }

    Popup1{
        id: p_protectorLimited
        textMB: "Ok"
        jea: true
    }

    header: MainPageHeader{}

    ScrollView{
        id: scrollView
        anchors.fill: parent
        visible: Backend.personalization.rootDirectory !== ""

        ListView{
            model: Backend.playlist.songs
            boundsBehavior: Flickable.StopAtBounds
            clip: true

            delegate: Loader{
                width: delegateWidth - 15 /*scrollbar offset*/
                height: delegateHeight

                sourceComponent: {
                    if(modelData.listIndex === currentSongIndex)    currentSongComponent
                    else if(modelData.listIndex === nextSongIndex)     nextSongComponent
                    else                                                   songComponent
                }

                Component{
                    id: currentSongComponent // highlight
                    Item{
                        ListButtonField{
                            delegate_text: modelData.title
                            onUserClickedElement: {
                                // just_used_id = + modelData.id;
                                // just_used_title = modelData.title;
                                console.log(modelData.id + " " + modelData.listIndex + " " + modelData.title)
                                // backend.database.loadEditPlaylistSongModel(+modelData.id)
                            }
                            onUserClickedPlay: {
                                // console.log("play: " + modelData.title)
                            }
                        }
                        Rectangle{
                            anchors.fill: parent
                            color: root.color_accent2
                            opacity: 0.7
                        }
                    }
                }

                Component{
                    id: nextSongComponent // highlight
                    Item{
                        ListButtonField{
                            delegate_text: modelData.title
                            onUserClickedElement: {
                                // just_used_id = + modelData.id;
                                // just_used_title = modelData.title;
                                console.log(modelData.id + " " + modelData.listIndex + " " + modelData.title)
                                // backend.database.loadEditPlaylistSongModel(+modelData.id)
                            }
                            onUserClickedPlay: {
                                // console.log("play: " + modelData.title)
                            }
                        }
                        Rectangle{
                            anchors.fill: parent
                            color: root.color_accent2
                            opacity: 0.3
                        }
                    }
                }

                Component{
                    id: songComponent
                    Item{
                        ListButtonField{
                            delegate_text: modelData.title
                            onUserClickedElement: {
                                // just_used_id = + modelData.id;
                                // just_used_title = modelData.title;
                                console.log(modelData.id + " " + modelData.listIndex + " " + modelData.title)
                                // backend.database.loadEditPlaylistSongModel(+modelData.id)
                            }
                            onUserClickedPlay: {
                                // console.log("play: " + modelData.title)
                            }
                        }
                    }
                }

            }






        }
    }

    ScrollCurrentSongMarker{
        dltSongIndex: currentSongIndex
        dltSongsCount: Backend.playlist.songs.length
        dltVisible: scrollView.contentHeight > scrollView.height
    }

    footer: MainPageFooter{}
}
