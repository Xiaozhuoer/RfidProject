/*
*   Copyright (C) [Xiaozhuoer]
*
*   Licensed under the Apache License, Version 2.0 (the "License");
*   you may not use this file except in compliance with the License.
*   You may obtain a copy of the License at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
*   Unless required by applicable law or agreed to in writing, software
*   distributed under the License is distributed on an "AS IS" BASIS,
*   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*   See the License for the specific language governing permissions and
*   limitations under the License.
*
*   Author: Xiaozhuoer <xiaozhuo2@foxmail.com>
*/

#include "rfidproject_zx.h"
#include "ui_rfidproject_zx.h"
#include "sqldb.h"
#include <QSerialPort>
#include <QDebug>
#include <QTimer>
#include <QTime>

RfidProject_zx::RfidProject_zx(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::RfidProject_zx)
{
    ui->setupUi(this);
    serialPort = new QSerialPort();
    timerRequest = new QTimer();
    timerCountDown = new QTimer();

//    ui->pushButton_openAntenna->setEnabled(false);
//    ui->pushButton_requestAll->setEnabled(false);
//    ui->pushButton_connectSql->setEnabled(false);
    ui->pushButton_requestCard->setEnabled(false);
    ui->lineEdit_CardNum->setEnabled(false);
    ui->pushButton_addCardUser->setEnabled(false);
    ui->pushButton_openCardMoneyModule->setEnabled(false);
    ui->pushButton_closeCardMoneyModule->setEnabled(false);
    ui->pushButton_MoneyNum->setEnabled(false);
    ui->pushButton_addMoneyNum->setEnabled(false);
    ui->lineEdit_addMoneyNum->setEnabled(false);
    ui->pushButton_subMoneyNum->setEnabled(false);
    ui->lineEdit_subMoneyNum->setEnabled(false);
    ui->pushButton_openDoorModule->setEnabled(false);
    ui->pushButton_closeDoorModule->setEnabled(false);
    ui->lineEdit_CardNumDoorModule->setEnabled(false);
    ui->pushButton_addUserDoorModule->setEnabled(false);
}

RfidProject_zx::~RfidProject_zx()
{
    if(serialPort->isOpen()){
        serialPort->close();
    }
    delete serialPort;
    delete ui;
}

void RfidProject_zx::Test()
{
    ui->textBrowser_log->append("test");
}


void RfidProject_zx::openAntenna()
{
    //打开天线
    QByteArray  data;
    data.resize(4);
    data[0] = 0x21;
    data[1] = 0x04;
    data[2] = 0x01;
    data[3] = 0x24;
    serialPort->write(data);
}

void RfidProject_zx::receiveInfo(){
    //接收单片机的数据
    qDebug()<<"ready";
    QByteArray info=serialPort->readAll();

    //数据处理
    //寻卡成功
    if(info.length() == 8){
        QString str = info.toHex().data();
        QString strInfo = "卡号 ： " + str.toUpper().mid(6,8);
        ui->textBrowser_log->append(strInfo);
        ui->lineEdit_CardNum->setText(str.toUpper().mid(6,8));
        traCardNumInfo();
    }
    QString str = info.toHex().data();
    qDebug()<<"receive info : "+str;
    if(str == "21040025"){
        ui->textBrowser_log->append("打开天线成功");
        stateAntenna = 1;
        if(stateSql == 1){
            ui->pushButton_openCardMoneyModule->setEnabled(true);
            ui->pushButton_openDoorModule->setEnabled(true);
        }
    }

    if(str == "31040134"){
        ui->textBrowser_log->append("寻找所有卡失败");
    }
    if(str == "310600040033"){
        ui->textBrowser_log->append("寻找所有卡成功");
        requestCard();
    }
    if(str == "32040234"){
        ui->textBrowser_log->append("寻卡失败");
    }
}

void RfidProject_zx::traCardNumInfo()
{
    //遍历卡信息
    Sqldb *db = new Sqldb();
    ui->textBrowser_log->append("遍历卡信息");
    QSqlQuery sqlquerydata = db->SelectMoneyNumData(ui->lineEdit_CardNum->text());
    if(sqlquerydata.next()){
        QString strCardNum = "卡号 : " + sqlquerydata.value("cardnum").toString();
        ui->textBrowser_log->append(strCardNum);
        QString strMoneyNum = "余额 : " + sqlquerydata.value("moneynum").toString();
        ui->textBrowser_log->append(strMoneyNum);
    }else{
        ui->textBrowser_log->append("无该卡信息");
    }
}

