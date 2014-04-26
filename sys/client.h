#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = 0);

signals:
    void sendClient(QStringList listClient);

public slots:
    void readFile();
private:
    QStringList clientList;

};

#endif // CLIENT_H