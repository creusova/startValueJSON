#ifndef EXCH_H
#define EXCH_H

extern "C" {
 bool InitDLL(int);
 bool InitDLL_Qt(int);
 bool dbConnected();
 bool dbConnect(wchar_t *dbDatabaseName,
                wchar_t *dbUserName,
                wchar_t *dbPassword,
                wchar_t *dbHostName,
                int dbPort);
 bool dbConnectIni();
 bool dbDisconnect();
 bool CloseDLL();
 wchar_t* GetDBDataString(int);
 bool SetDBDataString(int, wchar_t *IncomingString);
 void SetIniFileName(wchar_t *IncomingString);
 void SetNewChangesCameEvent(void (*evt)(int));
 int GetIpSession(wchar_t *IncomingString);
 int MySession();
 bool SessionExists(int);
 int NewSession(int, wchar_t *IncomingString);
 bool CloseSession(int);
 int GetTrenBySession(int);
 int MyTrenNum();
 wchar_t* GetDebugInfo();
}



#endif // EXCH_H

