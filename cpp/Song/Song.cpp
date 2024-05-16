#include "Song.h"

Song::Song(QObject *parent)
    : QObject{parent},
    m_title(""),
    m_listIndex(-1),
    m_ID(0)
{

    LFL("Song instance                 %1 created");
}

Song::~Song()
{
    LFL("Song instance                 %1 destroyed");
}

int Song::getID() const
{
    return m_ID;
}

int Song::getListIndex() const
{
    return m_listIndex;
}

QString Song::getTitle() const
{
    return m_title;
}

void Song::setID(int ID)
{
    m_ID = ID;
}

void Song::setListIndex(int listIndex)
{
    m_listIndex = listIndex;
    emit this->listIndexChanged();
}

void Song::setTitle(const QString &title)
{
    m_title = title;
}
