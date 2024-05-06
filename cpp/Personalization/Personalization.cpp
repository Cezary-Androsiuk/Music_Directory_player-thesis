#include "Personalization.h"

Personalization::Personalization(QObject *parent)
    : QObject{parent},
    m_isDarkTheme(DEFAULT_IS_DARK_THEME),
    m_darkAccentColor(DEFAULT_DARK_ACCENT_COLOR),
    m_lightAccentColor(DEFAULT_LIGHT_ACCENT_COLOR),
    m_rootDirectory(DEFAULT_ROOT_DIRECTORY),
    m_showTooltips(DEFAULT_SHOW_TOOLTIPS)
{
    QFile::remove(PERSONALIZATIONS_JSON_PATH);
}

Personalization::~Personalization()
{

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

    {
        QString key = "is dark theme";
        if(jp.contains(key)) this->setIsDarkTheme(jp[key].toBool());
        else WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";
    }
    {
        QString key = "dark accent color";
        if(jp.contains(key)) this->setDarkAccentColor((QRgb)jp[key].toInteger());
        else WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";

    }
    {
        QString key = "light accent color";
        if(jp.contains(key)) this->setLightAccentColor((QRgb)jp[key].toInteger());
        else WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";

    }
    {
        QString key = "root directory";
        if(jp.contains(key)) this->setRootDirectory(jp[key].toString());
        else WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";
    }
    {
        QString key = "show tooltips";
        if(jp.contains(key)) this->setShowTooltips(jp[key].toBool());
        else WR << PERSONALIZATIONS_JSON_PATH << " file not contains value related with '" << key << "' key";
    }

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
    json_personalizations["!NOTE"] = QString(DEFAULT_NOTE);
    json_personalizations["is dark theme"] = this->getIsDarkTheme();
    json_personalizations["dark accent color"] = (qint64) this->getDarkAccentColor().rgba();
    json_personalizations["light accent color"] = (qint64) this->getLightAccentColor().rgba();
    json_personalizations["root directory"] = this->getRootDirectory();
    json_personalizations["show tooltips"] = this->getShowTooltips();

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

