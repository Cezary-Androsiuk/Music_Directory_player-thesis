import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Popup {
    id: popup1
    property string textMessage: "no message has been set"
    property string textDescription: ""

    property string textMB: "Ok" // middle button text

    property int fontSizeMB: 17 // middle button font size

    property bool jea: true // just exit action

    signal clickedMB() // clicked middle button

    property bool noDesc: (textDescription === "")

    height: noDesc ? 160 : 240
    width: 310

    x: root._w/2 - width/2
    y: root._h * 1/3 - height/2

    dim: true
    modal: true
    closePolicy: jea ?
                     Popup.CloseOnEscape | Popup.CloseOnPressOutside :
                     NoAutoClose

    Item{
        Component.onCompleted: {
            popup1.focus = true
        }
        Keys.onEscapePressed: {
            if(jea)
                popup1.close()
            else
                root.close()
        }
    }

    Item{
        anchors{
            fill: parent
            topMargin: noDesc ? 0 : -10
            bottomMargin: noDesc ? 0 : -10
        }

        Item{
            id: areaTexts
            anchors{
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: parent.height * 3/5

            Item{
                id: areaMessage
                anchors{
                    top: parent.top
                    left: parent.left
                    right: parent.right
                }
                height: noDesc ? parent.height : parent.height * 3/6

                Text{
                    id: message
                    anchors.centerIn: parent
                    text: textMessage
                    clip: true
                    font.pixelSize: 14
                    color: root.color_accent1
                    width: popup1.width - 50
                    wrapMode: Text.Wrap
                    horizontalAlignment: Text.AlignHCenter
                }
            }
            Item{
                id: areaDescription
                anchors{
                    top: areaMessage.bottom
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                visible: !noDesc

                Rectangle{
                    anchors.fill: parent
                    color: "white"
                    opacity: 0.05
                }

                Text{
                    anchors{
                        top: parent.top
                        left: parent.left
                        right: parent.right
                    }
                    text: "Description"
                    color: root.color_accent1
                    font.pixelSize: 10
                    horizontalAlignment: Text.AlignHCenter
                }

                Text{
                    anchors{
                        fill: parent
                        leftMargin: 10
                        rightMargin: 10
                        topMargin: 15
                    }

                    text: textDescription
                    color: root.color_accent1
                    wrapMode: Text.Wrap
                    clip: true
                    font.pixelSize: 9
                }
            }

        }

        Item{
            id: areaButtons
            anchors{
                top: areaTexts.bottom
                left: parent.left
                right: parent.right
                bottom: parent.bottom
            }


            Item{
                id: areaMB
                anchors.fill: parent

                Rectangle{
                    anchors.fill: buttonL

                    color: "transparent"
                    border.color: root.color_accent1
                    border.width: 1
                    opacity: 0.4
                }
                TabButton{
                    id: buttonL
                    anchors.centerIn: parent

                    text: textMB
                    font.pixelSize: fontSizeMB
                    onClicked: {
                        popup1.close();
                        clickedMB();
                    }

                    width: 100
                }
            }
        }

    }



}
