#include "Personalization.h"

Personalization::Personalization(QObject *parent)
    : QObject{parent},
    m_isDarkTheme(DEFAULT_IS_DARK_THEME),
    m_darkAccentColor(DEFAULT_DARK_ACCENT_COLOR),
    m_lightAccentColor(DEFAULT_LIGHT_ACCENT_COLOR),
    m_rootDirectory(DEFAULT_ROOT_DIRECTORY),
    m_showTooltips(DEFAULT_SHOW_TOOLTIPS),
    m_songExtensions(DEFAULT_SONG_EXTENSIONS),
    m_songTransitionTimeMS(DEFAULT_SONG_TRANSITION_TIME_MS),
    m_loadProtector(DEFAULT_LOAD_PROTECTOR),
    m_showRefreshListButton(DEFAULT_SHOW_REFRESH_LIST_BUTTON)
{
    // WR << "REMOVING PERSONALIZATIONS";
    // QFile::remove(PERSONALIZATIONS_JSON_PATH);
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
    DB << "\tsongExtensions: " << m_songExtensions;
    DB << "\tsongTransitionTimeMS: " << m_songTransitionTimeMS;
    DB << "\tloadProtector: " << m_loadProtector;
    DB << "\tshowRefreshListButton: " << m_showRefreshListButton;
#endif
}

void Personalization::setDefaultPersonalizationData()
{
    DB << "assigning default values";

    this->setIsDarkTheme(DEFAULT_IS_DARK_THEME);
    this->setDarkAccentColor(DEFAULT_DARK_ACCENT_COLOR);
    this->setLightAccentColor(DEFAULT_LIGHT_ACCENT_COLOR);
    this->setRootDirectory(DEFAULT_ROOT_DIRECTORY);
    this->setShowTooltips(DEFAULT_SHOW_TOOLTIPS);
    this->setSongExtensions(DEFAULT_SONG_EXTENSIONS);
    this->setSongTransitionTimeMS(DEFAULT_SONG_TRANSITION_TIME_MS);
    this->setLoadProtector(DEFAULT_LOAD_PROTECTOR);
    this->setShowRefreshListButton(DEFAULT_SHOW_REFRESH_LIST_BUTTON);
}

