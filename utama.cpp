#include "utama.h"
#include "ui_utama.h"

Utama::Utama(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Utama)
{
    //1. Set Up Layout
    ui->setupUi(this);
    this->setCentralWidget(ui->tabWidget);
    ui->tabWidget->setCurrentIndex(0);
    this->setupTabel();
    //2. Call Function
    this->getClient();
}

void Utama::setupTabel(){
    QStringList tableHeader;
    tableHeader<<"Hostname"<<"IP Address"<<"Mac Address"<<"Lease";
    ui->tabelClient->setColumnCount(tableHeader.size());
    ui->tabelClient->setHorizontalHeaderLabels(tableHeader);
    ui->tabelClient->horizontalHeader()->setStretchLastSection(true);
}

void Utama::getClient(){
    client = new Client();
    clientThread = new QThread();

    client->moveToThread(clientThread);
    connect(clientThread,SIGNAL(started()),client,SLOT(startDaemon()));
    connect(client,SIGNAL(sendClient(QStringList)),this,SLOT(recieveClient(QStringList)));


    clientThread->start();
}

//Slot
void Utama::recieveClient(QStringList clientList){
    QString client;
    QStringList clientDetail;
    int count = clientList.size();

    ui->tabelClient->clear();
    this->setupTabel();
    ui->tabelClient->setRowCount(count);

    for(int x=0; x<=count-1; x++){
        client = clientList.value(x);
        clientDetail = client.split(" ");
        ui->tabelClient->setItem(x,0,new QTableWidgetItem(QString(clientDetail.value(3))));
        ui->tabelClient->setItem(x,1,new QTableWidgetItem(QString(clientDetail.value(2))));
        ui->tabelClient->setItem(x,2,new QTableWidgetItem(QString(clientDetail.value(1))));
        QDateTime time,curtime;
        int current = (int) curtime.currentDateTime().toTime_t();
        time.setTime_t(clientDetail.value(0).toInt() - current);
        //time.toString(Qt::SystemLocaleShortDate);
        ui->tabelClient->setItem(x,3,new QTableWidgetItem(QString(time.toString("hh:mm:ss"))));
    }
}

void Utama::on_btStart_clicked()
{
    this->startService();
}

void Utama::on_btStop_clicked()
{
    this->stopService();
}

void Utama::startService(){
    QString fileTmp = "/tmp/startHostapd.chip";
    QFile fileStart(fileTmp);

    qDebug()<<"Info : Execute start script";

    if( fileStart.open(QIODevice::ReadWrite | QIODevice::Truncate) ){
        QTextStream stream(&fileStart);
        stream<<"#!/usr/bin/pkexec /bin/bash\n";
        stream<<"echo \"Setting IP WLAN\"\n";
        stream<<"ifconfig wlan0 192.168.150.1\n";
        stream<<"echo \"Restart DNSMASQ\"\n";
        stream<<"service dnsmasq restart\n";
        stream<<"echo \"Setting IP Forward\"\n";
        stream<<"sysctl net.ipv4.ip_forward=1\n";
        stream<<"echo \"Setting IP Tables\"\n";
        stream<<"iptables -t nat -A POSTROUTING -o ppp0 -j MASQUERADE\n";
        stream<<"echo \"Start HostAPd\"\n";
        stream<<"hostapd -B -P /tmp/hostapdPID.chip /etc/hostapd.conf\n";
    }else{
        qDebug()<<"Error : Gagal menulis script start "<<__FILE__<<__LINE__;
    }

    QProcess execChmod,execStart;
    execChmod.start("chmod",QStringList()<<"+x"<<fileTmp);
    execChmod.waitForFinished(-1);
    execChmod.close();

    QString command = "pkexec --user root /bin/bash "+fileTmp;
    qDebug()<<command;
    execStart.start(command);
    execStart.waitForReadyRead(-1)
    qDebug()<<execStart.readAll();

    execStart.close();

}

void Utama::stopService(){
    QString fileTmp = "/tmp/stopHostapd.chip";
    QFile fileStop(fileTmp);

    if( fileStop.open(QIODevice::ReadWrite | QIODevice::Truncate) ){
        QTextStream stream(&fileStop);
        qDebug()<<"Info : Execute stop script";
        stream<<"#!/usr/bin/pkexec /bin/bash\n";
        stream<<"echo \"Restore IP Tables\"\n";
        stream<<"iptables -D POSTROUTING -t nat -o ppp0 -j MASQUERADE\n";
        stream<<"echo \"Restore IP Forward\"\n";
        stream<<"sysctl net.ipv4.ip_forward=0\n";
        stream<<"echo \"Stop DNSMASQ\"\n";
        stream<<"service dnsmasq stop\n";
        stream<<"echo \"Stop HostAPd\"\n";
        stream<<"service hostapd stop\n";
        stream<<"echo \"Kill Service IP WLAN\"\n";
        stream<<"kill `cat /tmp/hostapdPID.chip`";
    }else{
        qDebug()<<"Error : Gagal menulis script stop "<<__FILE__<<__LINE__;
    }

    //Chmod Script
    QProcess execChmod,execStop;
    execChmod.start("chmod +x "+fileTmp);
    execChmod.waitForFinished(-1);
    execChmod.close();

    //Execute the Script
    QString command = "pkexec --user root /bin/bash "+fileTmp;
    execStop.start(command);
    execStop.waitForFinished(-1)
    qDebug()<<execStop.readAll();

    execStop.close();

}

Utama::~Utama()
{
    delete ui;
}




