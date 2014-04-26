#ifndef UTAMA_H
#define UTAMA_H

#include <QMainWindow>
#include <QStringList>
#include <sys/client.h>
#include <QThread>
#include <QDebug>
#include <QTableWidgetItem>
#include <QDateTime>
#include <QProcess>
#include <QFile>
#include <QTextStream>

namespace Ui {
class Utama;
}

class Utama : public QMainWindow
{
    Q_OBJECT

public:
    explicit Utama(QWidget *parent = 0);
    void setupTabel();
    void getClient();
    void startService();
    void stopService();
    ~Utama();
public slots:
    void recieveClient(QStringList clientList);

private slots:
    void on_btStart_clicked();

    void on_btStop_clicked();

private:
    Ui::Utama *ui;
    Client *client;
    QThread *clientThread;
};

#endif // UTAMA_H
