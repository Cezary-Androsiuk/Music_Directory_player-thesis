#ifndef PERSONALIZATION_H
#define PERSONALIZATION_H

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QColor>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <functional>

#include "cpp/DebugPrint.h"



#define PERSONALIZATIONS_JSON_PATH "./assets/personalizations.json"
#define PRINT_LOAD_AND_SAVE_VALUES 0

#define DEFAULT_NOTE "### Any change to the contents of this file, may lead to unexpected results ###"
#define DEFAULT_IS_DARK_THEME true
#define DEFAULT_DARK_ACCENT_COLOR qRgb(215,90,90)
#define DEFAULT_LIGHT_ACCENT_COLOR qRgb(90,0,120)
#define DEFAULT_ROOT_DIRECTORY QUrl()
// #define DEFAULT_ROOT_DIRECTORY QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
#define DEFAULT_SHOW_TOOLTIPS true
#define DEFAULT_SONG_EXTENSIONS "mp4;mkv;mp3;mov;wav"
#define DEFAULT_SONG_TRANSITION_TIME_MS 300


#define CHECK_KEY(arg) if(jp.contains(key)) arg;\
else WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";

class Personalization : public QObject
{
    Q_OBJECT
    // frontend app personalisation accessors
    Q_PROPERTY(bool isDarkTheme                 READ getIsDarkTheme             WRITE setIsDarkTheme            NOTIFY isDarkThemeChanged)
    Q_PROPERTY(QColor darkAccentColor           READ getDarkAccentColor         WRITE setDarkAccentColor        NOTIFY darkAccentColorChanged)
    Q_PROPERTY(QColor lightAccentColor          READ getLightAccentColor        WRITE setLightAccentColor       NOTIFY lightAccentColorChanged)
    Q_PROPERTY(QUrl rootDirectory               READ getRootDirectory           WRITE setRootDirectory          NOTIFY rootDirectoryChanged)
    Q_PROPERTY(bool showTooltips                READ getShowTooltips            WRITE setShowTooltips           NOTIFY showTooltipsChanged)
    Q_PROPERTY(QString songExtensions           READ getSongExtensions          WRITE setSongExtensions         NOTIFY songExtensionsChanged)
    Q_PROPERTY(int songTransitionTimeMS         READ getSongTransitionTimeMS    WRITE setSongTransitionTimeMS   NOTIFY songTransitionTimeMSChanged)


public:
    explicit Personalization(QObject *parent = nullptr);
    ~Personalization();

private:
    void printValues() const;

public:
    void setDefaultPersonalizationData();
    int loadPersonalizationFromJson();
    int savePersonalizationToJson();

    bool getIsDarkTheme() const;
    QColor getDarkAccentColor() const;
    QColor getLightAccentColor() const;
    QUrl getRootDirectory() const;
    bool getShowTooltips() const;
    QString getSongExtensions() const;
    int getSongTransitionTimeMS() const;

    void setIsDarkTheme(bool isDarkTheme);
    void setDarkAccentColor(const QColor &accentColor);
    void setLightAccentColor(const QColor &accentColor);
    void setRootDirectory(const QUrl &newDirectory);
    void setShowTooltips(bool showTooltips);
    void setSongExtensions(const QString &songExtensions);
    void setSongTransitionTimeMS(int songTransitionTimeMS);

signals:
    void isDarkThemeChanged();
    void darkAccentColorChanged();
    void lightAccentColorChanged();
    void rootDirectoryChanged();
    void showTooltipsChanged();
    void songExtensionsChanged();
    void songTransitionTimeMSChanged();

private:
    bool m_isDarkTheme;
    QColor m_darkAccentColor;
    QColor m_lightAccentColor;
    QUrl m_rootDirectory;
    bool m_showTooltips;
    QString m_songExtensions;
    int m_songTransitionTimeMS; // MS stands for miliseconds (1000ms = 1s)
};

#endif // PERSONALIZATION_H
