import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/Music_directory_player/qml/popups"

ApplicationWindow {
    id: root
    property int _w: 400
    property int _h: 600
    minimumWidth: _w
    maximumWidth: _w
    minimumHeight: _h
    maximumHeight: _h

    visible: true
    title: qsTr("Music Directory Player")

    /*

      Zmieniłem aplikację bo aplikacja pierwotna na tyle się rozbudowała (+8k linii), że apis funkcjonalności również by przekroczył te przykładowe 40 stron pacy dyplomowej
      i mógłbym się nie wyrobić z tym wszystkim przed terminem. Skoro i tak nie mógłbym całej zaprezentować na obronie oraz żadnych benefitów dzięki temu bym nie miał, to
      poprostu było by to zbędne ryzyko.
      Ale chwilę... skoro miałeś już 2/3 gotowe, to gdy zaczniesz od nowa to będziesz musiał robić wszystko ponownie. Wtedy to się napewno nie wyrobisz.
      Rzecz w tym, że własnie nie! W pierwotnej wersji skupiłem się na koncepcie tagów i mimo, że miałem już 2/3 aplikacji to nie zacząłem tej właściwej części, którą jest
      sam odtwarzacz muzyki i playlista. Jeżeli teraz troszkę bym nadrobił posiadając wiedzę z pierwotnej aplikacji oraz gotowe komponenty jakie tam utworzyłem, to bym był
      w tym samym punkcie co w tamtej aplikacji.
      Ale to po co w ogóle nadrabiać skoro mówisz, że "trochę nadrobisz", możesz poprostu kontynuować już do końca?
      Ponieważ tak jak wspomnialem nawet nie był bym w stanie opisać czy omówić tej aplikacji, a na obronie również (o ile nie bardziej niż wielkość) jest istotna
      bezawaryjność aplikacji, a takową by było trudno utrzymać przy takej ilości fukcjonalności i pośpiechu. Mając mniejszą aplikacje mogę się bardziej skupić na
      bezbłędności i dodatkach, a opisywać i tak będzie co przy technologiach Qt, QML, JavaScript, C++ oraz JSON

    */

    Component.onCompleted: {
        console.log("qml initialized")
        Backend.initializeBackend()
        // this way allow to display popup when anything failed in loading backend
    }

    Connections{
        target: Backend
        function onBackendInitialized(){
            console.log("backend initialized")
            mainStackView.replace(Qt.resolvedUrl("pages/MainPage.qml"))//, {parentStackView: mainStackView}) // to pass property
        }

        function onPersonalizationLoadError(){
            p_personalizationLoadError.open()
        }
    }

    Popup3{
        id: p_personalizationLoadError
        textMessage: "Error while loading personalizations"
        textLB: "Retry"
        textMB: "Continue"
        textRB: "Exit"

        onClickedLB: Backend.reinitializePersonalization()
        onClickedMB: Backend.useDefaultPersonalization()
        onClickedRB: root.close()
    }

    Item{
        id: rootItem
        anchors.fill: parent
        focus: true
        Keys.onEscapePressed: root.close()
        Keys.onSpacePressed: console.log("-")
        Keys.onDeletePressed: p_personalizationLoadError.open()

        StackView{
            id: mainStackView
            anchors.fill: parent
            initialItem: Item{
                BusyIndicator{
                    id: bi
                    anchors.centerIn: parent
                    width: 100
                    height: 100
                }
                Text{
                    anchors{
                        horizontalCenter: bi.horizontalCenter
                        top: bi.bottom
                    }
                    text: "Loading view"
                    color: root.color_accent1
                    font.pixelSize: 15
                }
            }

            // on backend loaded to remove initial item
            replaceEnter: Transition {
                OpacityAnimator{
                    from: 0
                    to: 1
                    duration: animation_duration
                    easing.type: Easing.OutCubic
                }
            }
            replaceExit: Transition {
                OpacityAnimator{
                    from: 1
                    to: 0
                    duration: animation_duration
                    easing.type: Easing.OutCubic
                }
            }

            // on push new site (settings site)
            pushExit: Transition {
                OpacityAnimator{ // to hold bottom page while sliding in new one
                    from: 1
                    to: 1
                    duration: animation_duration
                }
            }
            pushEnter: Transition {
                XAnimator {
                    // idk what is "mirrored" here
                    from: (mainStackView.mirrored ? -1 : 1) * mainStackView.width
                    to: 0
                    duration: animation_duration
                    easing.type: Easing.OutCubic
                }
            }

            // on pop new site (settings site)
            popExit: Transition {
                XAnimator {
                    from: 0
                    to: (mainStackView.mirrored ? -1 : 1) * mainStackView.width
                    duration: animation_duration
                    easing.type: Easing.OutCubic
                }
            }
            popEnter: Transition {  }
        }
    }


    // ------------------------------------ Colors ------------------------------------

    function rgb(r, g, b, a=255){ return Qt.rgba(r/255, g/255, b/255, a/255); }

    property bool dark_theme: Backend.personalization.isDarkTheme

    property color _color_dark_accent1: rgb(255,255,255)
    property color _color_light_accent1: rgb(0,0,0)
    property color _color_dark_accent2: Backend.personalization.darkAccentColor
    property color _color_light_accent2: Backend.personalization.lightAccentColor

    property color color_accent1: dark_theme ? _color_dark_accent1 : _color_light_accent1
    property color color_accent2: dark_theme ? _color_dark_accent2 : _color_light_accent2
    property color color_background: dark_theme ? rgb(28,27,31) : rgb(255,251,254)
    property color color_element_idle: dark_theme ? rgb(200, 200, 200) : rgb(50, 50, 50)
    property color color_element_hover: dark_theme ? rgb(140, 140, 140) : rgb(110, 110, 110)
    property color color_element_press: color_accent2

    Material.theme: dark_theme ? Material.Dark : Material.Light
    Material.accent: color_accent2


    // ------------------------------------ Icons ------------------------------------


    // ------------------------------------ Personalization ------------------------------------

    property int animation_duration: 175
}
