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


#ifndef RFIDPROJECT_ZX_H
#define RFIDPROJECT_ZX_H

#include <QMainWindow>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class RfidProject_zx; }
QT_END_NAMESPACE

class RfidProject_zx : public QMainWindow
{
    Q_OBJECT

public:
    RfidProject_zx(QWidget *parent = nullptr);
    ~RfidProject_zx();

    bool doorModuleFlag = true;
    bool signalFlag = true;

private slots:
    void Test();

    void openAntenna();

    void receiveInfo();

    void traCardNumInfo();

    void requestCard();

    void requestAllCard();

    void connectSql();

    void cleanLog();

    void openCardMoneyModule();

    void closeCardMoneyModule();

    void addMoneyNum();

    void subMoneyNum();

    void MoneyNum();

    void openSerialPort();

    void closeSerialPort();

    void addCardUser();

    void openDoorModule();

    void closeDoorModule();

    void countDown();

    void openDoor();

    void closeDoor();

    void addUserDoorModule();

    void timeWait(int );

    void on_pushButton_connectSql_clicked();

    void on_pushButton_cleanlog_clicked();

    void on_pushButton_addMoneyNum_clicked();

    void on_pushButton_subMoneyNum_clicked();

    void on_pushButton_MoneyNum_clicked();

    void on_pushButton_openSerialPort_clicked();

    void on_pushButton_openDoorModule_clicked();

    void on_pushButton_closeDoorModule_clicked();

    void on_pushButton_addUserDoorModule_clicked();

    void on_pushButton_openAntenna_clicked();

    void on_pushButton_requestAll_clicked();

    void on_pushButton_requestCard_clicked();

    void on_pushButton_closeSerialPort_clicked();

    void on_pushButton_addCardUser_clicked();

    void on_pushButton_openCardMoneyModule_clicked();

    void on_pushButton_closeCardMoneyModule_clicked();

private:
    Ui::RfidProject_zx *ui;
    QSerialPort *serialPort;
    QTimer *timerRequest, *timerCountDown;

    int stateSql = 0;
    int stateAntenna = 0;
};
#endif // RFIDPROJECT_ZX_H