void RfidProject_zx::requestCard()
{
    //寻卡
    QByteArray  data;
    data.resize(3);
    data[0] = 0x32;
    data[1] = 0x03;
    data[2] = 0x31;
    serialPort->write(data);
    ui->textBrowser_log->append("寻卡中.....");

}

void RfidProject_zx::requestAllCard()
{
    //请求所有卡
    QByteArray  data;
    data.resize(4);
    data[0] = 0x31;
    data[1] = 0x04;
    data[2] = 0x01;
    data[3] = 0x34;
    serialPort->write(data);
}

void RfidProject_zx::connectSql()
{
    //连接数据库
    Sqldb *db = new Sqldb();
    if(db->InitConnet()){
        ui->textBrowser_log->append("连接数据库成功");
        stateSql = 1;
        if(stateAntenna == 1){
            ui->pushButton_openCardMoneyModule->setEnabled(true);
            ui->pushButton_openDoorModule->setEnabled(true);
        }

    }else{
        ui->textBrowser_log->append("连接数据库失败");
    }
}

void RfidProject_zx::cleanLog()
{
    //清空log
    ui->textBrowser_log->clear();
}

void RfidProject_zx::openCardMoneyModule()
{
    //打开电子钱包模块
    ui->pushButton_openDoorModule->setEnabled(false);
    ui->pushButton_openCardMoneyModule->setEnabled(false);
    ui->pushButton_closeCardMoneyModule->setEnabled(true);
    ui->pushButton_MoneyNum->setEnabled(true);
    ui->pushButton_addMoneyNum->setEnabled(true);
    ui->lineEdit_addMoneyNum->setEnabled(true);
    ui->pushButton_subMoneyNum->setEnabled(true);
    ui->lineEdit_subMoneyNum->setEnabled(true);
//    ui->pushButton_requestAll->setEnabled(true);
//    ui->pushButton_connectSql->setEnabled(true);
    ui->pushButton_requestCard->setEnabled(true);
    ui->lineEdit_CardNum->setEnabled(true);
    ui->pushButton_addCardUser->setEnabled(true);
}

void RfidProject_zx::closeCardMoneyModule()
{
    //关闭电子钱包模块
    ui->pushButton_openCardMoneyModule->setEnabled(true);
    ui->pushButton_openDoorModule->setEnabled(true);
    ui->pushButton_closeCardMoneyModule->setEnabled(false);
    ui->pushButton_MoneyNum->setEnabled(false);
    ui->pushButton_addMoneyNum->setEnabled(false);
    ui->lineEdit_addMoneyNum->setEnabled(false);
    ui->pushButton_subMoneyNum->setEnabled(false);
    ui->lineEdit_subMoneyNum->setEnabled(false);
//    ui->pushButton_requestAll->setEnabled(false);
//    ui->pushButton_connectSql->setEnabled(false);
    ui->pushButton_requestCard->setEnabled(false);
    ui->lineEdit_CardNum->setEnabled(false);
    ui->pushButton_addCardUser->setEnabled(false);
    ui->lineEdit_CardNum->clear();
}

void RfidProject_zx::addMoneyNum()
{
    //充值
    Sqldb *db = new Sqldb();
    QString addMoneyNum = ui->lineEdit_addMoneyNum->text();
    QSqlQuery sqlquerydata = db->UpdateAddMoneyNumData(ui->lineEdit_CardNum->text() , addMoneyNum.toInt());
    while(sqlquerydata.next())
    {
        QString strCardNum = "卡号 : " + sqlquerydata.value("cardnum").toString();
        ui->textBrowser_log->append(strCardNum);
        QString strAddMoneyNum = "充值 : " + addMoneyNum;
        ui->textBrowser_log->append(strAddMoneyNum);
        QString strMoneyNum = "余额 : " + sqlquerydata.value("moneynum").toString();
        ui->textBrowser_log->append(strMoneyNum);
    }
    ui->lineEdit_addMoneyNum->clear();
}

