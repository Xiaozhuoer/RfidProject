#ifndef SQLITEDB_H
#define SQLITEDB_H
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

struct Date
{
    int CareNun;
    int Money;
};

class sqlitedb
{
public:
    sqlitedb();
    ~sqlitedb();

    //创建数据库并建立连接
    bool ConnectionDb(void);
    //创建数据表（student）
    void createTable(void);
    //在表格中增加新的字段
    void addNewcolumn(QString &columnNameAndproperty);
    //查询和显示结果
    void queryTable(QString& str);
    //判断数据表是否存在
    bool IsTaBexists(QString& Tabname);
    //插入数据
    void Insertdata(QString &singledb);//插入单条数据
    //删除数据
    void deletedata();
    //修改数据
    void updatedata();
    //关闭数据库
    void CloseDb(void);
private:
    QSqlDatabase db; //建立和数据库的连接
};

#endif // SQLITEDB_H
