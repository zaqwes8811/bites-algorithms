#include <win_snmp/stdafx.h>
#include "debg.h"
using namespace std;
using namespace debg;
/// /// ///
/// 
/// IDebugInformer::
IDebugInformer::~IDebugInformer() {}
///
IDebugInformer::IDebugInformer() {}

/// /// ///
///
/// ADebugInformer::
std::string ADebugInformer::getLastError() { return this->err_info__; }
void ADebugInformer::PrintErr(std::string smsg, int imsg) 
{
	this->PrintMsg( smsg );
	this->PrintMsg( imsg );
	this->err_info__ = this->s_err_info__ + " - " + this->i_err_info__;
}
/*
/// /// ///
///
/// InnerInformer::
void InnerInformer::PrintMsg(std::string msg) 
{
	msg += '\n';
	this->err_info__ = msg;
}
///
void InnerInformer::PrintMsg(int msg) 
{
	char tmp[debg::MAX_CIF];
	this->err_info__ = ::_itoa(static_cast<int>(msg), tmp, 10);
	this->err_info__.append("\n");
}
///
void InnerInformer::PrintMsg(LPSTR msg) {}
///
void InnerInformer::PrintMsg(std::wstring msg) {}
*/

/// /// ///
/// 
/// ConsolDebugInformer::
ConsolDebugInformer::~ConsolDebugInformer() { //std::cout << "~ConsolDebugInformer()" << std::endl; 
}
ConsolDebugInformer::ConsolDebugInformer() { //std::cout << "ConsolDebugInformer()" << std::endl; 
}
///
void ConsolDebugInformer::PrintErr(std::string smsg) { this->err_info__ = smsg; }
///
//std::string ConsolDebugInformer::getLastError(void) { return this->err_info__; }
///
void ConsolDebugInformer::getAddInfo( HSNMP_SESSION* snmpSession ) { }
/// 
void ConsolDebugInformer::PrintMsg(string msg) { 
	this->s_err_info__ = msg;//cout << msg; 
}
///
void ConsolDebugInformer::PrintMsg(int msg) { 
	char tmp[debg::MAX_CIF];
	this->i_err_info__ = ::_itoa(static_cast<int>(msg), tmp, 10);
	//cout << msg << endl; 
}
/// 
void ConsolDebugInformer::PrintMsg(LPSTR msg) { 
	//cout << msg; 
}
///
void ConsolDebugInformer::PrintMsg(wstring msg) { 
	//cout << "Не реализовано." << endl; 
}

/// /// ///
///
/// SnmpConsolDebugInformer::
SnmpConsolDebugInformer::~SnmpConsolDebugInformer() { }

///
void SnmpConsolDebugInformer::getAddInfo( HSNMP_SESSION* snmpSession ) 
{ 
	snmpSession_ = snmpSession; 
}
SnmpConsolDebugInformer::SnmpConsolDebugInformer() { snmpSession_ = NULL; }
/// 
void SnmpConsolDebugInformer::PrintErr(std::string smsg) 
{
	this->setErrValue( smsg );
	this->PrintMsg( this->getLastError() );
}
///
std::string SnmpConsolDebugInformer::getLastError(void) { return this->err_info__; }
///
void SnmpConsolDebugInformer::setErrValue( std::string smsg ) 
{
    string err_info; 
	if( this->snmpSession_)
	{  
	  err_info += "SNMPAPI_FAILURE ";
	  err_info += smsg;
	  err_info += " - ";
	  this->snmpStatus_ = SnmpGetLastError(this->snmpSession_);
	  char tmp[debg::MAX_CIF];
	  err_info += ::_itoa(static_cast<int>(this->snmpStatus_), tmp, 10);
	  err_info += "\n";
	} 
	else { err_info = "Session no get()\n";	}
	/// установка
	this->err_info__ = err_info;
}

/// /// ///
/// 
/// Fabrics
std::auto_ptr<debg::IDebugInformer> FabricDWConsole::getDebugView() 
{ return std::auto_ptr<debg::IDebugInformer>(new ConsolDebugInformer()); }
std::auto_ptr<debg::IDebugInformer> FabricDWConsole::getSnmpDebugView() 
{ return std::auto_ptr<debg::IDebugInformer>(new SnmpConsolDebugInformer()); }
