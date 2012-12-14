#include "stdafx.h"
#include "RS485.h"
using namespace std;
using namespace Ifaces;
using namespace cpp_debug;
using namespace Convertors;

void RS485::_init() {
	_alock = ::EnterCriticalSection;
	_aunlock = ::LeaveCriticalSection;
	_init_locker = ::InitializeCriticalSection;
	_free_locker = ::DeleteCriticalSection;

	hComPortHandle = INVALID_HANDLE_VALUE;
	
	ReciveStart = FALSE;
	LockProtocol = TRUE;
	
	// инициализируем критическую секцию
	_init_locker(&cs_);
}


RS485::~RS485() {
	// закрываемс€
	this->_close();

	// уничтожаем критическую секцию
	_free_locker(&cs_);
}

RS485::RS485() { _init(); } 
RS485::RS485( string ComPort ) {
	_init();
	// уже идет залочивание
	this->_open( ComPort );
}

void RS485::enableProtocol() { LockProtocol = FALSE; }
void RS485::disableProtocol() {	LockProtocol = TRUE; }

BOOL RS485::_handleValid() {
	if (hComPortHandle != INVALID_HANDLE_VALUE)
		return TRUE;
	return FALSE;
}

BOOL RS485::_close() {
	if ( _handleValid( ) )	{
		_alock(&cs_);
		CloseHandle(hComPortHandle);
		hComPortHandle = INVALID_HANDLE_VALUE;
		_aunlock(&cs_);
		return TRUE;
	}
	else {
		return FALSE;
	}
}


BOOL RS485::_open(std::string ComPort){
	if ( _handleValid( )){ CloseHandle(hComPortHandle); }
	_alock(&cs_);
	stdComPortName = ComPort;
	hComPortHandle = CreateFile (
		ComPort.c_str(),
		GENERIC_READ | GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		0,
		NULL);
	_aunlock(&cs_);
	return _handleValid();
}


BOOL RS485::resetPortParam(
		std::string portName, 
		std::vector<uint> settings, 
		void (*changer)(_DCB&, _COMMTIMEOUTS&, std::vector<uint>) ) 
	{
	BOOL result = false;
	if ( _handleValid()) { _close(); }
	if ( _open( portName ) ) {
		_DCB DCBParam;
		_COMMTIMEOUTS COMM_TIMEOUTSParam;
		// читаем текущие настройки порта
		_alock(&cs_);
		if ( _getParam( &DCBParam, &COMM_TIMEOUTSParam ) ) {
			// перенастраиваем
			changer( DCBParam, COMM_TIMEOUTSParam, settings );

			// ставим обратно
			if( _setParam( &DCBParam, &COMM_TIMEOUTSParam ) ){ 
				result = TRUE;	// в одном случае все хорошо!
			}
		}
		_aunlock(&cs_);
	}
	return result;
}

BOOL RS485::_getParam(_DCB* lpDCBData, _COMMTIMEOUTS* lpTimeOutData) {
	BOOL a = GetCommState(hComPortHandle, lpDCBData);
	BOOL b = GetCommTimeouts(hComPortHandle, lpTimeOutData);
	if ((!a) || (!b)) {	return FALSE;}
	return TRUE;
}
BOOL RS485::_setParam(_DCB* lpDCBData, _COMMTIMEOUTS* lpTimeOutData) {
	// устанавливаем
	BOOL a = SetCommState(hComPortHandle, lpDCBData);
	BOOL b = SetCommTimeouts(hComPortHandle, lpTimeOutData);
	if ((!a) || (!b)) {	return FALSE;}
	return TRUE;
}

retStruct RS485::write(unsigned char* pDataOut, int LengthData) {
	_alock(&cs_);
	retStruct result = {0, false};
	DWORD dwWritten;
	if ( WriteFile(hComPortHandle, pDataOut, LengthData, &dwWritten, NULL) == FALSE) {
		_aunlock(&cs_);
		return result;
	}

	// ѕротоколируем
	string fname = stdComPortName+" protocol.dat";
	if (!LockProtocol) {
		protocol = fopen(fname.c_str(),"a");
		if (protocol != NULL) {
			fprintf(protocol,"ѕереданые данные\t");
			for (int i = 0; i < LengthData; i++) {
				BYTE *a;
				a = pDataOut + i;
				fprintf(protocol,"%2.2X  ", *a);
			}
			fprintf(protocol,"\n");
			fclose(protocol);
		}
	}
	_aunlock(&cs_);
	result.retValue = dwWritten;
	result.done = true;
	return result;
}

retStruct RS485::read(unsigned char* pDataIn, int LengthData) {
	_alock(&cs_);
	retStruct result = {0, false};
	DWORD dwReaden;

	if ( ReadFile(hComPortHandle, pDataIn, LengthData, &dwReaden, NULL) == FALSE ) {
		_aunlock(&cs_);
		return result;
	}
	if ( dwReaden == 0 ) {
		_aunlock(&cs_);
		return result;
	}
	if (!LockProtocol) {
		string fname = stdComPortName+" protocol.dat";
			protocol = fopen(fname.c_str(),"a");
		if (protocol != NULL) {
			fprintf(protocol,"ѕрин€тые данные\t\t");
			for (DWORD i = 0; i < dwReaden; i++) {
				BYTE *a;
				a = pDataIn + i;
				fprintf(protocol,"%2.2X  ", *a);
			}
			fprintf(protocol,"\n");
			fclose(protocol);
		}
	}
	_aunlock(&cs_);
	result.retValue = dwReaden;
	result.done = true;
	return result;
}
bool RS485::freeChannal() {
	return this->_close();
}
bool RS485::channalIsOpened() {
	return _handleValid();
}

/// /// ///
void Ifaces::changePortParametersStruct(_DCB &DCBParam, _COMMTIMEOUTS& COMM_TIMEOUTS_Param, 
										vector<uint> settings) {
	COMM_TIMEOUTS_Param.ReadIntervalTimeout = 0;
	COMM_TIMEOUTS_Param.ReadTotalTimeoutMultiplier = 0;
	COMM_TIMEOUTS_Param.ReadTotalTimeoutConstant = 50;
	COMM_TIMEOUTS_Param.WriteTotalTimeoutMultiplier = 0;
	COMM_TIMEOUTS_Param.WriteTotalTimeoutConstant = 50;

	// обмен идет на максимальной скорости
	DCBParam.BaudRate			=	115200;//38400;//19200;//9600;
	DCBParam.fBinary				=	1;
	DCBParam.fParity				=	0;
	DCBParam.fOutxCtsFlow			=	0;
	DCBParam.fOutxDsrFlow			=	0;
	DCBParam.fDtrControl			=	DTR_CONTROL_DISABLE;
	DCBParam.fDsrSensitivity		=	0;
	DCBParam.fTXContinueOnXoff	=	0;
	DCBParam.fOutX				=	0;
	DCBParam.fInX					=	0;
	DCBParam.fRtsControl			=	RTS_CONTROL_DISABLE;
	DCBParam.fAbortOnError		=	0;
	DCBParam.ByteSize				=	8;
	DCBParam.Parity				=	NOPARITY;
	DCBParam.StopBits				=	ONESTOPBIT;
}






