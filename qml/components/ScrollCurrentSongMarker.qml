import QtQuick 2.15

Item{
    id: currentSongMarker
    required property bool dltDisplay

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
            if(itemCount === 0) 0
            else ((itemIndex+0)/itemCount) * parent.height
        }
        visible: {
            if(itemCount === 0) false
            else true
        }

        color: root.color_accent1
        opacity: 0.4
    }
}
