#include "StdAfx.h"
#include "hardIface.h"
using namespace NativeCode;

/// <RS232>
/// hardIfaceRS232::
/// </RS232>
/// ���������� � ���������� ��������� ����� RAII
hardIfaceRS232::hardIfaceRS232(OVERLAPPED* polwr, OVERLAPPED* polrd) {  
  polwr_ = polwr;  // ��������� ������
  polrd_ = polrd;  
  hPort_ = NULL;  // ����� ��������
       
  /// ! ������ �������� ! ��� �������� �� ��������� !
  polwr_->hEvent = CreateEvent(NULL, true, true, NULL);
  polrd_->hEvent = CreateEvent(NULL, true, true, NULL);  // ����������
  InitializeCriticalSection(&csTx_);  // ����������� ����������� ������
  InitializeCriticalSection(&csRx_);
  maxReadOnceRs_ = 200;
}
hardIfaceRS232::~hardIfaceRS232() {
  Remove();  // ��������� ����
  if(polwr_->hEvent) {
    CloseHandle(polwr_->hEvent); }
  if(polrd_->hEvent) {
    CloseHandle(polrd_->hEvent); }
    // ����� ���� ����� ���������
  // other
  DeleteCriticalSection(&csTx_);  // ������� ����������� ������
  DeleteCriticalSection(&csRx_);  
}
/// �������� ����������
int hardIfaceRS232::Create(IfaceSettings* pifs) {
   
  /// open COM-port
	hPort_ = CreateFileA(pifs->cTypeGet(0),GENERIC_READ | GENERIC_WRITE, 0,
	  NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
  if(hPort_ == INVALID_HANDLE_VALUE) { 
    return 1; 
  }  // ERR1
  /// ���� ������. ������ ���������
  DCB dcb;
  dcb.DCBlength = sizeof(DCB); 	
  if(!GetCommState(hPort_, &dcb)) {
    if(hPort_) { CloseHandle(hPort_);  hPort_ = NULL; return 2;}
  } // Err2
  dcb.BaudRate = pifs->iTypeGet(0);       
  dcb.fBinary = TRUE; dcb.fOutxCtsFlow = FALSE;                             
  dcb.fOutxDsrFlow = FALSE; dcb.fDtrControl = DTR_CONTROL_DISABLE;                 
  dcb.fDsrSensitivity = FALSE; dcb.fNull = FALSE;                                     
  dcb.fRtsControl = RTS_CONTROL_DISABLE;  dcb.fAbortOnError = FALSE;                             
  dcb.ByteSize = 8; dcb.Parity = 0;                                        
  dcb.StopBits = 0;                                      
  if(!SetCommState(hPort_, &dcb))	{ 
    if(hPort_) { CloseHandle(hPort_);  hPort_ = NULL; }
    return 3; 
  } // Err3
  // �������� ! ������ ���� ����������� ! ��� ����� ������� ����� �������������
  COMMTIMEOUTS timeouts_;  	// ��������� ��� ��������� ���������
  timeouts_.ReadIntervalTimeout = 150;  // ������� ����� ����� ��������� 
	timeouts_.ReadTotalTimeoutMultiplier = 30;  // ����� ������� �������� ������ 
	timeouts_.ReadTotalTimeoutConstant = 100;  // ��������� ��� ������ �������� �������� ������ 
	timeouts_.WriteTotalTimeoutMultiplier = 0;  // ����� ������� �������� ������ 
	timeouts_.WriteTotalTimeoutConstant = 0;  // ��������� ��� ������ �������� �������� ������ 
	if(!SetCommTimeouts(hPort_, &timeouts_)) {   
	  if(hPort_) { CloseHandle(hPort_);  hPort_ = NULL; }
		return 4; 
  }  // Err4
  SetCommMask(hPort_, EV_RXCHAR);  // ���������� ����� �� ������������ �� ������� 
    // ����� ����� � ����  
  /// ������ ����� ����������, ��� ���������� �������
  setTxState(true);
  setRxState(true);
	return 0;
}
/// ������ ����������
void hardIfaceRS232::Remove(void) {
  // ������������� �� ���������� �������
  setTxState(false);
  setRxState(false);

  // �������� �������
  SetEvent(polrd_->hEvent);  // �������� ������
  Sleep(200);  // ����� ������� ���������, ���� ������ �����������

  // ��������� ����
  if(hPort_) { CloseHandle(hPort_);  hPort_ = NULL; }
}  

/// ������� ���������� � �������
int hardIfaceRS232::Tx(simpleCmd* scTx) {
  DWORD signal, temp;
  int TxOut = 0;
  ///
  bool locbTx;
  EnterCriticalSection(&csTx_);
  locbTx = bTxMT_;  // ������ ����� �� ��������� �����
  LeaveCriticalSection(&csTx_);  
  if(locbTx) {  // ����� ��������, � �������, ���� ����� �� ����, �� � ������ �������������
    if(scTx) {  //
      int NumCmd = (scTx->getNumCmd());
      int LenCmd = 0;
      for(int i = 0; i < NumCmd; i++) {
        DWORD LenCmd = scTx->getCmdLen(i);
        WriteFile(hPort_, scTx->getCmdPtr(i), LenCmd, &temp, polwr_);
        signal = WaitForSingleObject(polwr_->hEvent, 2000);	
        char* p = scTx->getCmdPtr(i);
        char pp = p[0];
        if((signal == WAIT_OBJECT_0) && 
           (GetOverlappedResult(hPort_, polwr_, &temp, true))) {
          TxOut = 0;
        } 
        //Sleep(40);
      }  // for(int..
    }  // ���� ��������� �� �������
  }  // if(locb..
  else {
    TxOut = 1;  // ��������� �������
  }
  return TxOut;
}
///
int hardIfaceRS232::Rx(simpleCmd* scRx) {
  int RxOut = 0;
  int lenRxCmd = 0;
  bool locbRx;
  EnterCriticalSection(&csRx_);
  locbRx = bRxMT_;
  LeaveCriticalSection(&csRx_);
  if(locbRx) {  // ���� ��������� ������
    if(scRx) {
      DWORD temp, mask, signal;
      PurgeComm(hPort_, PURGE_TXCLEAR);
      WaitCommEvent(hPort_, &mask, polrd_);
      signal = WaitForSingleObject(polrd_->hEvent, INFINITE);	
      if(signal == WAIT_OBJECT_0) {  // ��������� 
        if(GetOverlappedResult(hPort_, polrd_, &temp, true)) { 
          if((mask & EV_RXCHAR) != 0) {  // ���-�� ������ 
            /// ���� ������ - ��������� ��� �����, �������� ������ ����
              // ��� ���� ����� �������
            ReadFile(hPort_, scRx->getCmdPtr(0), 
               maxReadOnceRs_, &temp, polrd_);   
            GetOverlappedResult(hPort_, polrd_, &temp, true);  
            lenRxCmd = temp;  // 0 � ����� - ��� �������
            scRx->setCmdLen(0, temp);  // ��������� ����� �������� ����
            if(lenRxCmd != 0)
              RxOut = 0;  // ���������� �����  
            else
              RxOut = 3;  // �������� ��� 
          }
        }  // if(GetOverlappe...
      }  // if(signa...
    }  // ���� ��������� ���������
    else  RxOut = 2;  // 
  } else {  // ��������� �����
    RxOut = 1;  
  }
  return RxOut;
}  // �����! �������� ������ ���� ������ ���������, ������ �� ����� ���������
   // ���� ��� �������� ������ ����� ������� �����, ��� �����

/// ������������� ����
void hardIfaceRS232::setTxState(bool TxFlag) {
    EnterCriticalSection(&csTx_); bTxMT_ = TxFlag;
    LeaveCriticalSection(&csTx_); }
///
void hardIfaceRS232::setRxState(bool RxFlag) {
    EnterCriticalSection(&csRx_); bRxMT_ = RxFlag;
    LeaveCriticalSection(&csRx_); }


