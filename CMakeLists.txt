cmake_minimum_required(VERSION 3.16)

project(Music_directory_player VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.4 REQUIRED COMPONENTS Quick)

qt_standard_project_setup()

qt_add_executable(appMusic_directory_player
    main.cpp
)
# test
qt_add_qml_module(appMusic_directory_player
    URI Music_directory_player
    VERSION 1.0

    QML_FILES qml/Main.qml
    QML_FILES qml/pages/MainPage.qml
    QML_FILES qml/components/DirComboBox.qml
    SOURCES cpp/Backend/Backend.h cpp/Backend/Backend.cpp
    SOURCES cpp/Song/Song.h cpp/Song/Song.cpp
    SOURCES cpp/DebugPrint.h
    SOURCES cpp/SongList/SongList.h cpp/SongList/SongList.cpp
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
    PRIVATE Qt6::Quick
)

include(GNUInstallDirs)
install(TARGETS appMusic_directory_player
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
