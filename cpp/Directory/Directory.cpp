#include "Directory.h"

Directory::Directory(QObject *parent)
    : QObject{parent},
    m_path(""),
    m_depth(0)
{}

QString Directory::getPath() const
{
    return m_path;
}

int Directory::getDepth() const
{
    return m_depth;
}

void Directory::setPath(const QString &path)
{
    m_path = path;
}

void Directory::setDepth(const int &depth)
{
    m_depth = depth;
}

