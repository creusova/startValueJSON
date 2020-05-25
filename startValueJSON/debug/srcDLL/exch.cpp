#include <exch.h>
#include <QtSql>
#include <QString>
#include <QNetworkInterface>
#include <QCoreApplication>
#include <QThread>

// Applicaton Core (Необходимо для работы слотов и сигналов)

QCoreApplication *app=nullptr;
class TDllThread : public QThread {
protected:
 void run();
public:
 TDllThread();
 ~TDllThread();
};

void TDllThread::run() { app->exec(); }

TDllThread::TDllThread()
{
  int argc=0;
  if (app!=nullptr) {delete app;}
  app=new QCoreApplication(argc, NULL);
  start(QThread::LowPriority);
}

TDllThread::~TDllThread()
{
 if (app!=nullptr) {delete app;}
}

TDllThread *DllThread = nullptr;

// Переменные

static int Client_ID = 0;
static QString pgHostName="localhost";
static QString pgDatabaseName="postgres";
static int pgPort=5432;
static QString pgUserName="postgres";
static QString pgPassword="";
static wchar_t* vExchFromDb=nullptr;
static wchar_t* vDebugInfo=nullptr;
static QString qDebugInfo="DLL have loaded.";
static QString IniFileName="settings.ini";
#ifndef Q_OS_WIN
  static QString qLinAppFolder=".cnii_rtc";
#endif
static void (*new_changes_came_event)(int)=nullptr;
static QString NotifyName="new_changes_came";
static QString qMyIp = "";
static bool AmIQt=false;

//Основновной класс DLL

class TDllExchange : public QObject {

private:
  int sID=0;
  bool dbTryToConnect();
public:
  TDllExchange();
  QSqlDatabase db = QSqlDatabase(QSqlDatabase::addDatabase("QPSQL"));
  bool notify_subscribed=false;
  bool db_connected=false;
  bool _dbConnect(QString dbDatabaseName,
                  QString dbUserName,
                  QString dbPassword,
                  QString dbHostName,
                  int dbPort);
  bool _dbConnectIni();
  bool _dbDisconnect();
  QString _GetDBDataString(int Session_ID);
  bool _SetDBDataString(int Session_ID, QString IncomingString);
  int _GetIpSession(QString sIP);
  int _MySession();
  bool _SessionExists(int Session_ID);
  int _NewSession(int tren_num, QString sIP);
  bool _CloseSession(int Session_ID);
  int _GetTrenBySession(int Session_ID);
  int _MyTrenNum();

public slots:
 void SlotNotify(const QString &name, QSqlDriver::NotificationSource, const QVariant &payload);
};

bool TDllExchange::dbTryToConnect()
{
 qDebugInfo="";
 if (Client_ID!=0) {
 if (!db_connected) {
    db.setHostName(pgHostName);
    db.setPort(pgPort);
    db.setDatabaseName(pgDatabaseName);
    db.setUserName(pgUserName);
    db.setPassword(pgPassword);
    db_connected=db.open();
    if ((db_connected) && (!notify_subscribed)) { notify_subscribed=db.driver()->subscribeToNotification(NotifyName); }
                    }
    if (notify_subscribed) { qDebugInfo+=" DBSubscribed [OK]. ";} else { qDebugInfo+=" DBSubscribed [ERROR]. ";}
    if (db_connected) { qDebugInfo+="DBConnection [OK]."; } else { qDebugInfo+="DBConnection [ERROR]: Server=\""+pgHostName+"\";";
                                                                   qDebugInfo+="Port=%1,DB=\""+pgDatabaseName+"\";";
                                                                   qDebugInfo=qDebugInfo.arg(pgPort)+"User=\""+pgUserName+"\";";
                                                                   qDebugInfo+="Password=\""+pgPassword+"\" ] ";
                                                                   qDebugInfo+="Error Message: \""+db.lastError().text().trimmed()+"\"";}

    QObject::connect(db.driver(),
                static_cast<void(QSqlDriver::*)(const QString &, QSqlDriver::NotificationSource, const QVariant &)>(&QSqlDriver::notification),
                this,
                static_cast<void(TDllExchange::*)(const QString &, QSqlDriver::NotificationSource, const QVariant &)>(&TDllExchange::SlotNotify)
                ); } else {qDebugInfo="Dll was not inited correctly."; }
 return db_connected;
}

TDllExchange::TDllExchange()
{

}

bool TDllExchange::_dbConnect(QString dbDatabaseName, QString dbUserName, QString dbPassword, QString dbHostName, int dbPort)
{
  if (_dbDisconnect()) { pgHostName=dbHostName;
                         pgDatabaseName=dbDatabaseName;
                         pgPort=(dbPort==0)?5432:dbPort;
                         pgUserName=dbUserName;
                         pgPassword=dbPassword;
                         dbTryToConnect(); }
  return db_connected;
}


