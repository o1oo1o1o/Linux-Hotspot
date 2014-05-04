#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QThread>
#include <QProcess>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

signals:
    void sendClient(QStringList listClient);

public slots:
    void readFile();
    void startDaemon();
private:
    QStringList clientList;
    bool rundaemon;

};

#endif // CLIENT_H
