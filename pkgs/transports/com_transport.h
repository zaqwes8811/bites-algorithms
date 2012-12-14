#pragma once
//#include "../stdafx.h"
#include <windows.h>

#define PURGE_IFACE
class Ccom_transport
{
private:
  HANDLE h_Comm;
  COMMTIMEOUTS m_CommTimeouts;
  COMMTIMEOUTS m_StoreCommTimeouts;
  HANDLE hEvents[5];
    //hEvents[0] - receiv dwWaitRecBytes event, and its stored in pReceivedData
    //hEvents[1] - close receiv proc event

  // данные и контроль
  CString s_message;
  bool bEndReceiveProc;
  BYTE* pCurSendBuf;
  BYTE* pCurRecBuf;    //receive buffer
  BYTE* pReceivedData;  //data had been copied from pCurRecBuf
  BYTE* pFirstRecAdr;
  UINT uiRecBytes;
  DWORD dwMaxReceivedDataSize;  //max size of pReceivedData
  DWORD dwWaitRecBytes;      // curent size of waiting data
  UINT uiRecTimeoutSize;      // time out for return from receive waiting

  void _onEventOccure( DWORD dwRes );
  // Поток!!
  CWinThread *wtReceivProc;
  static UINT uiReceivCOMProc(LPVOID param);
public:
  DWORD numReceivedBytes;        // had read data

  void initRecEvent(HANDLE hEvent);
  void startRecProc();
  void endRecProc();
 
  void SetWaitingRecBytes(DWORD dwNewWaitBytes);
 
  //set data buf and size
  void setRecBuf(BYTE* pStartAdr);
  void SetReceivedDataBuf(BYTE* pNewRecDataBuf,DWORD dwNewMaxReceivedDataSize);

  void ResetRecv();
  void ClearRTS();

  //enamerate function
  int enumerate(CString *p);   

  // mdi send through COM rs-232
  BOOL sendByte(const BYTE* byBuf,const INT iSize); 
 
  //receive 1 byte during time X ms
  LONG ByteReceive(BYTE* byBuf,const INT iSize, BOOL &fWaitingOnRead);
  bool ClearComm();
 
  bool Init(const LPCTSTR lname, const int iRate);
  void ShutDownPort();

  // standart complect
  Ccom_transport();
  ~Ccom_transport();
#ifndef PURGE_IFACE
  void SetWaitingRecTimeout(UINT uiNewTime);
  void SetRTS();
#endif
};