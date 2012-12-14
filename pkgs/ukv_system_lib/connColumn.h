// file : #include "connColumn.h"
#pragma once 
#include "NativeCode.h"
#include "TypConv.h"
#include "hardIface.h"
#include "Model.h"
namespace IfaceEnvelop {
  const int  FAIL = -1;  // ������
  const int  NEI = 0;  // ����������� 
  const int  SUC = 1;  // �������
/// <IcDcIView>
   public ref class IViewTop {
   public : virtual wchar_t getIfaceType(void) {return 0;}
   // �������� ��������
   /// RS-232
   public : virtual std::string getPortName() {return 0;}
   public : virtual int getSpeed() {return 0;}
   /// Udp
   public : virtual std::string getSIP(void) {return 0;}
   public : virtual std::string getDIP(void) {return 0;}
   public : virtual int getSPort(void) {return 0;}
   public : virtual int getDPort(void) {return 0;}
   };
   ///  <�������� � ������>
///
   public ref class ActorWrapper {
   public : event System::EventHandler<System::EventArgs^>^ evStartStream;  // ��� ������ � ���� ����� 
   public : event System::EventHandler<System::EventArgs^>^ evStopStream;  // ��� ������ � ���� ����� 
   //public : event System::EventHandler<System::EventArgs^>^ evTick;
   // ���������� ���������
   public : virtual void genMsg(std::string &s, int success, std::string &m, int num, bool) {}

   };

/// <cDcPresenter>
/// </cDcPresenter>
    public ref class cDcPresenter {
    private :ActorWrapper^ aw_;  // ����� �� ������ ��� ������ ��
    private : IViewTop^ pcDcIV_;
    private : Model^ model_;
    public : cDcPresenter(ActorWrapper^ aw, IViewTop^ pcDcIV, Model^ model);
    private : char TypeStream_;
    // OnEvent
    public : void OnStartStream(System::Object^ sender, System::EventArgs^ e);
    public : void OnStopStream(System::Object^ sender, System::EventArgs^ e);

   };


};