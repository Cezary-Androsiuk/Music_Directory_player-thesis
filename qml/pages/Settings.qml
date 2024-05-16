import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/Music_directory_player/qml/delegates"
import "qrc:/Music_directory_player/qml/popups"

Page {

    property var mdl: [
        {id: 100, name: "Theme", delegate_type: "switch", desc: ""},
        {id: 200, name: "Accent Color", delegate_type: "color", desc: ""},
        {id: 300, name: "Songs Path", delegate_type: "select path", desc: ""},
        {id: 400, name: "Show Tooltips", delegate_type: "switch", desc: ""},
        {id: 500, name: "Songs Extenstions", delegate_type: "string", desc: ""},
        {id: 700, name: "Load Protector", delegate_type: "integer", desc: ""},
        {id: 800, name: "Show Refresh List Button", delegate_type: "switch", desc: ""},
    ]
    property int delegateHeight: 60
    property int delegateWidth: width

    function closeSettingsView(){
        mainStackView.pop()
    }

    header: Item{
        id: listViewHeader
        anchors{
            left: parent.left
            top: parent.top
            right: parent.right
        }
        height: 40

        Item{
            id: refreshField
            anchors{
                top: parent.top
                left: parent.left
                bottom: parent.bottom
            }
            width: height*2
            FlatButton{
                dltDescription: "Go Back"
                dltImageIdle: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/left_arrow.png")
                dltImageHover: Qt.resolvedUrl("qrc:/Music_directory_player/assets/icons/left_arrow.png")
                onUserClicked: closeSettingsView()
            }
        }

        Text{
            text: "Settings"
            anchors.centerIn: parent
            font.pixelSize: 20
            color: root.color_accent1
        }
    }

    ListView{
        id: settingsListView
        anchors.fill: parent
        model: mdl
        boundsBehavior: Flickable.StopAtBounds
        clip: true
        delegate: Loader{
            width: delegateWidth
            height: delegateHeight

            sourceComponent: {
                if(false);
                else if(modelData.delegate_type === "switch")       switchComponent
                else if(modelData.delegate_type === "color")        colorComponent
                else if(modelData.delegate_type === "select path")  selectPathComponent
                else if(modelData.delegate_type === "string")       stringComponent
                else if(modelData.delegate_type === "integer")      integerComponent
                else{
                    console.log("#####################################################");
                    console.log("UNKNOWN DELEGATE TYPE!");
                    root.close();
                }
            }

            Component {
                id: switchComponent
                SwitchField{
                    dltText: modelData.name
                    dltDesc: modelData.desc
                    dltValue: {
                        if(false); // below lines are so beauty when equal <3
                        else if(modelData.id === 100) root.dark_theme
                        else if(modelData.id === 400) Backend.personalization.showTooltips
                        else if(modelData.id === 800) Backend.personalization.showRefreshListButton
                    }
                    onDltValueChanged: {
                        if(false); // below lines are so beauty when equal <3
                        else if(modelData.id === 100) Backend.personalization.isDarkTheme = dltValue
                        else if(modelData.id === 400) Backend.personalization.showTooltips = dltValue
                        else if(modelData.id === 800) Backend.personalization.showRefreshListButton = dltValue
                    }
                }
            }

            Component{
                id: colorComponent
                ColorSelectField{
                    dltText: modelData.name
                    dltDesc: modelData.desc
                    dltColor: {
                        if(modelData.id === 200)
                            root.color_accent2 // is dynamically changed in Main.qml
                    }
                    onColorChoosed: {
                        // reacting on delegate_color changed wasn't update anything when user changed theme
                        //      so I changes local vairalble "dltChoosedColor" and emit signal colorChoosed
                        //      and thats wokrs and when isDarkTheme (that responds for a color_accent2)
                        //      is changed accent2 color is also changed
                        if(modelData.id === 200){
                            if(root.dark_theme)
                                Backend.personalization.darkAccentColor = dltChoosedColor
                            else
                                Backend.personalization.lightAccentColor = dltChoosedColor
                        }
                    }
                }
            }

            Component{
                id: selectPathComponent
                PathSelectField{
                    dltText: modelData.name
                    dltDesc: modelData.desc
                    dltValue: {
                        if(modelData.id === 300)
                            Backend.personalization.rootDirectory
                    }
                    onDltValueChanged: {
                        if(modelData.id === 300)
                            Backend.personalization.rootDirectory = dltValue
                    }
                }
            }

            Component{
                id: stringComponent
                StringField{
                    dltText: modelData.name
                    dltDesc: modelData.desc
                    dltValue: {
                        if(modelData.id === 500)
                            Backend.personalization.songExtensions
                    }
                    onDltValueChanged: {
                        if(modelData.id === 500)
                            Backend.personalization.songExtensions = dltValue
                    }
                }
            }

            Component{
                id: integerComponent
                IntegerField{
                    dltText: modelData.name
                    dltDesc: modelData.desc
                    dltValue: {
                        if(false); // below lines are so beauty when equal <3
                        else if(modelData.id === 700) Backend.personalization.loadProtector

                    }
                    onDltValueChanged: {
                        if(false); // below lines are so beauty when equal <3
                        else if(modelData.id === 700) Backend.personalization.loadProtector = dltValue

                    }
                }
            }

        }
    }


}
