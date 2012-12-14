// file : #include "connColumn.h"
#pragma once 
#include "NativeCode.h"
#include "TypConv.h"
#include "hardIface.h"
#include "Model.h"
namespace IfaceEnvelop {
  const int  FAIL = -1;  // провел
  const int  NEI = 0;  // нейтральное 
  const int  SUC = 1;  // успешно
/// <IcDcIView>
   public ref class IViewTop {
   public : virtual wchar_t getIfaceType(void) {return 0;}
   // получени настроек
   /// RS-232
   public : virtual std::string getPortName() {return 0;}
   public : virtual int getSpeed() {return 0;}
   /// Udp
   public : virtual std::string getSIP(void) {return 0;}
   public : virtual std::string getDIP(void) {return 0;}
   public : virtual int getSPort(void) {return 0;}
   public : virtual int getDPort(void) {return 0;}
   };
   ///  <интерфес к актору>
///
   public ref class ActorWrapper {
   public : event System::EventHandler<System::EventArgs^>^ evStartStream;  // вот кстати к нему может 
   public : event System::EventHandler<System::EventArgs^>^ evStopStream;  // вот кстати к нему может 
   //public : event System::EventHandler<System::EventArgs^>^ evTick;
   // скоростные сообщения
   public : virtual void genMsg(std::string &s, int success, std::string &m, int num, bool) {}

   };

/// <cDcPresenter>
/// </cDcPresenter>
    public ref class cDcPresenter {
    private :ActorWrapper^ aw_;  // можно ли вообще так делать то
    private : IViewTop^ pcDcIV_;
    private : Model^ model_;
    public : cDcPresenter(ActorWrapper^ aw, IViewTop^ pcDcIV, Model^ model);
    private : char TypeStream_;
    // OnEvent
    public : void OnStartStream(System::Object^ sender, System::EventArgs^ e);
    public : void OnStopStream(System::Object^ sender, System::EventArgs^ e);

   };


};