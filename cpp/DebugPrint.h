#ifndef DEBUGPRINT_H
#define DEBUGPRINT_H

#include <QDebug>
#include <QString>

#include <QFile>
#include <QTextStream>

#define PRETTY_FUNC true
#define MULTI_LINE false

#if PRETTY_FUNC
#define F_NAME __PRETTY_FUNCTION__
#else
#define F_NAME __FUNCTION__
#endif // PRETTY_FUNC

#ifndef DB
// easier to trace execution path
#if MULTI_LINE
#define DB qDebug() << QString("\n" + QString(F_NAME) + ":\n -").toStdString().c_str()
#else
#define DB qDebug() << QString(QString(F_NAME) + " -").toStdString().c_str()
#endif // MULTILINE
#endif // ifndef DB


#ifndef WR
// easier to trace WARNING/ERROR path
#if MULTI_LINE
#define WR qDebug() << QString("\n##############################################################\n### " + QString(F_NAME) + ":\n -").toStdString().c_str()
#else
#define WR qDebug() << QString(/*"##############################################################"*/"\n### " + QString(F_NAME) + " -").toStdString().c_str()
#endif // MULTILINE
#endif // ifndef WR

// // for later:
// #define _DB << QString(F_NAME) << "-    "
// #define DB_OK   qDebug() << "   OK   " _DB
// #define DB_FAIL qDebug() << " FAIL ! " _DB
// #define DB_INFO qDebug() << "    ?   " _DB

// later, in addition, you can add saving the information passed to the DB in the log file and add a class for QML to make it work instead of console.log


// void messageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
// {
//     QFile logFile("log.txt");
//     if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
//         return;

//     QTextStream logStream(&logFile);
//     if(false); // for beauty code below
//     else if(type == QtDebugMsg)     logStream << "Debug: " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
//     else if(type == QtWarningMsg)   logStream << "Warning: " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
//     else if(type == QtCriticalMsg)  logStream << "Critical: " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";
//     else if(type == QtFatalMsg)     logStream << "Fatal: " << msg << " (" << context.file << ":" << context.line << ", " << context.function << ")\n";

//     // Wyświetlanie w terminalu
//     QTextStream terminalStream(stdout);
//     terminalStream << msg << "\n";
// }

#endif // DEBUGPRINT_H
