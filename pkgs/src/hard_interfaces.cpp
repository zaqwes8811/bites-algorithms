#include "StdAfx.h"
#include "hardIface.h"
using namespace NativeCode;

/// <RS232>
/// hardIfaceRS232::
/// </RS232>
/// коструктор и деструктор реализуют часть RAII
hardIfaceRS232::hardIfaceRS232(OVERLAPPED* polwr, OVERLAPPED* polrd) {  
  polwr_ = polwr;  // сохраняем ссылки
  polrd_ = polrd;  
  hPort_ = NULL;  // лучше обнулить
       
  /// ! захват ресурсов ! без проверки на результат !
  polwr_->hEvent = CreateEvent(NULL, true, true, NULL);
  polrd_->hEvent = CreateEvent(NULL, true, true, NULL);  // сигнальное
  InitializeCriticalSection(&csTx_);  // захватываем критическую секцию
  InitializeCriticalSection(&csRx_);
  maxReadOnceRs_ = 200;
}
hardIfaceRS232::~hardIfaceRS232() {
  Remove();  // закрываем порт
  if(polwr_->hEvent) {
    CloseHandle(polwr_->hEvent); }
  if(polrd_->hEvent) {
    CloseHandle(polrd_->hEvent); }
    // может тоже нужно подождать
  // other
  DeleteCriticalSection(&csTx_);  // удаляем критическую секции
  DeleteCriticalSection(&csRx_);  
}
/// создание соединения
int hardIfaceRS232::Create(IfaceSettings* pifs) {
   
  /// open COM-port
	hPort_ = CreateFileA(pifs->cTypeGet(0),GENERIC_READ | GENERIC_WRITE, 0,
	  NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
  if(hPort_ == INVALID_HANDLE_VALUE) { 
    return 1; 
  }  // ERR1
  /// порт открыт. Теперь настройка
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
  // таймауты ! должны быть осмысленные ! для любых сделать очень проблематично
  COMMTIMEOUTS timeouts_;  	// структура для установки таймаутов
  timeouts_.ReadIntervalTimeout = 150;  // таймаут между двумя символами 
	timeouts_.ReadTotalTimeoutMultiplier = 30;  // общий таймаут операции чтения 
	timeouts_.ReadTotalTimeoutConstant = 100;  // константа для общего таймаута операции чтения 
	timeouts_.WriteTotalTimeoutMultiplier = 0;  // общий таймаут операции записи 
	timeouts_.WriteTotalTimeoutConstant = 0;  // константа для общего таймаута операции записи 
	if(!SetCommTimeouts(hPort_, &timeouts_)) {   
	  if(hPort_) { CloseHandle(hPort_);  hPort_ = NULL; }
		return 4; 
  }  // Err4
  SetCommMask(hPort_, EV_RXCHAR);  // установить маску на срабатывание по событию 
    // приёма байта в порт  
  /// только здесь утверждаем, что соединение создано
  setTxState(true);
  setRxState(true);
	return 0;
}
/// разрыв соединения
void hardIfaceRS232::Remove(void) {
  // настраиваемся на завершение потоков
  setTxState(false);
  setRxState(false);

  // имитация событий
  SetEvent(polrd_->hEvent);  // имитация приема
  Sleep(200);  // лучше немного подождать, пока потоки завершаться

  // закрываем порт
  if(hPort_) { CloseHandle(hPort_);  hPort_ = NULL; }
}  

/// Функции работающие в потоках
int hardIfaceRS232::Tx(simpleCmd* scTx) {
  DWORD signal, temp;
  int TxOut = 0;
  ///
  bool locbTx;
  EnterCriticalSection(&csTx_);
  locbTx = bTxMT_;  // читаем можно ли завершить поток
  LeaveCriticalSection(&csTx_);  
  if(locbTx) {  // поток работает, а значить, если дошло до сюда, то и бланки действительны
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
    }  // если указатель не нулевой
  }  // if(locb..
  else {
    TxOut = 1;  // завершаем процесс
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
  if(locbRx) {  // если разрешена работа
    if(scRx) {
      DWORD temp, mask, signal;
      PurgeComm(hPort_, PURGE_TXCLEAR);
      WaitCommEvent(hPort_, &mask, polrd_);
      signal = WaitForSingleObject(polrd_->hEvent, INFINITE);	
      if(signal == WAIT_OBJECT_0) {  // дождались 
        if(GetOverlappedResult(hPort_, polrd_, &temp, true)) { 
          if((mask & EV_RXCHAR) != 0) {  // что-то пришло 
            /// само чтение - принимаем все разом, таймауты должны покр
              // ыть весь пакет комманд
            ReadFile(hPort_, scRx->getCmdPtr(0), 
               maxReadOnceRs_, &temp, polrd_);   
            GetOverlappedResult(hPort_, polrd_, &temp, true);  
            lenRxCmd = temp;  // 0 и более - что приняли
            scRx->setCmdLen(0, temp);  // сохраняем число принятых байт
            if(lenRxCmd != 0)
              RxOut = 0;  // продолжаем поток  
            else
              RxOut = 3;  // холостой ход 
          }
        }  // if(GetOverlappe...
      }  // if(signa...
    }  // если указатель ненулевой
    else  RxOut = 2;  // 
  } else {  // завершаем поток
    RxOut = 1;  
  }
  return RxOut;
}  // Выжно! таймауты должны быть строго настроены, вобщем их проще настроить
   // если все комманды потока будут приняты разом, или часть

/// многопоточная чать
void hardIfaceRS232::setTxState(bool TxFlag) {
    EnterCriticalSection(&csTx_); bTxMT_ = TxFlag;
    LeaveCriticalSection(&csTx_); }
///
void hardIfaceRS232::setRxState(bool RxFlag) {
    EnterCriticalSection(&csRx_); bRxMT_ = RxFlag;
    LeaveCriticalSection(&csRx_); }
/// <UDP>
/// hardIfaceUdp::
/// </UDP>

int hardIfaceUdp::WINSOCK_VERSION = 0x0101;  // init static
hardIfaceUdp::hardIfaceUdp(OVERLAPPED* polwr, OVERLAPPED* polrd){
  // other
  InitializeCriticalSection(&csTx_);  // захватываем критическую секцию
  InitializeCriticalSection(&csRx_);
  maxReadOnceUdp_ = 200;
}
 
hardIfaceUdp::~hardIfaceUdp(){
  ///
  if(iSockRx_) { closesocket(iSockRx_); }
  if(iSockTx_) { closesocket(iSockRx_); }
  // other
  DeleteCriticalSection(&csTx_);  // удаляем критическую секции
  DeleteCriticalSection(&csRx_); 
}
int hardIfaceUdp::Create(IfaceSettings* pifs) {
  iResult_ = WSAStartup(WINSOCK_VERSION, &wsaData_);
  if (iResult_ != NO_ERROR) { WSACleanup(); return 4; }
  ///
  iSockRx_ = 0;  // лучше в конструеторе, но пока так
  iSockTx_ = 0;

  /// "распиновка"
  unsigned short port = pifs->iTypeGet(0);
  unsigned short dport = pifs->iTypeGet(1);
  const char* const sipaddr = pifs->cTypeGet(0);
  const char* const dipaddr = pifs->cTypeGet(1);

  /// Rx ///
  iSockRx_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(iSockRx_ >= 0) {  // сокет создан
    addrRx_.sin_family = AF_INET;
    addrRx_.sin_port = htons(port);
    addrRx_.sin_addr.s_addr = inet_addr(sipaddr);
    // связываем 
    if(bind(iSockRx_, (struct sockaddr *)&addrRx_, sizeof(addrRx_)) < 0) {
      return 2;  // ощибка связывания bind()
    }
  } 
  else { return 1; }  // не создан сокет чтения
  
  /// Tx ///
  iSockTx_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(iSockTx_ >= 0) {  // по карйней мере создали
    addrTx_.sin_family = AF_INET;
    addrTx_.sin_port = htons(dport);
    addrTx_.sin_addr.s_addr = inet_addr(dipaddr);
  } 
  else { return 3; } // сокет записи не создан

  // разрашаем работу потоков
  setTxState(true);  
  setRxState(true);

  // все прошло успешно, раз мы здесь
  return 0;       
}
/// разрыв соединения
void hardIfaceUdp::Remove(void) {
  /// останавливаем потоки, вернее меняем флаги
  setTxState(false);
  setRxState(false);
  ///
  if(iSockRx_) {
    closesocket(iSockRx_);  // если был заблокирован, то вернет -1
    iSockRx_ = 0;}
  if(iSockTx_) {
    closesocket(iSockTx_);
    iSockTx_ = 0; }
  /// зактываем библиотеку
  WSACleanup();
}

/// методы запускаемые в потоках
int hardIfaceUdp::Rx(simpleCmd* scRx) {
  int RxOut = 0;
  int lenRxCmd = 0;
  bool locbRx;
  EnterCriticalSection(&csRx_);
  locbRx = bRxMT_;
  LeaveCriticalSection(&csRx_); 
  if(locbRx) {
    if(scRx) {
      // читаем, сокет блокирующий, поэтому пока он спить
      lenRxCmd = recvfrom(iSockRx_, scRx->getCmdPtr(0), maxReadOnceUdp_, 0, NULL, NULL);
      ///
      scRx->setCmdLen(0, lenRxCmd);  // сохраняем число принятых байт
      if(lenRxCmd >= 0) RxOut = 0;
      else RxOut = -1; 
      ///
    }  // if pointer no zero
  }
  else {
      RxOut = 1;  // выходим из потока
  }  
  ///
  return RxOut;
}
///
int hardIfaceUdp::Tx(simpleCmd* scTx) {
  int err = 0;
  int TxOut = 0;
  bool locbTx;
  EnterCriticalSection(&csTx_);
  locbTx = bTxMT_;
  LeaveCriticalSection(&csTx_);  
  if(locbTx) {  // если поток завершается никаких обращений к бланку нет
    if(scTx) {
      int NumCmd = (scTx->getNumCmd());
      for(int i = 0; i < NumCmd; i++) {
        int LenCmd = scTx->getCmdLen(i);
        int err1 = sendto(iSockTx_, scTx->getCmdPtr(i), LenCmd, 0,(struct sockaddr *)&addrTx_,  sizeof(addrTx_)); 
        if(err1 != -1) {  // ошибок нет
          TxOut = 0;
        } 
        else 
        { TxOut = 2; }
      }  // for(int..
    }  // если указатель ненулевой
  }
  else {
    TxOut = 1;  // завершаем процесс
  }
  return TxOut;
}

/// многопоточная часть
void hardIfaceUdp::setTxState(bool TxFlag) {
    EnterCriticalSection(&csTx_); bTxMT_ = TxFlag;
    LeaveCriticalSection(&csTx_); }
///
void hardIfaceUdp::setRxState(bool RxFlag) {
    EnterCriticalSection(&csRx_); bRxMT_ = RxFlag;
    LeaveCriticalSection(&csRx_); }
///

