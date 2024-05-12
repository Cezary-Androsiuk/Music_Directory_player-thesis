#ifndef DIRECTORY_H
#define DIRECTORY_H

#include <QObject>

class Directory : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath CONSTANT FINAL)
    Q_PROPERTY(int depth READ getDepth CONSTANT FINAL)
    Q_PROPERTY(bool include READ getInclude WRITE setInclude NOTIFY includeChanged FINAL)

public:
    explicit Directory(QObject *parent = nullptr);

    QString getPath() const;
    int getDepth() const;
    bool getInclude() const;

    void setPath(const QString &path);
    void setDepth(const int &depth);
    void setInclude(const bool &include);

signals:
    void includeChanged();

private:
    QString m_path;
    int m_depth;
    bool m_include;

};

#endif // DIRECTORY_H
