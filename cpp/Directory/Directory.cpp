#include "Directory.h"

Directory::Directory(QObject *parent)
    : QObject{parent},
    m_path(""),
    m_depth(0),
    m_include(true)
{}

QString Directory::getPath() const
{
    return m_path;
}

int Directory::getDepth() const
{
    return m_depth;
}

bool Directory::getInclude() const
{
    return m_include;
}

void Directory::setPath(const QString &path)
{
    m_path = path;
}

void Directory::setDepth(const int &depth)
{
    m_depth = depth;
}

void Directory::setInclude(const bool &include)
{
    if(m_include == include)
        return;

    m_include = include;
    emit this->includeChanged();
}

