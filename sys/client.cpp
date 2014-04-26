#include "sys/client.h"

Client::Client(QObject *parent) :
    QObject(parent)
{
}

void Client::readFile(){
    QString file;
    if( file.isNull() ){
        //Default Parameter
        file = "/var/lib/misc/dnsmasq.leases";
    }

    QFile fileClient(file);
    if( fileClient.open(QIODevice::ReadOnly | QIODevice::Text) ){
        qDebug()<<"Info : File Berhasil dibuka";

        QTextStream clientItem(&fileClient);

        //int count=0;
        while(!clientItem.atEnd()){
            //Membaca Perbaris Line
            clientList.append(clientItem.readLine());
            //qDebug()<<"Client "+clientList.value(count);
        }

        emit sendClient(clientList);

    }else{
        qDebug()<<"Error : Gagal Membuka File "+file+" On Line : "<<__LINE__;
    }
}