int Personalization::loadPersonalizationFromJson()
{
    auto PJP = PERSONALIZATIONS_JSON_PATH;
    if(!QFile(PJP).exists()){
        WR << "file " << PERSONALIZATIONS_JSON_PATH << " not found";
        return 10;
    }

    QFile json_file(PJP);
    if(!json_file.open(QIODevice::ReadOnly | QIODevice::Text)){
        WR << "Can not open personalization json file: " << PJP;
        return 20;
    }

    QJsonParseError json_error;
    QJsonDocument json_data = QJsonDocument::fromJson(json_file.readAll(), &json_error);
    json_file.close();

    if(json_error.error != QJsonParseError::NoError) {
        WR << "json parse error: " << json_error.errorString();
        return 30;
    }

    if(!json_data.isObject()){
        WR << "json file does not contains json object";
        return 40;
    }

    // at this point data are default

    auto jp = json_data.object();
    QString key;


    // try to load data, but if key is missing then notify
    // following code is compressed and can be used only for this purpose

    key = "is dark theme";
    CHECK_KEY(this->setIsDarkTheme(jp[key].toBool()))

    key = "dark accent color";
    CHECK_KEY(this->setDarkAccentColor((QRgb)jp[key].toInteger()))

    key = "light accent color";
    CHECK_KEY(this->setLightAccentColor((QRgb)jp[key].toInteger()))

    key = "root directory";
    CHECK_KEY(this->setRootDirectory(jp[key].toString()))

    key = "show tooltips";
    CHECK_KEY(this->setShowTooltips(jp[key].toBool()))

    key = "song extensions";
    CHECK_KEY(this->setSongExtensions(jp[key].toString()))

    key = "song transition time (ms)";
    CHECK_KEY(this->setSongTransitionTimeMS(jp[key].toInt()))

    key = "load protector";
    CHECK_KEY(this->setLoadProtector(jp[key].toInt()))

    key = "show refresh list button";
    CHECK_KEY(this->setShowRefreshListButton(jp[key].toBool()))


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

    QJsonObject json_object;
    json_object["!NOTE"] = QString(DEFAULT_NOTE);

    json_object["is dark theme"] = this->getIsDarkTheme();
    json_object["dark accent color"] = (qint64) this->getDarkAccentColor().rgba();
    json_object["light accent color"] = (qint64) this->getLightAccentColor().rgba();
    json_object["root directory"] = this->getRootDirectory().toString();
    json_object["show tooltips"] = this->getShowTooltips();
    json_object["song extensions"] = this->getSongExtensions();
    json_object["song transition time (ms)"] = this->getSongTransitionTimeMS();
    json_object["load protector"] = this->getLoadProtector();
    json_object["show refresh list button"] = this->getShowRefreshListButton();

    QJsonDocument json_data(json_object);

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

QUrl Personalization::getRootDirectory() const
{
    return m_rootDirectory;
}

bool Personalization::getShowTooltips() const
{
    return m_showTooltips;
}

QString Personalization::getSongExtensions() const
{
    return m_songExtensions;
}

int Personalization::getSongTransitionTimeMS() const
{
    return m_songTransitionTimeMS;
}

int Personalization::getLoadProtector() const
{
    return m_loadProtector;
}

bool Personalization::getShowRefreshListButton() const
{
    return m_showRefreshListButton;
}


void Personalization::setIsDarkTheme(bool isDarkTheme)
{
    if(m_isDarkTheme == isDarkTheme) // removes binding loop in qml
        return;

    m_isDarkTheme = isDarkTheme;
    emit this->isDarkThemeChanged();
}

void Personalization::setDarkAccentColor(const QColor &accentColor)
{
    if(m_darkAccentColor == accentColor) // removes binding loop in qml
        return;

    m_darkAccentColor = accentColor;
    emit this->darkAccentColorChanged();
}

void Personalization::setLightAccentColor(const QColor &accentColor)
{
    if(m_lightAccentColor == accentColor) // removes binding loop in qml
        return;

    m_lightAccentColor = accentColor;
    emit this->lightAccentColorChanged();
}

void Personalization::setRootDirectory(const QUrl &newDirectory)
{
    if(m_rootDirectory == newDirectory) // removes binding loop in qml
        return;

    m_rootDirectory = newDirectory;
    emit this->rootDirectoryChanged();
}

void Personalization::setShowTooltips(bool showTooltips)
{
    if(m_showTooltips == showTooltips) // removes binding loop in qml
        return;

    m_showTooltips = showTooltips;
    emit this->showTooltipsChanged();
}

void Personalization::setSongExtensions(const QString &songExtensions)
{
    if(m_songExtensions == songExtensions) // removes binding loop in qml
        return;

    m_songExtensions = songExtensions;
    emit this->songExtensionsChanged();
}

void Personalization::setSongTransitionTimeMS(int songTransitionTimeMS)
{
    if(m_songTransitionTimeMS == songTransitionTimeMS) // removes binding loop in qml
        return;

    if(songTransitionTimeMS < 0){
        WR << "Trying to set negative time in songTransitionTime!";
        songTransitionTimeMS = 0;
    }
    m_songTransitionTimeMS = songTransitionTimeMS;
    emit this->songTransitionTimeMSChanged();
}

void Personalization::setLoadProtector(int loadProtector)
{
    if(m_loadProtector == loadProtector) // removes binding loop in qml
        return;

    if(loadProtector < 0){
        WR << "Trying to set negative count in loadProtector!";
        loadProtector = 0;
    }
    m_loadProtector = loadProtector;
    emit this->loadProtectorChanged();
}

void Personalization::setShowRefreshListButton(bool showRefreshListButton)
{
    if(m_showRefreshListButton == showRefreshListButton) // removes binding loop in qml
        return;

    m_showRefreshListButton = showRefreshListButton;
    emit this->showRefreshListButtonChanged();
}