void RfidProject_zx::subMoneyNum()
{
    //扣款
    Sqldb *db = new Sqldb();
    QString subMoneyNum = ui->lineEdit_subMoneyNum->text();
    QSqlQuery sqlquerydata = db->UpdateSubMoneyNumData(ui->lineEdit_CardNum->text() , subMoneyNum.toInt());
    while(sqlquerydata.next())
    {
        QString strCardNum = "卡号 : " + sqlquerydata.value("cardnum").toString();
        ui->textBrowser_log->append(strCardNum);
        QString strSubMoneyNum = "扣款 : " + subMoneyNum;
        ui->textBrowser_log->append(strSubMoneyNum);
        QString strMoneyNum = "余额 : " + sqlquerydata.value("moneynum").toString();
        ui->textBrowser_log->append(strMoneyNum);
    }
    ui->lineEdit_subMoneyNum->clear();
}

void RfidProject_zx::MoneyNum()
{
    //余额
    Sqldb *db = new Sqldb();
    QSqlQuery sqlquerydata = db->SelectMoneyNumData(ui->lineEdit_CardNum->text());
    while(sqlquerydata.next())
    {
        QString strCardNum = "卡号 : " + sqlquerydata.value("cardnum").toString();
        ui->textBrowser_log->append(strCardNum);
        QString strMoneyNum = "余额 : " + sqlquerydata.value("moneynum").toString();
        ui->textBrowser_log->append(strMoneyNum);
    }
}

void RfidProject_zx::openSerialPort()
{
    //开启串口
    if(serialPort->isOpen()){
        serialPort->clear();
        serialPort->close();
    }

    serialPort->setPortName(ui->comboBox_Port->currentText());
    serialPort->setBaudRate(QSerialPort::Baud19200,QSerialPort::AllDirections);
    serialPort->setDataBits(QSerialPort::Data8);
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    serialPort->setParity(QSerialPort::NoParity);
    serialPort->setStopBits(QSerialPort::OneStop);

    QString strPort = ui->comboBox_Port->currentText();
    QString strBaudRate = ui->comboBox_BaudRate->currentText();

    if(!serialPort->open(QIODevice::ReadWrite)){
        qDebug()<<"打开失败";
    }else{
        qDebug()<<"打开成功";
        ui->textBrowser_log->append(strPort);
        ui->textBrowser_log->append(strBaudRate);
        ui->textBrowser_log->append("打开串口成功");
//       ui->pushButton_openAntenna->setEnabled(true);
//        ui->pushButton_connectSql->setEnabled(true);
        openAntenna();
        connectSql();
    }
    connect(serialPort,SIGNAL(readyRead()),this,SLOT(receiveInfo()));
}

void RfidProject_zx::closeSerialPort()
{
    //关闭串口
    closeDoorModule();
    closeCardMoneyModule();
    ui->pushButton_openDoorModule->setEnabled(false);
    ui->pushButton_openCardMoneyModule->setEnabled(false);
    serialPort->clear();
    serialPort->close();
    if(serialPort->isOpen()){
        ui->textBrowser_log->append("关闭串口失败");
    }else{
        ui->textBrowser_log->append("关闭串口成功");
//        ui->pushButton_openAntenna->setEnabled(false);
//        ui->pushButton_connectSql->setEnabled(false);
    }
}

void RfidProject_zx::addCardUser()
{
    //添加用户
    Sqldb *db = new Sqldb();
    if(db->InsertData(ui->lineEdit_CardNum->text()))
    {
        QString strCardNum = "卡号 : " + ui->lineEdit_CardNum->text();
        ui->textBrowser_log->append(strCardNum);
        QString str = "添加用户成功" ;
        ui->textBrowser_log->append(str);
        QString strMoneyNum = "余额 : 50 ";
        ui->textBrowser_log->append(strMoneyNum);
    }else{
        ui->textBrowser_log->append("添加用户失败");

    }
}

void RfidProject_zx::openDoorModule()
{
    //打开门禁模块
//    changeDoorModuleFlag();
    if(doorModuleFlag){
//        ui->textBrowser_log->append("门禁系统开启");
        requestAllCard();
        requestCard();
        timeWait(1000);
        if(!ui->lineEdit_CardNum->text().isEmpty()){
            Sqldb *db = new Sqldb();
            QSqlQuery sqlquerydata = db->SelectMoneyNumData(ui->lineEdit_CardNum->text());
            if(sqlquerydata.next()){
                closeDoorModule();
                QString strCardNum = "存在卡号 : " + sqlquerydata.value("cardnum").toString();
                ui->textBrowser_log->append(strCardNum);
                openDoor();
            }else{
                ui->textBrowser_log->append("无该卡信息，拒绝开门!!!");
                ui->lineEdit_CardNumDoorModule->setText(ui->lineEdit_CardNum->text());
                ui->lineEdit_CardNum->clear();
                openDoorModule();
            }
        }else{
            openDoorModule();
        }
    }
}

