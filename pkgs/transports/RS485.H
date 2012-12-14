/**
  file : #include "RS482.H"
*/
#ifndef COM
#define COM

//#include <afxmt.h>

  // Критические секции
#include <iostream>
#include <stdio.h>
#include <vector>
#include <windows.h>

// следующие идут в связке
#include "forDebug.h"
#include <uni_convertors/convertors.h>

/// /// /// 
///
/// Под интерфейсы сделаем отдельное простарнство имен
namespace Ifaces {
	typedef unsigned int uint;
	// абстракция к winapi
	typedef void (__stdcall * atom_lock)(LPCRITICAL_SECTION);
	typedef void (__stdcall * atom_unlock)(LPCRITICAL_SECTION);
	typedef void (__stdcall * init_locker)(LPCRITICAL_SECTION);
	typedef void (__stdcall * free_locker)(LPCRITICAL_SECTION);
struct retStruct {
	int retValue;
	bool done;
};
class RS485 {
public :
	RS485(std::string ComPort);
	RS485();	// пусть будет - ничего не открывает при создании
    ~RS485();

	// Операции непосредственно с портом
	retStruct write(UCHAR* pDataOut, int LengthData);
	retStruct read(UCHAR* pDataIn, int LengthData);
	BOOL resetPortParam(
		std::string portName, std::vector<uint> settings, 
		void (*changer)(_DCB&, _COMMTIMEOUTS&, std::vector<uint>) );	/* изменяет настройки */
	bool freeChannal();
	bool channalIsOpened();

	// Logging
	void enableProtocol();
	void disableProtocol();
private : 
	// Закрытие соединения
	BOOL _close();
	BOOL _open(std::string ComPort);
	BOOL _handleValid(); 
	BOOL _getParam(_DCB* lpDCBData, _COMMTIMEOUTS* lpTimeOutData);
	BOOL _setParam(_DCB* lpDCBData, _COMMTIMEOUTS* lpTimeOutData);
	void _init();  // общая для всех конструкторов иниц.
	// Настройки порта
	HANDLE hComPortHandle;
	std::string stdComPortName;
	_DCB RS485mode;  // в модуле не используется
	_COMMTIMEOUTS TimeOutsRS485;  // в модуле не используется
	
	FILE *protocol;
	uchar ReceiveData[256];
	uchar ReciveCheckSumm;
	BOOL ReciveStart;
	BOOL LockProtocol;

	// внутренние абстракции к многопоточному контролю
	atom_lock _alock;
	atom_unlock	_aunlock;
	init_locker _init_locker;
	free_locker _free_locker;
	::CRITICAL_SECTION cs_;
};
void changePortParametersStruct(
								_DCB &DCBParam,
								_COMMTIMEOUTS& COMM_TIMEOUTS_Param,  
								std::vector<uint> settings);
}  // end of namespace
#endif