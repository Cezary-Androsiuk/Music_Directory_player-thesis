import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

Popup {
    id: popupLoading
    property string textMessage: "no message has been set"
    property string textProgress: "" // "13/21"
    property int textFontSize: 12

    property string textMB: "Cancel" // middle button text

    property int fontSizeMB: 17 // middle button font size

    signal clickedMB() // clicked middle button

    height: 160
    width: 310

    x: root._w/2 - width/2
    y: (root._h/2 - height/4) - height/2

    dim: true
    modal: true
    closePolicy: Popup.NoAutoClose

    Item{
        anchors.fill: parent

        // Item{
        //     id: areaTexts
        //     anchors{
        //         top: parent.top
        //         left: parent.left
        //         right: parent.right
        //     }
        //     height: parent.height * 3/5

        //     Item{
        //         id: areaMessage
        //         anchors{
        //             top: parent.top
        //             left: parent.left
        //             right: parent.right
        //         }
        //         height: parent.height

        //         Text{
        //             id: message
        //             anchors.centerIn: parent
        //             text: textMessage
        //             clip: true
        //             font.pixelSize: 14
        //             color: root.color_accent1
        //             width: popupLoading.width - 50
        //             wrapMode: Text.Wrap
        //             horizontalAlignment: Text.AlignHCenter
        //         }
        //     }
        // }

        Item{
            id: areaBusyIndicator
            anchors{
                top: parent.top
                left: parent.left
                right: parent.right
            }
            height: parent.height * 3/5

            BusyIndicator{
                id: bi
                anchors{
                    horizontalCenter: parent.horizontalCenter
                    top: parent.top
                    topMargin: 10
                }

                width: 50
                height: 50
            }
            Text{
                anchors{
                    horizontalCenter: bi.horizontalCenter
                    top: bi.bottom
                }
                text: textMessage + textProgress
                color: root.color_accent1
                font.pixelSize: textFontSize
            }
        }

        Item{
            id: areaButtons
            anchors{
                top: areaBusyIndicator.bottom
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
                        popupLoading.close();
                        clickedMB();
                    }

                    width: 100
                }
            }
        }

    }



}
