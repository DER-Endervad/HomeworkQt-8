#ifndef DATABASE_H
#define DATABASE_H

#include <QTableWidget>
#include <QObject>
#include <QSqlError>
#include <QSqlDatabase>
#include <QSqlQuery>

#define POSTGRE_DRIVER "QPSQL"
#define DB_NAME "MyDB"

//Количество полей данных необходимых для подключения к БД
#define NUM_DATA_FOR_CONNECT_TO_DB 5

//Перечисление полей данных
enum fieldsForConnect{
    hostName = 0,
    dbName = 1,
    login = 2,
    pass = 3,
    port = 4
};

//Типы запросов
enum requestType{

    requestAllFilms = 1,
    requestComedy   = 2,
    requestHorrors  = 3

};



class DataBase : public QObject
{
    Q_OBJECT

public:
    explicit DataBase(QObject *parent = nullptr);
    ~DataBase();

    void AddDataBase(QString driver, QString nameDB = "");
    void DisconnectFromDataBase(QString nameDb = "");
    void RequestToDB(QString request);
    void ReadAnswerFromDB( int answerType );
    QSqlError GetLastError(void);
    void ConnectToDataBase(QVector<QString> dataForConnect);
    void ClearWidget();

signals:

   void sig_SendDataFromDB(QTableWidget *tableWg, int typeR);
   void sig_SendStatusConnection(bool);
   void sig_SendStatusRequest(QSqlError err);


private:

    QSqlDatabase* dataBase;
    QSqlQuery* simpleQuery;
    QTableWidget* tableWidget;

};

#endif // DATABASE_H
