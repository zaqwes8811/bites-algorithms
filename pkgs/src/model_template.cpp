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
  
  /// ������� ������
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
  
  /// ������� ������
  scTx_ = new simpleCmd(numWrCmd, maxWrLen);  // ����� ������� � �� ������������ ����� 
  scRx_ = new simpleCmd(numRdCmd, maxRdLen); 
  Main_ = Main;  // ������������ ���������� �������
  
}
Model::!Model() {
  if(thwr_) Model::DisConnect();
  if(thr_) Model::DisConnect();
  if(evForWrite_)  { SetEvent(evForWrite_);  Sleep(100);  CloseHandle(evForWrite_); }
  if(evReadEnd_) { SetEvent(evReadEnd_); Sleep(100);  CloseHandle(evReadEnd_); }
  
}


/// <����������>
int Model::Connect(IfaceSettings* pifs) {
  if(hift_) { 
    delete hift_;	 
    hift_ = NULL; }  // �����������������
  /// ����� �������. ������������ ����������� ///
  char tc = pifs->getTypeConnect();
  switch(tc) { // ������ ��� ����������
    case 'c' : {  // COM-port
      hift_ = new hardIfaceRS232(polwr_, polrd_); 
      TypeStream_ = 'c';         
               } break;
    case 'u' : {  // UDP
      hift_ = new hardIfaceUdp(polwr_, polrd_);  TypeStream_ = 'u';} break;
    default : {  // �� ���� ��� (������ ������������� ���������)
    } break;
  } 
  
  /// connection creating
  int Err = hift_->Create(pifs);  // ����� ������� ����������
  // ������� ���������� ����������
  if(Err == 0) // ���� ��� ������
  { 
    bool value = true;
    try { Monitor::Enter(this); ConnectActiveMT_ = value; }
    finally { Monitor::Exit(this);}
  }
  /// ������� ������� �����
  evReadEnd_ = CreateEvent(NULL, true, true, NULL);  // ����������
  evForWrite_ = CreateEvent(NULL, true, false, NULL);  // ������������
  ///
  return Err;
}
/// ������ �������
void Model::Run() {
  thr_ = gcnew Thread(gcnew ThreadStart(this, &IfaceEnvelop::Model::fthRead_));
  thr_->Name = "RxThread";
  thr_->Start();  // ����� ������
  Thread::Sleep(0);
  thwr_ = gcnew Thread(gcnew ThreadStart(this, &IfaceEnvelop::Model::fthWrite_));
  thwr_->Name = "TxThread";
  thwr_->Start();
  Thread::Sleep(0); 
}
/// ������ ����������
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
  // ���������� �������� ����������
  
  bool value = false;
  try {Monitor::Enter(this); ConnectActiveMT_ = value; } 
  finally { Monitor::Exit(this); }

  ///
  return 0;
}

/// ������� ����������� � �������
void Model::fthWrite_(void) {
  int TxOut = 0;  // ���� ������ ������� ������
  DWORD signal;
  while(1) {
    signal = WaitForSingleObject(evForWrite_, INFINITE);
    ResetEvent(evForWrite_);
    // ����� ��� ����������� ������
    TxOut = hift_->Tx(scTx_);  // ������ ������ 
    if(TxOut == 1){ break;}
  }
}
void Model::fthRead_(void) {
  int RxOut = 0;  // ���� ������ ������� ������
  DWORD signal;
  while(1) {
    // ���� �������
    if(Main_) {
      signal = WaitForSingleObject(evForWrite_, INFINITE);  // �������� ��������, ���� �����
      ResetEvent(evForWrite_); 
    }

    // ����� ������, ������ ����� � ��������������
    RxOut = hift_->Rx(scRx_);  // ������
    if((RxOut == 1)) { break; }  // ��������� �����
    
    // ���������
    if((RxOut != 2) && (RxOut != 3)) {
      ResetEvent(evReadEnd_);  // ��������
      evViewUbdate(this, gcnew EventArgs());
      signal = WaitForSingleObject(evReadEnd_, 10000);  // � ������� ������ ����������
    }
  }  // while(..
}

/// �������� ��������
int Model::Send(void) {
  if((thwr_ != nullptr) && (thwr_ != nullptr))
    SetEvent(evForWrite_);  // ��������� ������, ����� ������ �� ������, ������ �� ������� ������������
  return 0;
}

/// �������������
bool Model::getActive(void) {  // ��������� ������� ���������� ����������
  bool value = false;
  try { Monitor::Enter(this); value = ConnectActiveMT_; }
  finally {  Monitor::Exit(this); }
  return value;
}