import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Dialogs

Item {
    id: dirComboBox
    anchors.fill: parent

    readonly property int dltListElementHeight: selectedValueField.height * 0.8;
    property int dltPopupHeight: 0
    property int dltDepthIndent: 20
    property int dltPixelSize: 15
    readonly property bool dltCheckboxVisible: true

    required property string dltRootDirectory
    onDltRootDirectoryChanged: {

    }

    property var dltModel: [
        {path: "example", depth: 0, include: true},
        {path: "data", depth: 1, include: false},
        {path: "in", depth: 2, include: true},
        {path: "list", depth: 3, include: true}
    ]
    onDltModelChanged: {
        updatePopupHeight()
    }

    Component.onCompleted: {
        updatePopupHeight()
        listLoader.active = true;
    }

    function updatePopupHeight(){
        var popup_height = selectedValueField.height; // add for header: dltRootDirectory name
        for(var _element of dltModel)
            popup_height += dltListElementHeight;

        var max_height = dltListElementHeight * 7.8; // max 3.8 buttons will be shown
        if(popup_height > max_height)
            popup_height = max_height;

        dltPopupHeight = popup_height
    }

    Popup{
        id: modelList
        width: selectedValueField.width
        height: dltPopupHeight
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
                        visible: dltCheckboxVisible
                        CheckBox{
                            anchors.fill: parent
                            checked: modelData.include
                            onCheckedChanged: {
                                dltModel[index].include = checked
                            }
                        }
                    }


                    Text{
                        anchors{
                            top: parent.top
                            left: {
                                if(dltCheckboxVisible)
                                    checkbox.right
                                else
                                    parent.left
                            }

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
        enabled: dltRootDirectory !== ""
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

        text: dltRootDirectory === "" ? "---" : dltRootDirectory

        color: selectedValueField.pressed ? root.color_element_press : root.color_element_idle
        font.pixelSize: dltPixelSize
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignLeft

    }
}

