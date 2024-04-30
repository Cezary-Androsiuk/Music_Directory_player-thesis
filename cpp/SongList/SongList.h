#ifndef SONGLIST_H
#define SONGLIST_H

#include <QObject>

class SongList : public QObject
{
    Q_OBJECT
public:
    explicit SongList(QObject *parent = nullptr);

signals:
};

#endif // SONGLIST_H
