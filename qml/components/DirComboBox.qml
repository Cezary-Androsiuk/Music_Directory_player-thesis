import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs

Item {
    id: dirComboBox
    anchors.fill: parent

    property var dltModel: [
        {path: "example", depth: 0},
        {path: "data", depth: 1},
        {path: "in", depth: 2},
        {path: "list", depth: 3}
    ]
    required property string dltRootDirectory
    property int dltDepthIndent: 20

    readonly property int dltListElementHeight: selectedValueField.height * 0.8;
    property int dltPixelSize: 15

    Component.onCompleted: {
        if(dltRootDirectory === "")
            dltRootDirectory = "---"

        var popup_height = dltListElementHeight; // add for dltRootDirectory
        for(var _element of dltModel)
            popup_height += dltListElementHeight;
        if(popup_height === 0)
            popup_height = 10;

        var max_height = dltListElementHeight * 7.8; // max 3.8 buttons will be shown
        if(popup_height > max_height)
            popup_height = max_height;

        modelList.height = popup_height;

        listLoader.active = true;
    }

    Popup{
        id: modelList
        // height set in parent's onCompleted
        width: selectedValueField.width
        x: selectedValueField.x
        y: selectedValueField.y

        property int delegateWidth: width
        padding: 4
        focus: true
        Loader{
            id: listLoader
            anchors.centerIn: parent
            width: parent.width
            height: parent.height
            sourceComponent: listComponent
            active: false
        }

        Component{
            id: listComponent
            ListView{
                anchors.fill: parent
                model: dltModel
                clip: true
                boundsBehavior: Flickable.StopAtBounds
                // opacity: 0.5

                header: Item{
                    width: modelList.width
                    height: dltListElementHeight

                    TabButton{
                        id: button
                        anchors.fill: parent
                        opacity: 0.4

                        onClicked: {
                            modelList.close()
                        }

                    }
                    Text{
                        anchors.fill: button

                        text: dltRootDirectory

                        color: button.pressed ? root.color_element_press : root.color_element_idle
                        font.pixelSize: dltPixelSize
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft

                    }
                }

                delegate: Item{
                    width: modelList.width
                    height: dltListElementHeight

                    Item{
                        id: checkbox
                        anchors{
                            left: parent.left
                            top: parent.top
                            bottom: parent.bottom
                        }
                        width: height
                        // DirCheckBox{

                        // }
                        CheckBox{
                            anchors.fill: parent
                        }
                    }


                    Text{
                        anchors{
                            top: parent.top
                            left: checkbox.right
                            right: parent.right
                            bottom: parent.bottom
                            leftMargin: {
                                var space = 10
                                for(var i=0; i<modelData.depth; i++)
                                    space += dltDepthIndent
                                space

                            }

                            rightMargin: 10
                        }

                        text: modelData.path

                        color: root.color_element_idle
                        font.pixelSize: dltPixelSize
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignLeft

                    }

                    // ToolTip{
                    //     visible: parent.hovered && modelList.opened
                    //     text: dltModel[index].text
                    //     delay: 500
                    // }

                }
            }
        }
    }

    TabButton{
        id: selectedValueField
        anchors{
            fill: parent
            // margins: 5
        }
        onClicked: {
            modelList.open()
        }
        opacity: 0.4

    }
    Text{
        anchors{
            fill: parent
            leftMargin: 10
            rightMargin: 10
        }

        text: dltRootDirectory

        color: selectedValueField.pressed ? root.color_element_press : root.color_element_idle
        font.pixelSize: dltPixelSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft

    }
}

