import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/Music_directory_player/qml/delegates"
import "qrc:/Music_directory_player/qml/popups"

Page {

    property var mdl: [
        {id: 100, name: "Theme", delegate_type: "switch"},
        {id: 200, name: "Accent Color", delegate_type: "color"},
        {id: 300, name: "Root Path", delegate_type: "select path"},
        {id: 400, name: "Show Tooltips", delegate_type: "switch"},
        {id: 500, name: "Songs Extenstions", delegate_type: "string"},
        {id: 600, name: "Song Transition Time", delegate_type: "integer"}
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

        // Text{
        //     text: "Settings"
        //     anchors.centerIn: parent
        //     font.pixelSize: 20
        //     color: root.color_accent1
        // }
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
                    delegate_text: {
                        modelData.name
                    }
                    delegate_value: {
                        if(false); // below lines are so beauty when equal <3
                        else if(modelData.id === 100) root.dark_theme
                        else if(modelData.id === 400) Backend.personalization.showTooltips
                    }
                    onDelegate_valueChanged: {
                        if(modelData.id === 100)
                        {
                            if(Backend.personalization.isDarkTheme !== delegate_value) // this condition removes "Binding loop" <3
                                Backend.personalization.isDarkTheme = delegate_value
                        }
                        else if(modelData.id === 400)
                        {
                            if(Backend.personalization.showTooltips !== delegate_value) // this condition removes "Binding loop" <3
                                Backend.personalization.showTooltips = delegate_value
                        }
                    }
                }
            }

            Component{
                id: colorComponent
                ColorSelectField{
                    delegate_text: modelData.name
                    delegate_color: {
                        if(modelData.id === 200)
                            root.color_accent2 // is dynamically changed in Main.qml
                    }
                    onColorChoosed: {
                        // reacting on delegate_color changed wasn't update anything when user changed theme
                        //      so I changes local vairalble "choosed_color" and emit signal colorChoosed
                        //      and thats wokrs and when isDarkTheme (that responds for a color_accent2)
                        //      is changed accent2 color is also changed
                        if(modelData.id === 200){
                            if(root.dark_theme)
                                Backend.personalization.darkAccentColor = choosed_color
                            else
                                Backend.personalization.lightAccentColor = choosed_color
                        }
                    }
                }
            }

            Component{
                id: selectPathComponent
                PathSelectField{
                    delegate_text: modelData.name
                    delegate_value: {
                        if(modelData.id === 300)
                            Backend.personalization.rootDirectory
                            // Backend.rootDirectory
                    }
                    onDelegate_valueChanged: {
                        if(modelData.id === 300)
                        {

                            if(Backend.personalization.rootDirectory !== delegate_value) // this condition removes "Binding loop" <3
                                Backend.personalization.rootDirectory = delegate_value
                        }
                    }
                }
            }

            Component{
                id: stringComponent
                StringField{
                    delegate_text: modelData.name
                    delegate_value: {
                        if(modelData.id === 500)
                            Backend.personalization.songExtensions
                    }
                    onDelegate_valueChanged: {

                    }
                }
            }

            Component{
                id: integerComponent
                IntegerField{
                    delegate_text: modelData.name
                    delegate_value: {
                        if(modelData.id === 600)
                            Backend.personalization.songTransitionTimeMS
                    }
                    onDelegate_valueChanged: {

                    }
                }
            }

        }
    }


}
