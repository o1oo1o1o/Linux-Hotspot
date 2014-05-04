#include "sys/client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
}

void Client::readFile(){
    QString file = "/var/lib/misc/dnsmasq.leases";

    QFile fileClient(file);
    if( fileClient.open(QIODevice::ReadOnly | QIODevice::Text) ){
        //qDebug()<<"Info : File Berhasil dibuka";

        QTextStream clientItem(&fileClient);
        clientList.clear();



        //int count=0;
        while(!clientItem.atEnd()){
            QString item = clientItem.readLine();
            QString ip = item.split(" ").value(2);
            QString state;

            //Ping Client
            int pingResult = QProcess::execute("ping",QStringList()<<"-c1"<<ip);
            if(pingResult == 0){
                state = "Connected";
            }else{
                state = "Disconnect";
            }

            //Membaca Perbaris Line
            clientList.append(item+" "+state);
            //qDebug()<<"Client "+clientList.value(count);
        }


        emit sendClient(clientList);

    }else{
        qDebug()<<"Error : Gagal Membuka File "+file+" On Line : "<<__LINE__;
        emit sendClient(clientList);
    }
}

void Client::startDaemon(){
    while(rundaemon){
        this->readFile();
        QThread::usleep(10000000);
        //qDebug()<<"Running";
    }
}
