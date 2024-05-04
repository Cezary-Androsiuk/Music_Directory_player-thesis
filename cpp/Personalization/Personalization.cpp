#include "Personalization.h"

Personalization::Personalization(QObject *parent)
    : QObject{parent},
    m_isDarkTheme(DEFAULT_IS_DARK_THEME),
    m_darkAccentColor(DEFAULT_DARK_ACCENT_COLOR),
    m_lightAccentColor(DEFAULT_LIGHT_ACCENT_COLOR),
    m_rootDirectory(DEFAULT_ROOT_DIRECTORY),
    m_showTooltips(DEFAULT_SHOW_TOOLTIPS)
{
    QObject::connect(this, &Personalization::rootDirectoryChanged, this, &Personalization::backendPersonalizationDataChanged);
}

Personalization::~Personalization()
{

}

void Personalization::setJsonValue(bool &var, QJsonObject &jo, const QString &key)
{
    if(jo.contains(key))
        var = jo[key].toBool();
    else
        WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";
}
void Personalization::setJsonValue(int &var, QJsonObject &jo, const QString &key)
{
    if(jo.contains(key))
        var = jo[key].toInteger();
    else
        WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";
}
void Personalization::setJsonValue(QString &var, QJsonObject &jo, const QString &key)
{
    if(jo.contains(key))
        var = jo[key].toString();
    else
        WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";
}

void Personalization::setJsonValue(QColor &var, QJsonObject &jo, const QString &key)
{
    if(jo.contains(key))
        var = (QRgb)jo[key].toInteger();
    else
        WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";
}

void Personalization::printValues() const
{
#if PRINT_LOAD_AND_SAVE_VALUES
    DB << "\tisDarkTheme: " << m_isDarkTheme;
    DB << "\tdarkAccentColor: " << m_darkAccentColor.rgba();
    DB << "\tlightAccentColor: " << m_lightAccentColor.rgba();
    DB << "\trootDirectory: " << m_rootDirectory;
    DB << "\tshowTooltips: " << m_showTooltips;
#endif
}

void Personalization::setDefaultPersonalizationData()
{
    DB << "assigning default values";

    m_isDarkTheme = DEFAULT_IS_DARK_THEME;
    m_darkAccentColor = DEFAULT_DARK_ACCENT_COLOR;
    m_lightAccentColor = DEFAULT_LIGHT_ACCENT_COLOR;
    m_rootDirectory = DEFAULT_ROOT_DIRECTORY;
    m_showTooltips = DEFAULT_SHOW_TOOLTIPS;
}

int Personalization::loadPersonalizationFromJson()
{
    auto PJP = PERSONALIZATIONS_JSON_PATH;
    this->setDefaultPersonalizationData();
    if(!QFile(PJP).exists()){
        WR << "file " << PERSONALIZATIONS_JSON_PATH << " not found";
        return 10;
    }

    QFile json_file(PJP);
    if(!json_file.open(QIODevice::ReadOnly | QIODevice::Text)){
        WR << "Can not open personalization json file: " << PJP;
        this->setDefaultPersonalizationData();
        return 20;
    }

    QJsonParseError json_error;
    QJsonDocument json_data = QJsonDocument::fromJson(json_file.readAll(), &json_error);
    json_file.close();

    if(json_error.error != QJsonParseError::NoError) {
        WR << "json parse error: " << json_error.errorString();
        this->setDefaultPersonalizationData();
        return 30;
    }

    if(!json_data.isObject()){
        WR << "json file does not contains json object";
        this->setDefaultPersonalizationData();
        return 40;
    }

    auto jp = json_data.object();

    this->setJsonValue(m_isDarkTheme, jp, "is dark theme");
    this->setJsonValue(m_darkAccentColor, jp, "dark accent color");
    this->setJsonValue(m_lightAccentColor, jp, "light accent color");
    this->setJsonValue(m_rootDirectory, jp, "root directory");
    this->setJsonValue(m_showTooltips, jp, "show tooltips");

    DB << "personalization data readed!";
    this->printValues();
    return 0;
}

int Personalization::savePersonalizationToJson()
{
    QString PJP = PERSONALIZATIONS_JSON_PATH;

    int last_forward_slash = PJP.lastIndexOf('/');
    if(last_forward_slash == -1)
    {
        DB << "it is not possible to determine whether the personalisation save "
              "file is in any sub-pathway, we assume that it is in the application pathway";
    }
    else
    {
        // found that save file is in some directory, create it if not exist
        QDir personalization_directory(PJP.left(last_forward_slash));

        if(!personalization_directory.exists())
        {
            if(!personalization_directory.mkpath("."))
            {
                WR << "error while creating path for" << PJP << "file!";
            }
        }
    }

    QJsonObject json_personalizations;
    json_personalizations["is dark theme"] = m_isDarkTheme;
    json_personalizations["dark accent color"] = (qint64) m_darkAccentColor.rgba();
    json_personalizations["light accent color"] = (qint64) m_lightAccentColor.rgba();
    json_personalizations["root directory"] = m_rootDirectory;
    json_personalizations["show tooltips"] = m_showTooltips;

    QJsonDocument json_data(json_personalizations);

    QFile file(PJP);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        WR << "error while saving json file to " << PJP << " with error " << file.errorString();
        return 10;
    }

    file.write(json_data.toJson());
    file.close();

    DB << "personalization data saved!";
    this->printValues();
    return 0;
}

bool Personalization::getIsDarkTheme() const
{
    return m_isDarkTheme;
}

QColor Personalization::getDarkAccentColor() const
{
    return m_darkAccentColor;
}

QColor Personalization::getLightAccentColor() const
{
    return m_lightAccentColor;
}

QString Personalization::getRootDirectory() const
{
    return m_rootDirectory;
}

bool Personalization::getShowTooltips() const
{
    return m_showTooltips;
}


void Personalization::setIsDarkTheme(bool isDarkTheme)
{
    m_isDarkTheme = isDarkTheme;
    emit this->isDarkThemeChanged();
}

void Personalization::setDarkAccentColor(const QColor &accentColor)
{
    m_darkAccentColor = accentColor;
    emit this->darkAccentColorChanged();
}

void Personalization::setLightAccentColor(const QColor &accentColor)
{
    m_lightAccentColor = accentColor;
    emit this->lightAccentColorChanged();
}

void Personalization::setRootDirectory(const QString &newDirectory)
{
    m_rootDirectory = newDirectory;
    emit this->rootDirectoryChanged();
}

void Personalization::setShowTooltips(bool showTooltips)
{
    m_showTooltips = showTooltips;
    emit this->showTooltipsChanged();
}

