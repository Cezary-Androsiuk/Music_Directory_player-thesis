#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QString>

#include <cpp/DebugPrint.h>

class Song : public QObject
{
    Q_OBJECT
public:
    explicit Song(QObject *parent = nullptr);

signals:

private:
    QString m_title;
};

#endif // SONG_H
