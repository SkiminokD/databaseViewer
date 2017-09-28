#ifndef DEBUG_H
#define DEBUG_H

#include <QDebug>
#include <QThread>
#include <QTime>

#define ERR_SOURCE __FILE__+QString(":")+QString::number(__LINE__)
#define QSTR_TO_CHAR(text) QString(text).toStdString().data()

#define LOG_MESSAGE_FORMAT(text) \
        " "<<__FUNCTION__<<": "<<QSTR_TO_CHAR(text)<<" ("<<__FILE__<<":"<<__LINE__<<")"


#define PRINT_CRITICAL(text) \
    qCritical().nospace()<<QSTR_TO_CHAR(QTime::currentTime().toString("hh:mm:ss.zzz"))\
                         <<" "<<QThread::currentThreadId() \
                         <<" ERROR:"<<LOG_MESSAGE_FORMAT(text);

#define PRINT_WARNING(text) \
    qWarning().nospace()<<QSTR_TO_CHAR(QTime::currentTime().toString("hh:mm:ss.zzz"))\
                        <<" "<<QThread::currentThreadId() \
                        <<" WARN: "<<LOG_MESSAGE_FORMAT(text);

#define PRINT_DEBUG(text) \
    qDebug().nospace()  <<QSTR_TO_CHAR(QTime::currentTime().toString("hh:mm:ss.zzz"))\
                        <<" "<<QThread::currentThreadId() \
                        <<" DEBUG:"<<LOG_MESSAGE_FORMAT(text);
#define PRINT_INFO(text) \
    qDebug().nospace()  <<QSTR_TO_CHAR(QTime::currentTime().toString("hh:mm:ss.zzz"))\
                        <<" "<<QThread::currentThreadId() \
                        <<" INFO: "<<LOG_MESSAGE_FORMAT(text);
#define PRINT_TRACE(text) \
    qDebug().nospace()  <<QSTR_TO_CHAR(QTime::currentTime().toString("hh:mm:ss.zzz"))\
                        <<" "<<QThread::currentThreadId() \
                        <<" TRACE:"<<LOG_MESSAGE_FORMAT(text);

#endif // DEBUG_H
