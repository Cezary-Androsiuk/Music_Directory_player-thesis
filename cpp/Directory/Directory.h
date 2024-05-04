#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QObject>

class Directory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath CONSTANT FINAL)
    Q_PROPERTY(int depth READ getDepth CONSTANT FINAL)
public:
    explicit Directory(QObject *parent = nullptr);

    QString getPath() const;
    int getDepth() const;

    void setPath(const QString &path);
    void setDepth(const int &depth);

signals:

private:
    QString m_path;
    int m_depth;

signals:
};

#endif // DIRECTORY_H
