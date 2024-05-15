import QtQuick 2.15

Item{
    id: currentSongMarker
    required property bool dltDisplay
    property int dltSongIndex: 0
    property int dltSongsCount: 0


    anchors{
        top: parent.top
        bottom: parent.bottom
        right: parent.right
    }
    width: 8

    visible: dltDisplay

    Rectangle{
        anchors{
            right: parent.right
            left: parent.left
        }
        height: width/2
        y: {
            if(dltSongsCount === 0) 0
            else ((dltSongIndex+0)/dltSongsCount) * parent.height
        }
        visible: {
            if(dltSongsCount === 0) false
            else true
        }

        color: root.color_accent1
        opacity: 0.4
    }
}
