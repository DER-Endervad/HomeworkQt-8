#include "database.h"

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    dataBase = new QSqlDatabase();
    simpleQuery = new QSqlQuery();
}

DataBase::~DataBase()
{
    delete dataBase;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);
}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data, QString nameDB)
{
    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    bool status;
    status = dataBase->open();

    ///Тут должен быть код ДЗ
    if(status) {
        modelDataBase = new QSqlTableModel(0, *dataBase);
        queryModel = new QSqlQueryModel;
    }

    emit sig_SendStatusConnection(status);

}

void DataBase::ClearWidget()
{
    QTableView *view_clear = new QTableView;
    emit sig_SendDataFromDB(view_clear, requestAllFilms);
}
/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{
    delete modelDataBase;
    delete queryModel;
    *dataBase = QSqlDatabase::database(nameDb);
    dataBase->close();
}
/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request) {
    ///Тут должен быть код ДЗ
    if(request == "All") {
        modelDataBase->setTable("film");
        modelDataBase->select();
        modelDataBase->removeColumns(3,11);
        modelDataBase->removeColumn(0);
    } else if (request == "Comedy"){
        queryModel->setQuery("SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Comedy'", *dataBase);
    } else {
        queryModel->setQuery("SELECT title, description FROM film f JOIN film_category fc on f.film_id = fc.film_id JOIN category c on c.category_id = fc.category_id WHERE c.name = 'Horror'", *dataBase);
    }

    emit sig_SendStatusRequest(request);
}

void DataBase::ReadAnswerFromDB(QString requestType) {
    if(requestType == "All") {
        QTableView *view_all = new QTableView();
        view_all->setModel(modelDataBase);
        emit sig_SendDataFromDB(view_all, requestAllFilms);
    } else {
        QTableView *view_CH = new QTableView();
        view_CH->setModel(queryModel);
        emit sig_SendDataFromDB(view_CH, requestAllFilms);
    }
}


/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError();
}
