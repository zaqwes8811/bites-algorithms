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
  aw_ = aw;  // оболочка для Actor
  pcDcIV_ = pcDcIV;
  model_ = model;
  
  ///
  aw_->evStartStream += gcnew System::EventHandler<EventArgs^>(this, &IfaceEnvelop::cDcPresenter::OnStartStream);
  aw_->evStopStream += gcnew System::EventHandler<EventArgs^>(this, &IfaceEnvelop::cDcPresenter::OnStopStream);
  
}
void cDcPresenter::OnStartStream(System::Object^ sender, System::EventArgs^ e) {
  /// нужно узнать что за тип соединения 
  wchar_t Type = pcDcIV_->getIfaceType();
  // заполнение и загрузка
  string Msg;
  IfaceSettings *pifs;
  pifs = NULL;
  switch(Type) {
    /// <Ethernet>
    case 'E' : {
      pifs = new IfaceSettings(2, 2, 'u'); // порты int
      TypeStream_ = 'u';
      // parse
      int isPort = pcDcIV_->getSPort();
      int idPort = pcDcIV_->getDPort();
      pifs->iTypeSet(0, isPort);  // нумерация с нуля
      pifs->iTypeSet(1, idPort);  
      // string
      string sIP = pcDcIV_->getSIP(); 
      string dIP = pcDcIV_->getDIP(); 
      pifs->cTypeSet(0, sIP.c_str(), sIP.length()); 
      pifs->cTypeSet(1, dIP.c_str(), dIP.length()); 

      ///
      Msg = "Тип - Ethernet.";
    } break;  // Ethernet
    /// <RS-232>
    case 'R' : {
      pifs = new IfaceSettings(1,1, 'c');
      TypeStream_ = 'c';
      /** 1. имя порта  * 2. скорость обмена  * 3. 'c' - COMx-порт */

      // load in transporter
      int iSpeed = pcDcIV_->getSpeed();
      pifs->iTypeSet(0, iSpeed);  // нумерация с нуля

      // имя порта
      string pn = pcDcIV_->getPortName(); 
      pifs->cTypeSet(0, pn.c_str(), pn.length()); 
      
      // for indication
      Msg = "Тип - RS-232.";
    } break;
    default : {
        aw_->genMsg(static_cast<string>("Неизвестное соединение."), 
          NEI, static_cast<string>("Связь с передатчиком"), 1, true);
        return;
    } break;
  }
  // загружаем в модель
  // загружаем настройки в соединение
  int Err;
  if(pifs) {
    Err = model_->Connect(pifs);  // ! в модель
    delete pifs; pifs = NULL;
    // анализируем ошибки
     if(Err == 0) {
       model_->Run();
       aw_->genMsg(static_cast<string>("Соединение установлено."),
         SUC, "Соединение установлено. " + Msg, 0, true);
    }  // если не ноль то есть ошибки
    else {
      if(TypeStream_ == 'u') {
        switch(Err){  // проверка успешности создания соединения
          case 1 : { 
            aw_->genMsg(static_cast<string>("Не удалось создан сокет чтения."),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
          case 2 : {  // самая распространенная
            aw_->genMsg(static_cast<string>("Ощибка при привязке к IP адресу."),
             FAIL, "Соединение не установлено. " + Msg, 0, false);
            aw_->genMsg(static_cast<string>("Скорее всего sIP отличается от реального."),
             FAIL, "Соединение не установлено. " + Msg, 0, false);
            aw_->genMsg(static_cast<string>("Проверьте настройки соединения."),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
          case 3 : {
            aw_->genMsg(static_cast<string>("Не удалось создать сокет записи"),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
          default : {
            aw_->genMsg(static_cast<string>("Неизвестная ошибка"),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
        }  // switch(Er..
      }  // по типу соединения
      if(TypeStream_ == 'c') {  // COM
        switch(Err){
          case 1 : { 
            aw_->genMsg(static_cast<string>("Не открыть порт. Возможно он уже открыт."),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
          case 2 : {  // самая распространенная
            aw_->genMsg(static_cast<string>("Ошибка чтения DCB."),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
          case 3 : {
            aw_->genMsg(static_cast<string>("Ошибка установки DCB."),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
          case 4 : {
            aw_->genMsg(static_cast<string>("Ошибка установки таймаутов."),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
          default : {
            aw_->genMsg(static_cast<string>("Неизвестная ошибка"),
             FAIL, "Соединение не установлено. " + Msg, 0, true);
          } break;
        }  // sw..
      }
    }
  }  // if(pi.. если объект был создан

  // удаляем если не ноль
  if(pifs) { delete pifs; pifs = NULL; }
}
void cDcPresenter::OnStopStream(System::Object^ sender, System::EventArgs^ e) {
  if(model_ != nullptr) 
      model_->DisConnect();  // ! в модель
  string s = "Соединение разорвано.";
  aw_->genMsg(s, NEI, static_cast<string>("Соединение не производилось"), 0, true);
  aw_->genMsg(s, NEI, static_cast<string>("Связь с передатчиком"), 1, true);
}

