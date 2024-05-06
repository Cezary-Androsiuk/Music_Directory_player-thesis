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
#define PRINT_LOAD_AND_SAVE_VALUES true

#define DEFAULT_IS_DARK_THEME true
#define DEFAULT_DARK_ACCENT_COLOR qRgb(215,90,90)
#define DEFAULT_LIGHT_ACCENT_COLOR qRgb(90,0,120)
#define DEFAULT_ROOT_DIRECTORY ""
#define DEFAULT_SHOW_TOOLTIPS true

#define DEFAULT_NOTE ""\
"###"\
"### Any change to the contents of this file, may lead to unexpected results."\
"###"

class Personalization : public QObject
{
    Q_OBJECT
    // frontend app personalisation accessors
    Q_PROPERTY(bool isDarkTheme                 READ getIsDarkTheme             WRITE setIsDarkTheme            NOTIFY isDarkThemeChanged FINAL)
    Q_PROPERTY(QColor darkAccentColor           READ getDarkAccentColor         WRITE setDarkAccentColor        NOTIFY darkAccentColorChanged)
    Q_PROPERTY(QColor lightAccentColor          READ getLightAccentColor        WRITE setLightAccentColor       NOTIFY lightAccentColorChanged)
    Q_PROPERTY(QString rootDirectory            READ getRootDirectory           WRITE setRootDirectory          NOTIFY rootDirectoryChanged)
    Q_PROPERTY(bool showTooltips                READ getShowTooltips            WRITE setShowTooltips           NOTIFY showTooltipsChanged)

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
    QString getRootDirectory() const;
    bool getShowTooltips() const;

    void setIsDarkTheme(bool isDarkTheme);
    void setDarkAccentColor(const QColor &accentColor);
    void setLightAccentColor(const QColor &accentColor);
    void setRootDirectory(const QString &newDirectory);
    void setShowTooltips(bool showTooltips);

signals:
    void isDarkThemeChanged();
    void darkAccentColorChanged();
    void lightAccentColorChanged();
    void rootDirectoryChanged();
    void showTooltipsChanged();

private:
    bool m_isDarkTheme;
    QColor m_darkAccentColor;
    QColor m_lightAccentColor;
    QString m_rootDirectory;
    bool m_showTooltips;
};

#endif // PERSONALIZATION_H
