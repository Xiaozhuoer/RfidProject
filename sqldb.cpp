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


#include "sqldb.h"
#include <QtDebug>


Sqldb::Sqldb()
{

}

Sqldb::~Sqldb()
{
    db.close();
    if(db.open()){
        qDebug()<<"sqlite close error";
    }
    else{
        qDebug()<<"sqlite close success";
    }

}

void Sqldb::InitDbDate()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("RfidDataBase.db");
    db.setUserName("root");
    db.setPassword("xzer");

}

bool Sqldb::InitConnet()
{
    InitDbDate();
    db.open();
    if(!db.open())
    {
      qDebug()<<"connect to sqlite error"<<db.lastError().text();
      return false;
    }
    else
    {
     qDebug()<<"connect to sqlite success";
     return true;
    }
}

bool Sqldb::InsertData(QString cardnum )
{
    int initMoney = 50;
    QSqlQuery sql_query;
    QString insert_sql = "insert into cardinfo values (?,?)";
    sql_query.prepare(insert_sql);
    sql_query.addBindValue(cardnum);
    sql_query.addBindValue(initMoney);
    if(!sql_query.exec())
    {
        qDebug() << sql_query.lastError().text();
        return false;
    }
    else
    {
        qDebug() << "insertdata success";
        return true;
    }
}

QSqlQuery Sqldb::SelectAllData()
{
    QSqlQuery sql_query;
    QString select_sql = "select * from cardinfo";
    sql_query.prepare(select_sql);
    sql_query.exec();
    return sql_query;
}

QSqlQuery Sqldb::SelectMoneyNumData(QString cardnum)
{
    QSqlQuery sql_query;
    QString select_sql = "select * from cardinfo where cardnum = ?";
    sql_query.prepare(select_sql);
    sql_query.addBindValue(cardnum);
    sql_query.exec();

    return sql_query;
}

QSqlQuery Sqldb::UpdateInitMoneyNumData(QString cardnum, int initmoneynum)
{
    QSqlQuery sql_query;
    QString update_sql = "update cardinfo set moneynum = ? where cardnum = ?";
    sql_query.prepare(update_sql);
    sql_query.addBindValue(initmoneynum);
    sql_query.addBindValue(cardnum);
    sql_query.exec();
    sql_query = SelectMoneyNumData(cardnum);
    return sql_query;
}

QSqlQuery Sqldb::UpdateAddMoneyNumData(QString cardnum, int addmoneynum)
{
    int nowMoneyNum = 0;
    QSqlQuery sql_query = SelectMoneyNumData(cardnum);
    while(sql_query.next()){
        nowMoneyNum = sql_query.value("moneynum").toInt();
    }
    QString update_sql = "update cardinfo set moneynum = ? where cardnum = ?";
    sql_query.prepare(update_sql);
    sql_query.addBindValue(nowMoneyNum + addmoneynum);
    sql_query.addBindValue(cardnum);
    sql_query.exec();
    sql_query = SelectMoneyNumData(cardnum);
    return sql_query;
}

QSqlQuery Sqldb::UpdateSubMoneyNumData(QString cardnum, int submoneynum)
{
    int nowMoneyNum = 0;
    QSqlQuery sql_query = SelectMoneyNumData(cardnum);
    while(sql_query.next()){
        nowMoneyNum = sql_query.value("moneynum").toInt();
    }
    QString update_sql = "update cardinfo set moneynum = ? where cardnum = ?";
    sql_query.prepare(update_sql);
    sql_query.addBindValue(nowMoneyNum - submoneynum);
    sql_query.addBindValue(cardnum);
    sql_query.exec();
    sql_query = SelectMoneyNumData(cardnum);
    return sql_query;
}



















