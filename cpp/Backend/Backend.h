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

    void setRootDirectory(QUrl rootDirectory);
    void setSongExtensions(QString songExtensions);

public slots: /// actions
    void loadSongs();           /// triggered by MainPage.onCompleted and rootDirectoryChanged
    void cancelLoadingSongs();  /// trigered by MainPage.popup.cancel (button)

signals: /// something happend in action
    void songsLoadError(QString desc);                                              /// emitted after error occur while loading songs (propably rootDirectory not exist)
    void loadProtectorLimited(int limit);                                           /// emitted after count of loaded files reached Personalization::getLoadProtector()
    void loadingSongsInProgress(int songsLoaded, int filesLoaded, int filesTotal);  /// emitted while loading songs to inform about progress
    void loadingSongsFinished();                                                    /// emitted after loading songs has been finished

signals: /// some variable changed
    void rootDirectoryChanged();        /// used to trigger loadSongs
    void songsChanged(SongList songs);  /// used to trigger Playlist::loadPlaylistSongs(SongList songs)

private: // private methods

private:
    Personalization *m_personalization;
    Playlist *m_playlist;
    Player *m_player;

    QUrl m_rootDirectory; // stores root path where all songs are located (also in subfolders)
    QString m_songExtensions;

    bool m_cancelLoadingSong;
};

#endif // BACKEND_H
