#include "Song.h"

Song::Song(QObject *parent)
    : QObject{parent},
    m_title(""),
    m_ID(0)
{}

int Song::getID() const
{
    return m_ID;
}

QString Song::getTitle() const
{
    return m_title;
}

void Song::setID(int ID)
{
    m_ID = ID;
}

void Song::setTitle(const QString &title)
{
    m_title = title;
}
