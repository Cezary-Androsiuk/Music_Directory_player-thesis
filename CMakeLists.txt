cmake_minimum_required(VERSION 3.16)

project(Music_directory_player VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick Multimedia)

qt_standard_project_setup()

qt_add_executable(appMusic_directory_player
    main.cpp
)

qt_add_qml_module(appMusic_directory_player
    URI Music_directory_player
    VERSION 1.0

    SOURCES cpp/Backend/Backend.h cpp/Backend/Backend.cpp
    SOURCES cpp/DebugPrint.h cpp/DebugPrint.cpp
    SOURCES cpp/Song/Song.h cpp/Song/Song.cpp
    SOURCES cpp/Personalization/Personalization.h cpp/Personalization/Personalization.cpp
    SOURCES cpp/Player/Player.h cpp/Player/Player.cpp
    SOURCES cpp/Playlist/Playlist.h cpp/Playlist/Playlist.cpp

    QML_FILES qml/Main.qml

    QML_FILES qml/pages/MainPage.qml
    QML_FILES qml/pages/Settings.qml

    QML_FILES qml/components/ThinSeparator.qml
    QML_FILES qml/components/ImageButton.qml
    QML_FILES qml/components/InfoToolTip.qml
    QML_FILES qml/components/BetterButton.qml
    QML_FILES qml/components/MainPage/Header.qml
    QML_FILES qml/components/MainPage/Footer.qml
    QML_FILES qml/components/MainPage/Player.qml
    QML_FILES qml/components/MainPage/ScrollCurrentSongMarker.qml
    # QML_FILES qml/components/MainPage/Player/
    # QML_FILES qml/components/Settings/

    QML_FILES qml/delegates/MainPage/SongField.qml
    QML_FILES qml/delegates/Settings/ColorSelectField.qml
    QML_FILES qml/delegates/Settings/IntegerField.qml
    QML_FILES qml/delegates/Settings/PathSelectField.qml
    QML_FILES qml/delegates/Settings/StringField.qml
    QML_FILES qml/delegates/Settings/SwitchField.qml
    QML_FILES qml/delegates/Settings/Common/LeftText.qml

    QML_FILES qml/popups/Popup1.qml
    QML_FILES qml/popups/Popup2.qml
    QML_FILES qml/popups/Popup3.qml
    QML_FILES qml/popups/PopupLoading.qml
    QML_FILES qml/popups/common/TextsArea.qml

    RESOURCES assets/icons/refresh.png
    RESOURCES assets/icons/settings.png
    RESOURCES assets/icons/shuffle.png
    RESOURCES assets/icons/left_arrow.png
    RESOURCES assets/icons/info.png
    # # I CAN' DECIDE WHICH IS BETTER
    # RESOURCES assets/icons/folder_ios.png
    RESOURCES assets/icons/opened_folder_ios.png
    # RESOURCES assets/icons/folder_windows.png
    # RESOURCES assets/icons/opened_folder_windows.png

    # RESOURCES assets/icons/player/end.png
    # RESOURCES assets/icons/player/pause.png
    # RESOURCES assets/icons/player/play.png
    # RESOURCES assets/icons/player/skip_to_start.png
    # RESOURCES assets/icons/player/stop.png
    # RESOURCES assets/icons/player/volume_0.png
    # RESOURCES assets/icons/player/volume_1.png
    # RESOURCES assets/icons/player/volume_2.png
    # RESOURCES assets/icons/player/volume_3.png

)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMusic_directory_player PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMusic_directory_player
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMusic_directory_player
    PRIVATE
    Qt6::Quick
    Qt6::Multimedia
)

include(GNUInstallDirs)
install(TARGETS appMusic_directory_player
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