void RfidProject_zx::closeDoorModule()
{

}

void RfidProject_zx::countDown()
{
    //倒计时
    for(int downTimevalue = 5; downTimevalue > 0; downTimevalue--){
        QString str = QString::number(downTimevalue);
        ui->textBrowser_log->append(str);
        timeWait(1000);
    }
    closeDoor();
}

void RfidProject_zx::openDoor()
{
    //开门
    ui->textBrowser_log->append("开门!!!.....5秒后自动关门");
    countDown();
}

void RfidProject_zx::closeDoor()
{
    ui->textBrowser_log->append("关门!!!.....");
    ui->lineEdit_CardNum->clear();
    openDoorModule();

}

void RfidProject_zx::addUserDoorModule()
{
    //添加用户门禁模块
    Sqldb *db = new Sqldb();
    if(db->InsertData(ui->lineEdit_CardNumDoorModule->text()))
    {
        QString strCardNum = "卡号 : " + ui->lineEdit_CardNum->text();
        ui->textBrowser_log->append(strCardNum);
        QString str = "添加用户成功" ;
        ui->textBrowser_log->append(str);
        QString strMoneyNum = "余额 : 50 ";
        ui->textBrowser_log->append(strMoneyNum);
    }else{
        ui->textBrowser_log->append("添加用户失败");

    }
}

void RfidProject_zx::timeWait(int msec)
{

    QTime _Timer = QTime::currentTime().addMSecs(msec);
    while( QTime::currentTime() < _Timer ){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }

}

void RfidProject_zx::on_pushButton_connectSql_clicked()
{
    connectSql();
}

void RfidProject_zx::on_pushButton_cleanlog_clicked()
{
    cleanLog();
}

void RfidProject_zx::on_pushButton_addMoneyNum_clicked()
{
    addMoneyNum();
}

void RfidProject_zx::on_pushButton_subMoneyNum_clicked()
{
    subMoneyNum();
}

void RfidProject_zx::on_pushButton_MoneyNum_clicked()
{
    MoneyNum();
}

void RfidProject_zx::on_pushButton_openSerialPort_clicked()
{
    openSerialPort();
}

void RfidProject_zx::on_pushButton_openDoorModule_clicked()
{
    doorModuleFlag = true;
    ui->pushButton_openCardMoneyModule->setEnabled(false);
    ui->pushButton_openDoorModule->setEnabled(false);
    ui->pushButton_closeDoorModule->setEnabled(true);
    ui->lineEdit_CardNumDoorModule->setEnabled(true);
    ui->pushButton_addUserDoorModule->setEnabled(true);
    openDoorModule();
}

void RfidProject_zx::on_pushButton_closeDoorModule_clicked()
{
    ui->textBrowser_log->append("门禁系统关闭");
    doorModuleFlag = false;
    ui->pushButton_openCardMoneyModule->setEnabled(true);
    ui->pushButton_openDoorModule->setEnabled(true);
    ui->pushButton_closeDoorModule->setEnabled(false);
    ui->lineEdit_CardNumDoorModule->setEnabled(false);
    ui->pushButton_addUserDoorModule->setEnabled(false);
    ui->lineEdit_CardNumDoorModule->clear();
    closeDoorModule();
}

void RfidProject_zx::on_pushButton_addUserDoorModule_clicked()
{
    addUserDoorModule();
}

void RfidProject_zx::on_pushButton_openAntenna_clicked()
{
    openAntenna();

}

void RfidProject_zx::on_pushButton_requestAll_clicked()
{
    requestAllCard();
}

void RfidProject_zx::on_pushButton_requestCard_clicked()
{
    requestAllCard();
}

void RfidProject_zx::on_pushButton_closeSerialPort_clicked()
{
    closeSerialPort();
}

void RfidProject_zx::on_pushButton_addCardUser_clicked()
{
    addCardUser();
}



void RfidProject_zx::on_pushButton_openCardMoneyModule_clicked()
{
    openCardMoneyModule();
}

void RfidProject_zx::on_pushButton_closeCardMoneyModule_clicked()
{
    closeCardMoneyModule();
}