bool TDllExchange::_dbConnectIni()
{
  if (_dbDisconnect()) {
     QString IniFile;
     #ifdef Q_OS_WIN
      IniFile=QDir::currentPath();
     #else
      IniFile=QDir::homePath()+"/"+qLinAppFolder;
     #endif
     IniFile+="/"+IniFileName;
     QSettings conf(IniFile, QSettings::IniFormat);
     pgHostName=conf.value("DATABASE/pgHostName",pgHostName).toString();
     pgDatabaseName=conf.value("DATABASE/pgDatabaseName",pgDatabaseName).toString();
     pgUserName=conf.value("DATABASE/pgUserName",pgUserName).toString();
     pgPassword=conf.value("DATABASE/pgPassword",pgPassword).toString();
     pgPort=conf.value("DATABASE/pgPort",5432).toInt();
     dbTryToConnect();
    }
    return db_connected;
}

bool TDllExchange::_dbDisconnect()
{
  qDebugInfo="dbDisconnect() have called.";
  if (db_connected)
     {
      if (notify_subscribed) {db.driver()->unsubscribeFromNotification(NotifyName); }
      db.close();
      db_connected=db.isOpen();
     }
  return !db_connected;
}

QString TDllExchange::_GetDBDataString(int Session_ID)
{
  QString _Result="";
  if ((db_connected) && (Session_ID>0) && (Client_ID>0))
    {
     QString SQLText("select GetDBDataString(%1,%2) as result;");
     QSqlQuery qu(db);
     if (qu.exec(SQLText.arg(Session_ID).arg(Client_ID)))
      {
       qu.first();
       QSqlRecord rec = qu.record();
       _Result = qu.value(rec.indexOf("result")).toString();
      }
    }
  return _Result;
}

bool TDllExchange::_SetDBDataString(int Session_ID, QString IncomingString)
{
  bool _Result=false;
    if ((IncomingString.trimmed().length()>0) && (Client_ID>0) && db_connected)
     {
      QString SQLText("select SetDBDataString(%1,%2,:JSON) as result;");
      SQLText=SQLText.arg(Session_ID).arg(Client_ID);
      QSqlQuery qu(db);
       qu.prepare(SQLText);
       qu.bindValue(":JSON",IncomingString.trimmed());
          if (qu.exec())
           {
            qu.first();
            QSqlRecord rec = qu.record();
            _Result=qu.value(rec.indexOf("result")).toBool();
             }
      }
  qDebugInfo="SetDBDataString(%1,\""+IncomingString.trimmed()+"\") have called.";
  qDebugInfo=qDebugInfo.arg(Session_ID);
  return _Result;
}

int TDllExchange::_GetIpSession(QString sIP)
{
    int _Result=0;
    if ((Client_ID>0) && db_connected && (sIP.trimmed()!=""))
         {
          QString SQLText("select public.getmysession(:IP) as result;");
          QSqlQuery qu(db);
           qu.prepare(SQLText);
           qu.bindValue(":IP",sIP);
              if (qu.exec())
               {
                qu.first();
                QSqlRecord rec=qu.record();
                _Result=qu.value(rec.indexOf("result")).toInt();
                 }
          }
    qDebugInfo="GetIpSession(\""+sIP+"\") have called.";
    return _Result;
}

int TDllExchange::_MySession()
{
    qDebugInfo="MySession() have called for \""+qMyIp+"\".";
    if ((Client_ID==1) or (Client_ID==4)) { return _GetIpSession(qMyIp); } else {return 0;}
}

bool TDllExchange::_SessionExists(int Session_ID)
{
    bool _Result=false;
    if ((Client_ID>0) && db_connected && (Session_ID>0))
         {
          QString SQLText("select public.session_exists(%1) as result;");
          QSqlQuery qu(db);
           qu.prepare(SQLText.arg(Session_ID));
               if (qu.exec())
               {
                qu.first();
                QSqlRecord rec = qu.record();
                _Result=qu.value(rec.indexOf("result")).toBool();
                 }
          }
    qDebugInfo="SessionExists(%1) have called.";
    qDebugInfo=qDebugInfo.arg(Session_ID);
    return _Result;
}

int TDllExchange::_NewSession(int tren_num, QString sIP)
{
    qDebugInfo="";
    int _Result=0;
    if ((Client_ID==3) && db_connected && (sIP.trimmed()!=""))
         { ;
          QString SQLText("select public.new_session(:IP,%1) as result;");
          QSqlQuery qu(db);
           qu.prepare(SQLText.arg(tren_num));
           qu.bindValue(":IP",sIP);
              if (qu.exec())
               {
                qu.first();
                QSqlRecord rec=qu.record();
                _Result=qu.value(rec.indexOf("result")).toInt();
               }
          }
    qDebugInfo+="NewSession(%1,\""+sIP+"\") have called. ";
    qDebugInfo=qDebugInfo.arg(tren_num);
    return _Result;
}

