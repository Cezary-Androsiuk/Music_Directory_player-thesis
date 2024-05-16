#include "cpp/DebugPrint.h"

void lifeCycleLog(QString func, QString data){
#if PRINT_LIFE_CYCLE_LOG
    qDebug() << "   :" << data;
#endif

    QFile logFile("runtimeLog.txt");
    if (!logFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text))
    {
        WR << "error while saving log in pathTracer";
        return;
    }
    QTextStream logStream(&logFile);
    logStream << QString(QString(F_NAME) + " -").toStdString().c_str() << data << "\n";
    logFile.close();
}
