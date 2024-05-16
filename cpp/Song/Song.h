#ifndef SONG_H
#define SONG_H

#include <QObject>
#include <QString>

#include <cpp/DebugPrint.h>


class Song : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int id           READ getID              CONSTANT FINAL)
    Q_PROPERTY(int listIndex    READ getListIndex       NOTIFY listIndexChanged FINAL)
    Q_PROPERTY(QString title    READ getTitle           CONSTANT FINAL)
public:
    explicit Song(QObject *parent = nullptr);
    ~Song();

    int getID() const;
    int getListIndex() const;
    QString getTitle() const;

    void setID(int ID);
    void setListIndex(int listIndex);
    void setTitle(const QString &title);

signals:
    void listIndexChanged();
private:
    int m_ID;
    int m_listIndex;
    QString m_title;
};

typedef QList<Song *> SongList;

#endif // SONG_H
