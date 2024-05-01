#include "Backend.h"

Backend::Backend(QObject *parent)
    : QObject{parent},
    m_configurationLoaded(false),
    m_qmlInitialized(false)
{}

