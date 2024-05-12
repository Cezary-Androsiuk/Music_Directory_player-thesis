#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

class Playlist : public QObject
{
    Q_OBJECT
public:
    explicit Playlist(QObject *parent = nullptr);

signals:
};

#endif // PLAYLIST_H
