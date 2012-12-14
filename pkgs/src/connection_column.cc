#include "stdafx.h"
#include "connColumn.h"
using namespace IfaceEnvelop;
using namespace System;
using namespace Threading;
using namespace NativeCode;
using namespace std;


/// <cDcPresenter>
cDcPresenter::cDcPresenter(ActorWrapper^ aw, IViewTop^ pcDcIV, Model^ model) {
  //Actor_ = pForm;
  aw_ = aw;  // �������� ��� Actor
  pcDcIV_ = pcDcIV;
  model_ = model;
  
  ///
  aw_->evStartStream += gcnew System::EventHandler<EventArgs^>(this, &IfaceEnvelop::cDcPresenter::OnStartStream);
  aw_->evStopStream += gcnew System::EventHandler<EventArgs^>(this, &IfaceEnvelop::cDcPresenter::OnStopStream);
  
}
void cDcPresenter::OnStartStream(System::Object^ sender, System::EventArgs^ e) {
  /// ����� ������ ��� �� ��� ���������� 
  wchar_t Type = pcDcIV_->getIfaceType();
  // ���������� � ��������
  string Msg;
  IfaceSettings *pifs;
  pifs = NULL;
  switch(Type) {
    /// <Ethernet>
    case 'E' : {
      pifs = new IfaceSettings(2, 2, 'u'); // ����� int
      TypeStream_ = 'u';
      // parse
      int isPort = pcDcIV_->getSPort();
      int idPort = pcDcIV_->getDPort();
      pifs->iTypeSet(0, isPort);  // ��������� � ����
      pifs->iTypeSet(1, idPort);  
      // string
      string sIP = pcDcIV_->getSIP(); 
      string dIP = pcDcIV_->getDIP(); 
      pifs->cTypeSet(0, sIP.c_str(), sIP.length()); 
      pifs->cTypeSet(1, dIP.c_str(), dIP.length()); 

      ///
      Msg = "��� - Ethernet.";
    } break;  // Ethernet
    /// <RS-232>
    case 'R' : {
      pifs = new IfaceSettings(1,1, 'c');
      TypeStream_ = 'c';
      /** 1. ��� �����  * 2. �������� ������  * 3. 'c' - COMx-���� */

      // load in transporter
      int iSpeed = pcDcIV_->getSpeed();
      pifs->iTypeSet(0, iSpeed);  // ��������� � ����

      // ��� �����
      string pn = pcDcIV_->getPortName(); 
      pifs->cTypeSet(0, pn.c_str(), pn.length()); 
      
      // for indication
      Msg = "��� - RS-232.";
    } break;
    default : {
        aw_->genMsg(static_cast<string>("����������� ����������."), 
          NEI, static_cast<string>("����� � ������������"), 1, true);
        return;
    } break;
  }
  // ��������� � ������
  // ��������� ��������� � ����������
  int Err;
  if(pifs) {
    Err = model_->Connect(pifs);  // ! � ������
    delete pifs; pifs = NULL;
    // ����������� ������
     if(Err == 0) {
       model_->Run();
       aw_->genMsg(static_cast<string>("���������� �����������."),
         SUC, "���������� �����������. " + Msg, 0, true);
    }  // ���� �� ���� �� ���� ������
    else {
      if(TypeStream_ == 'u') {
        switch(Err){  // �������� ���������� �������� ����������
          case 1 : { 
            aw_->genMsg(static_cast<string>("�� ������� ������ ����� ������."),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
          case 2 : {  // ����� ����������������
            aw_->genMsg(static_cast<string>("������ ��� �������� � IP ������."),
             FAIL, "���������� �� �����������. " + Msg, 0, false);
            aw_->genMsg(static_cast<string>("������ ����� sIP ���������� �� ���������."),
             FAIL, "���������� �� �����������. " + Msg, 0, false);
            aw_->genMsg(static_cast<string>("��������� ��������� ����������."),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
          case 3 : {
            aw_->genMsg(static_cast<string>("�� ������� ������� ����� ������"),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
          default : {
            aw_->genMsg(static_cast<string>("����������� ������"),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
        }  // switch(Er..
      }  // �� ���� ����������
      if(TypeStream_ == 'c') {  // COM
        switch(Err){
          case 1 : { 
            aw_->genMsg(static_cast<string>("�� ������� ����. �������� �� ��� ������."),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
          case 2 : {  // ����� ����������������
            aw_->genMsg(static_cast<string>("������ ������ DCB."),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
          case 3 : {
            aw_->genMsg(static_cast<string>("������ ��������� DCB."),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
          case 4 : {
            aw_->genMsg(static_cast<string>("������ ��������� ���������."),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
          default : {
            aw_->genMsg(static_cast<string>("����������� ������"),
             FAIL, "���������� �� �����������. " + Msg, 0, true);
          } break;
        }  // sw..
      }
    }
  }  // if(pi.. ���� ������ ��� ������

  // ������� ���� �� ����
  if(pifs) { delete pifs; pifs = NULL; }
}
void cDcPresenter::OnStopStream(System::Object^ sender, System::EventArgs^ e) {
  if(model_ != nullptr) 
      model_->DisConnect();  // ! � ������
  string s = "���������� ���������.";
  aw_->genMsg(s, NEI, static_cast<string>("���������� �� �������������"), 0, true);
  aw_->genMsg(s, NEI, static_cast<string>("����� � ������������"), 1, true);
}

