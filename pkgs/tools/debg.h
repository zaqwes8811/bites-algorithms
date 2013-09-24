/**
  about : вспомогательный функции
  
  file : #include "debg.h"

  think :
    русский на консоль пока вывести не удалось
*/
#ifndef _DEBG
#define _DEBG
#include "StdAfx.h"
#include <WinSnmp.h>
#include <string>
#include <iostream>
#include <memory>
#include <ctime>
#include <deque>
#include <vector>
/**
  abs. : выводит отладочную информацию и ошибки
  - на консоль
  - на форму из основного потока 
  // хорошо бы, но пока до конца не по€нтно как
  // возможно наследовать придетс€ от классов MFC
  - на форму из неосновного потока

  эти классы можно будет потом использовать 
  и при обработки исключений

*/
namespace debg 
{
  const int MAX_CIF = 5;
  /// /// ///
  /// 
  /// дополнительна€ информаци€ - иерархи€ интерфейсов
  class IAddInfoForDebug 
  {
  public : virtual ~IAddInfoForDebug(){}
  };
  /// /// ///
  class IAddSnmpInfo : public IAddInfoForDebug 
  {
    public : virtual HSNMP_SESSION* getSnmpSession() = 0;
    public : virtual void setSnmpSession(HSNMP_SESSION* p) = 0;
  };

  /// /// ///
  /// 
  /// дополнительна€ информаци€ уже конкретные классы
  class AddSnmpInfo : public IAddSnmpInfo 
  {
    public : HSNMP_SESSION* getSnmpSession() { return this->p_; }
    public : void setSnmpSession(HSNMP_SESSION* p) { this->p_ = p; }
	/// Core
    private : HSNMP_SESSION* p_;

  };

  /// ¬ершина иерарxии
  //template <typename T>
  class IDebugInformer
  {
    public : virtual void PrintMsg(std::string msg) = 0;
    public : virtual void PrintMsg(int msg) = 0;
    public : virtual std::string getLastError(void) = 0;
    public : virtual void PrintMsg(LPSTR msg) = 0;
    public : virtual void PrintMsg(std::wstring msg) = 0;
    public : virtual void PrintErr(std::string smsg) = 0;
    public : virtual void PrintErr(std::string smsg, int imsg) = 0;
    public : virtual void getAddInfo( HSNMP_SESSION* snmpSession ) = 0;
	public : virtual ~IDebugInformer();
    public : IDebugInformer();
  };
  
  /// /// ///
  class ADebugInformer : public IDebugInformer 
  {
	  /// Iface
      public : virtual std::string getLastError(void);
      public : virtual void PrintErr(std::string smsg, int imsg);

	  /// Core
	  protected : std::string i_err_info__;  // ошибки идут парой - „то и число
	  protected : std::string s_err_info__;
      protected : std::string err_info__;
  };
/*
  /// вывод в err_info__
  class InnerInformer : public ADebugInformer 
  {
    /// просто вывод сообщений
    public : virtual void PrintMsg(std::string msg);
    public : virtual void PrintMsg(int msg);
    public : virtual void PrintMsg(LPSTR msg);
    public : virtual void PrintMsg(std::wstring msg);
  };
*/

  /**
    вывод на консоль
  */
  class ConsolDebugInformer : public ADebugInformer 
  {
    public : void virtual PrintMsg(std::string msg);
    public : void virtual PrintMsg(int msg);
    public : void virtual PrintMsg(LPSTR msg);
    public : void virtual PrintMsg(std::wstring msg);
    public : void virtual PrintErr(std::string smsg);
    //public : std::string virtual getLastError(void);
    public : void virtual getAddInfo( HSNMP_SESSION* snmpSession );
    public : ~ConsolDebugInformer();
    public : ConsolDebugInformer();
    
  };

  /// /// ///
  class SnmpConsolDebugInformer : public ConsolDebugInformer 
  {
    /// Iface
    public : void getAddInfo( HSNMP_SESSION* snmpSession );
    public : void PrintErr(std::string smsg);
	public : std::string virtual getLastError(void);
    public : ~SnmpConsolDebugInformer();
    public : SnmpConsolDebugInformer();
    
    /// Core
    private : SNMPAPI_STATUS snmpStatus_;
    private : HSNMP_SESSION* snmpSession_;
    private : void setErrValue( std::string smsg ) ;
  };

  /// /// /// 
  /// 
  /// √енераци€ видов дл€ вывода сообщений об ошибках
  class FabricDebugView 
  {
    public : std::auto_ptr<debg::IDebugInformer> virtual getDebugView() = 0; 
    public : std::auto_ptr<debg::IDebugInformer> virtual getSnmpDebugView() = 0; 
  };
  class FabricDWConsole : public FabricDebugView 
  {
    public : std::auto_ptr<debg::IDebugInformer> getDebugView();
    public : std::auto_ptr<debg::IDebugInformer> getSnmpDebugView();
  };
};
#endif _DEBG
