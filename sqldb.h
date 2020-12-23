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

#ifndef SQLDB_H
#define SQLDB_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>


class Sqldb
{
public:
    Sqldb();
    ~Sqldb();

    bool InitConnet();
    bool InsertData(QString );
    QSqlQuery SelectAllData();
    QSqlQuery SelectMoneyNumData(QString );
    QSqlQuery UpdateInitMoneyNumData(QString, int );
    QSqlQuery UpdateAddMoneyNumData(QString, int );
    QSqlQuery UpdateSubMoneyNumData(QString, int );
private:
    void InitDbDate();
    QSqlDatabase db;
};

#endif // SQLDB_H
