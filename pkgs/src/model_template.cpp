#include "stdafx.h"
#include "Model.h"
using namespace IfaceEnvelop;
using namespace System;
using namespace Threading;
using namespace NativeCode;

/// <Model>
/// </Model>
Model::~Model(void) { 
  DisConnect();
  Thread::Sleep(100);
  if(hift_) { delete hift_; hift_ = NULL; }
  
  /// удаляем бланки
  if(scRx_) { delete scRx_; scRx_ = NULL;}
  if(scTx_) { delete scTx_; scTx_ = NULL;}
  
}
void Model::ReadComplete(void) {
             SetEvent(evReadEnd_);
           }
Model::Model(OVERLAPPED* polwr, OVERLAPPED* polrd, int numWrCmd, int maxWrLen, int numRdCmd, int maxRdLen, bool Main) {
  hift_ = NULL;
  polwr_ = polwr;
  polrd_ = polrd;
  
  /// создаем бланки
  scTx_ = new simpleCmd(numWrCmd, maxWrLen);  // число комманд и их максимальные длины 
  scRx_ = new simpleCmd(numRdCmd, maxRdLen); 
  Main_ = Main;  // конфигурация начального запуска
  
}
Model::!Model() {
  if(thwr_) Model::DisConnect();
  if(thr_) Model::DisConnect();
  if(evForWrite_)  { SetEvent(evForWrite_);  Sleep(100);  CloseHandle(evForWrite_); }
  if(evReadEnd_) { SetEvent(evReadEnd_); Sleep(100);  CloseHandle(evReadEnd_); }
  
}


/// <Соединение>
int Model::Connect(IfaceSettings* pifs) {
  if(hift_) { 
    delete hift_;	 
    hift_ = NULL; }  // переинициализация
  /// Здесь использ. динамический полиморфизм ///
  char tc = pifs->getTypeConnect();
  switch(tc) { // узнали тип соединения
    case 'c' : {  // COM-port
      hift_ = new hardIfaceRS232(polwr_, polrd_); 
      TypeStream_ = 'c';         
               } break;
    case 'u' : {  // UDP
      hift_ = new hardIfaceUdp(polwr_, polrd_);  TypeStream_ = 'u';} break;
    default : {  // Не ясно что (ошибка инициализации стркутуры)
    } break;
  } 
  
  /// connection creating
  int Err = hift_->Create(pifs);  // сразу создаем соединение
  // признак активности соединения
  if(Err == 0) // если все хорошо
  { 
    bool value = true;
    try { Monitor::Enter(this); ConnectActiveMT_ = value; }
    finally { Monitor::Exit(this);}
  }
  /// события создаем здесь
  evReadEnd_ = CreateEvent(NULL, true, true, NULL);  // сигнальное
  evForWrite_ = CreateEvent(NULL, true, false, NULL);  // несигнальное
  ///
  return Err;
}
/// запуск потоков
void Model::Run() {
  thr_ = gcnew Thread(gcnew ThreadStart(this, &IfaceEnvelop::Model::fthRead_));
  thr_->Name = "RxThread";
  thr_->Start();  // поток чтения
  Thread::Sleep(0);
  thwr_ = gcnew Thread(gcnew ThreadStart(this, &IfaceEnvelop::Model::fthWrite_));
  thwr_->Name = "TxThread";
  thwr_->Start();
  Thread::Sleep(0); 
}
/// разрыв соединения
int Model::DisConnect() {
  if(hift_) { hift_->Remove(); }
  if(evForWrite_)  {
    SetEvent(evForWrite_);  CloseHandle(evForWrite_); evForWrite_ = NULL;
  }
  if(evReadEnd_) {
    SetEvent(evReadEnd_); Sleep(200); CloseHandle(evReadEnd_);
      evReadEnd_ = NULL;
      Sleep(100);}
  ///
  if(hift_) { delete hift_;
  hift_ = NULL;}
  // соединение признаем неактивным
  
  bool value = false;
  try {Monitor::Enter(this); ConnectActiveMT_ = value; } 
  finally { Monitor::Exit(this); }

  ///
  return 0;
}

/// Функции запускаемые в потоках
void Model::fthWrite_(void) {
  int TxOut = 0;  // итог работы функции приема
  DWORD signal;
  while(1) {
    signal = WaitForSingleObject(evForWrite_, INFINITE);
    ResetEvent(evForWrite_);
    // выход при завершеннии потока
    TxOut = hift_->Tx(scTx_);  // всегда входит 
    if(TxOut == 1){ break;}
  }
}
void Model::fthRead_(void) {
  int RxOut = 0;  // итог работы функции приема
  DWORD signal;
  while(1) {
    // ждем сигнала
    if(Main_) {
      signal = WaitForSingleObject(evForWrite_, INFINITE);  // комманда отослана, ждем ответ
      ResetEvent(evForWrite_); 
    }

    // можно читать, адреса новые и действительные
    RxOut = hift_->Rx(scRx_);  // чтение
    if((RxOut == 1)) { break; }  // завершаем поток
    
    // обработка
    if((RxOut != 2) && (RxOut != 3)) {
      ResetEvent(evReadEnd_);  // заслонка
      evViewUbdate(this, gcnew EventArgs());
      signal = WaitForSingleObject(evReadEnd_, 10000);  // в обычном режиме прозрачная
    }
  }  // while(..
}

/// отпарвка комманды
int Model::Send(void) {
  if((thwr_ != nullptr) && (thwr_ != nullptr))
    SetEvent(evForWrite_);  // спусковой крючок, здесь только на запись, чтение по другому организовано
  return 0;
}

/// многопоточные
bool Model::getActive(void) {  // получение степени активности соединения
  bool value = false;
  try { Monitor::Enter(this); value = ConnectActiveMT_; }
  finally {  Monitor::Exit(this); }
  return value;
}