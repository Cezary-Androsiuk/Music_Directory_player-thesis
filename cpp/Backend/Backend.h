#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QString>
#include <QList>

#include <cpp/DebugPrint.h>
#include <cpp/Song/Song.h>


class Backend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QList<Song*> songs READ getSongs NOTIFY songsChanged FINAL)
public:
    explicit Backend(QObject *parent = nullptr);

    void loadConfiguration(); // started in constructor


public slots:
    void qmlInitialized(); // emited in Component.onConpleated
    // void qmlLoaded(); // propably not required cause will signal that qml and his list was loaded

    void loadSongs(); // started in qmlInitialized and emits songs changed

public:
    QList<Song*> getSongs() const;

signals:
    void songsChanged();

private:
    bool m_configurationLoaded;
    bool m_qmlInitialized;

    QList<Song*> m_songs;
};

#endif // BACKEND_H