bool TDllExchange::_CloseSession(int Session_ID)
{
    bool _Result=false;
    if ((Client_ID>0) && db_connected && (Session_ID>0))
         {
          QString SQLText("select public.close_session(%1) as result;");
          QSqlQuery qu(db);
           qu.prepare(SQLText.arg(Session_ID));
               if (qu.exec())
               {
                qu.first();
                QSqlRecord rec=qu.record();
                _Result=qu.value(rec.indexOf("result")).toBool();
               }
          }
    qDebugInfo="CloseSession(%1) have called.";
    qDebugInfo=qDebugInfo.arg(Session_ID);
    return _Result;
}

int TDllExchange::_GetTrenBySession(int Session_ID)
{
    int _Result=0;
    if ((Client_ID>0) && db_connected && (Session_ID>0))
         {
          QString SQLText("select public.getmytrennum(%1) as result;");
          QSqlQuery qu;
           qu.prepare(SQLText.arg(Session_ID));
               if (qu.exec())
               {
                qu.first();
                QSqlRecord rec=qu.record();
                _Result=qu.value(rec.indexOf("result")).toInt();
               }
          }
    qDebugInfo="GetTrenBySession(%1) have called.";
    qDebugInfo=qDebugInfo.arg(Session_ID);
    return _Result;
}

int TDllExchange::_MyTrenNum()
{
  int _Result=_GetTrenBySession(_MySession());
  qDebugInfo="MyTrenNum() have called.";
  return _Result;
}

void TDllExchange::SlotNotify(const QString &name, QSqlDriver::NotificationSource, const QVariant &payload)
{
    const QStringList list=payload.toString().split(";");
    int cID=list.at(0).toInt();
    sID=list.at(1).toInt();
    if ((new_changes_came_event!=nullptr)
         && (name==NotifyName)
         && (cID==Client_ID))
    { new_changes_came_event(sID); }
}

//DLL

TDllExchange DllExchange;

// Внешние функции

//Возвращает подключена ли СУБД
bool dbConnected() { return DllExchange.db_connected; }

//Отключает СУБД и возвращает успешность своего действия
bool dbDisconnect() { return DllExchange._dbDisconnect(); }

/* Подключает СУБД по параметрам,
   реализована на всякий случай,
   предполагается, что вместо неё будет использоваться dbConnectIni(),
   как более простая.
   Возвращает успешность подключения */
bool dbConnect(wchar_t *dbDatabaseName,
               wchar_t *dbUserName,
               wchar_t *dbPassword,
               wchar_t *dbHostName,
               int dbPort) {

  return DllExchange._dbConnect(QString::fromWCharArray(dbDatabaseName),
                                QString::fromWCharArray(dbUserName),
                                QString::fromWCharArray(dbPassword),
                                QString::fromWCharArray(dbHostName),
                                ((dbPort==0)?5432:dbPort));
}

/* Подключает СУБД по параметрам из INI-файла.
   Возвращает успешность подключения */
bool dbConnectIni() { return DllExchange._dbConnectIni(); }

/* Возвращает указатель на строку-посылку,
   пришедшую для данной сессии и данного
   приложения (по данным регистрации в DllInit(int) */
wchar_t* GetDBDataString(int Session_ID) {

  if (vExchFromDb!=nullptr) {delete vExchFromDb;}
  QString Result=DllExchange._GetDBDataString(Session_ID);
  unsigned long ResultLength = static_cast<unsigned long>(Result.length());
  vExchFromDb = new wchar_t[ResultLength+1];
  Result.toWCharArray(vExchFromDb);
  vExchFromDb[ResultLength]=0;

  qDebugInfo="GetDBDataString(%1) have called.";
  qDebugInfo=qDebugInfo.arg(Session_ID);
  return vExchFromDb;
}

/* Прекращает сеанс работы с DLL */
bool CloseDLL() {
 DllExchange._dbDisconnect();
 Client_ID=0;
 qMyIp="";
 AmIQt=false;
 new_changes_came_event=nullptr;
 if (vExchFromDb!=nullptr) {delete vExchFromDb;}
 if (vDebugInfo!=nullptr) {delete vDebugInfo;}
 if (DllThread!=nullptr) { DllThread->quit(); DllThread->wait(); delete DllThread; }
 if (!DllExchange.db_connected) {qDebugInfo="DLL was disconnected.";} else {qDebugInfo="ERROR! DLL was not disconnected correctly.";}
 return !DllExchange.db_connected;
}

