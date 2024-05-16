#ifndef BACKEND_H
#define BACKEND_H

#include <QtCore>
#include <QObject>
#include <QString>
#include <QList>
#include <QUrl>
#include <QDir>
// #include <QFile>
// #include <Qfile
#include <QEventLoop>
#include <QTimer>
#include <QStandardPaths>
#include <QtMultimedia/QMediaPlayer> // load song data
#include <QtMultimedia/QMediaMetaData> // load song data

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>
#include <cpp/Personalization/Personalization.h>
#include <cpp/Playlist/Playlist.h>
#include <cpp/Player/Player.h>


class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(Personalization *personalization READ getPersonalization CONSTANT FINAL)
    Q_PROPERTY(Playlist *playlist               READ getPlaylist        CONSTANT FINAL)
    Q_PROPERTY(Player *player                   READ getPlayer          CONSTANT FINAL)

public:
    explicit Backend(QObject *parent = nullptr);
    ~Backend();

    /// initialize Backend
    void initializeParameters();
    void initializeConnections();

    /// destroy Backend
    // none

public slots: /// after qml was loaded (to check if steps are correctly initialized or display popup)
    void checkPersonalization();
    /// idea is that QML will structurally and with specyfic order call "check" and react to correct calling another one untill received backendInitialized()
signals: /// communicate with QML to check if initialized correctly (emited by above slots)
    void personalizationLoaded();                       /// emitted after confirmed that personalization has been initialized correctly
    void personalizationLoadError(int errorCode);       /// emitted after finding out that personalization has not been initialized correctly
    void backendInitialized();                          /// emitted after confirming that the last(currently personalization) of the steps(these above) has been correctly initialized

public slots: /// initialize actions
    void reinitializePersonalization();
    void useDefaultPersonalization();

public: /// getters / setters
    Personalization *getPersonalization() const;
    Playlist *getPlaylist() const;
    Player *getPlayer() const;

public slots: /// actions
    void loadSongs();           /// triggered by MainPage.onCompleted, rootDirectoryChanged and MainPageHeader.refreshSongs
    void cancelLoadingSongs();  /// trigered by MainPage.popup.cancel (button)

signals: /// something happend in action
    void loadingSongsStarted();                                                     /// emitted after loading songs started
    void loadingSongsInProgress(int songsLoaded, int filesLoaded, int filesTotal);  /// emitted while loading songs to inform about progress
    void loadingSongsFinished(SongList songs);                                      /// emitted after loading songs has been finished
    void loadingSongsError(QString desc);                                           /// emitted after error occur while loading songs (propably rootDirectory not exist)
    void loadingSongsProtected(int limit);                                          /// emitted after count of loaded files reached Personalization::getLoadProtector()

private: /// private methods

private:
    Personalization *m_personalization;
    Playlist *m_playlist;
    Player *m_player;

    bool m_cancelLoadingSong;
};

#endif // BACKEND_H
