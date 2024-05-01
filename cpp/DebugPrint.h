#ifndef DEBUGPRINT_H
#define DEBUGPRINT_H

#include <QDebug>

#define PRETTY_FUNC false

#if PRETTY_FUNC
#define F_NAME __PRETTY_FUNCTION__
#else
#define F_NAME __FUNCTION__
#endif

#ifndef DB
// easier to trace execution path
#define DB qDebug() "\n" << F_NAME << "\n"
#endif


#ifndef WR
// easier to trace WARNING path
#define WR qDebug() "\n############################### " << F_NAME << " ###############################\n"
#endif

#endif // DEBUGPRINT_H
