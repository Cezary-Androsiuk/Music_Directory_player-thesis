#include "Song.h"

Song::Song(QObject *parent)
    : QObject{parent},
    m_title("")
{}

QString Song::getTitle() const
{
    return m_title;
}

void Song::setTitle(const QString &title)
{
    m_title = title;
}