/*Отправляет в БД строку для текущей сессии*/
bool SetDBDataString(int Session_ID, wchar_t *IncomingString) {
 return DllExchange._SetDBDataString(Session_ID,QString::fromWCharArray(IncomingString).trimmed());
}

/* Устанавливает альтернативное имя для ini-файла вместо system.ini. */
 void SetIniFileName(wchar_t *IncomingString)
{
 IniFileName=QString::fromWCharArray(IncomingString).trimmed();
 qDebugInfo="SetIniFileName(\""+IniFileName+"\") have called.";
}

/*Возвращает номер сессии для указанного IP-адреса.
  IP-адрес передавать в виде указателя на нуль-терминированную строку,
  содержащую его.*/
int GetIpSession(wchar_t *IncomingString) { return DllExchange._GetIpSession(QString::fromWCharArray(IncomingString).trimmed());}

/* Если приложение зарегистрировалось в качестве отрисовщика графики,
   то функция вернёт номер его сессии, в противном случае, или
   в случае, если сессия не была открыта - 0 */
int MySession() { return DllExchange._MySession(); }

//Специальная инициализация при вызове из Qt-проека
bool InitDLL_Qt(int Id){
 AmIQt=true;
 return InitDLL(Id);
}

/* Инициализация DLL
Обязательно вызывать перед началом работы,
передав свой "номер приложения".*/
bool InitDLL(int Id)
{
 qDebugInfo="InitDLL() have called.";
 if ((Client_ID==0) && DllExchange._dbDisconnect())
 {
 //Глобальный ID программы
 Client_ID=Id;
 //DB
 //Определяем собственный IP-адрес
 QList<QHostAddress> list = QNetworkInterface::allAddresses();
 for(int nIter=0; nIter<list.count(); nIter++)
 {
 if(!list[nIter].isLoopback())
 if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol)
  {qMyIp=list[nIter].toString();
  break;}
 }
 //Проверяем, не уточнён ли он в конфиге (на случай, если у хоста много IP)
 QString IniFile;
 #ifdef Q_OS_WIN
  IniFile=QDir::currentPath();
 #else
  IniFile=QDir::homePath()+"/"+qLinAppFolder;
 #endif
 IniFile+="/"+IniFileName;
 QSettings conf(IniFile, QSettings::IniFormat);
 qMyIp=conf.value("NET/MyIp",qMyIp).toString();
 /*Если IP совсем не удалось получить, считаем себя локалхостом.
  Например, если сетевая карта отключена, а в ini-файле нет данных.*/
 if (qMyIp.trimmed()=="") qMyIp="127.0.0.1";
 qDebugInfo="InitDLL(): Client_ID=%1; IP="+qMyIp;
 qDebugInfo=qDebugInfo.arg(Client_ID);
 if (!AmIQt) DllThread = new TDllThread;
 return true;
 }
 else {
        qDebugInfo="InitDLL(%1): ERROR. You must close an old initialization before.";
        qDebugInfo=qDebugInfo.arg(Id);
        return false;
      }
}

//Прикрепление пользовательской функции к коллбеку.
void SetNewChangesCameEvent(void (*evt)(int))
{
 new_changes_came_event=static_cast<void (*)(int)>(*evt);
 qDebugInfo="SetNewChangesCameEvent(void (*)(int)) have called.";
}


//Проверяет была ли открыта сессия
bool SessionExists(int Session_ID) { return DllExchange._SessionExists(Session_ID); }

//Создаем новую сессию и возвращает её номер.
//При ошибке возвращает 0.
//Разрешается вызывать только если ты регистрировался как ПСУУП (3).
int NewSession(int tren_num, wchar_t *IncomingString) { return DllExchange._NewSession(tren_num,QString::fromWCharArray(IncomingString)); }

//Закрываем сессию
bool CloseSession(int Session_ID) { return DllExchange._CloseSession(Session_ID); }

//Выдает номер тренировки по номеру сессии
//0 - если сессии нет, или она закрыта.
int GetTrenBySession(int Session_ID) { return DllExchange._GetTrenBySession(Session_ID);}

int MyTrenNum() {return DllExchange._MyTrenNum();}

//Информация от последней выполненной функции
wchar_t *GetDebugInfo()
{
    if (vDebugInfo!=nullptr) {delete vDebugInfo;}
    unsigned long ResultLength = static_cast<unsigned long>(qDebugInfo.length());
    vDebugInfo = new wchar_t[ResultLength+1];
    qDebugInfo.toWCharArray(vDebugInfo);
    vDebugInfo[ResultLength]=0;
    return vDebugInfo;
}

