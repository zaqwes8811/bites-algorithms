#include "stdafx.h"
#include "Protocols.h"
using namespace std;
using namespace protocols;
using namespace Convertors;
using namespace cpp_debug;
using namespace transport_layers;
/// Перепаковка потока. вообще-то это уже часть реализации протокола
BOOL IProtocol::send(_FormatRS485Data* pDataRS485, void (*printTx_)(UCHAR*, int)) {
	uchar Data[520];
	const uchar* pData = pDataRS485->pDataMessage;

	int Offset = 5;

	Data[0] = 0xAA;
	Data[1] = 0x55;

	//
	Data[2] = pDataRS485->LengthDataMessage+4;
	if (Data[2] == 0xAA){
		Offset++;
		Data[ 2+Offset ] = 0x00;
	}

	//
	Data[3] = pDataRS485->IDDevice;
	if (Data[3] == 0xAA){
		Offset++;
		Data[ 3+Offset ] = 0x00;
	}

	//
	Data[4] = pDataRS485->TypeMessage;
	if (Data[4] == 0xAA){
		Offset++;
		Data[ 4+Offset] = 0x00;
	}

	// для контролья целостности посылки считаме контрольную сумму
	unsigned char CheckSumm = pDataRS485->LengthDataMessage + 4 + 
		pDataRS485->IDDevice+
		pDataRS485->TypeMessage;

	/// /// ///
	int i = 0;
	for ( i = 0; i < pDataRS485->LengthDataMessage; i++ ){
		CheckSumm += *pData;
		Data[i + Offset] = *pData;
		if (*pData == 0xAA) {
			Offset ++;	Data[i + Offset] = 0x00;
		}
		pData++;
	}

	Data[ i+Offset ] = CheckSumm;
	if (CheckSumm == 0xAA){
		Offset++;
		Data[ i+Offset ] = 0x00;
	}

#ifdef CONTENT_UDP_CLIENT
	int LenInfo = i+Offset+1;
	FormatMsg fm = { LenInfo+1, reinterpret_cast<char*>(Data)};
	fm.pDataContent[ LenInfo ] = 0;	// заменяем на признак напр. отпр. Tx = 0
	sendMsg_( fm );
	//::Sleep(100);
#endif
	//
	Ifaces::retStruct result = _rs485.write(&Data[0], i+Offset+1);
	int numWrite = result.retValue;
	if ( numWrite == (i+Offset+1) ) return TRUE;
	else return FALSE;
}


BOOL IProtocol::receive(FormatDataToSend* pDataRS485, void (*printRx_)(UCHAR*, int)) {
	// приемный буффер
	uchar Data[520]; 

	// само чтение
	
	Ifaces::retStruct result = _rs485.read(&Data[0], 100);
	if( !result.done ) { return FALSE; }
	int Readen = result.retValue;
#ifdef _отладка
	// рисуем полученные данные в консоле
	printRx_(Data, Readen);
#endif //_отладка
	// входной контроль
	int LenghtRD = 0;
	if ( Readen > 0) {
		for (int i = 0; i < Readen; i ++) {
			ReceiveTempData[LenghtRD] = Data[i];
			if (LenghtRD < 1023) LenghtRD++;
		}
	}
#ifdef CONTENT_UDP_CLIENT
	FormatMsg fm = { LenghtRD+1, reinterpret_cast<char*>(Data)};
	fm.pDataContent[LenghtRD] = 1;	// заменяем на признак напр. отпр. Tx = 0
	sendMsg_( fm );
	//::Sleep(500);
#endif

	// обработка? весьма замысловатая
	while (LenghtRD > 5) {
		if  ((ReceiveTempData[0] == 0xAA) && (ReceiveTempData[1] == 0x55))	break;
		for ( int i = 1; i < LenghtRD; ++i )	ReceiveTempData[ i-1 ] = ReceiveTempData[i];
		LenghtRD--;
	}
	
	// первые два байта (0, 1)
	if ((LenghtRD > 5) && ((ReceiveTempData[0] == 0xAA) && (ReceiveTempData[1] == 0x55))) {
		OldReceiveByte = 0;
	
		int Offset = 0;

		// 2, 3
		unsigned char CS = ReceiveTempData[2];
		pDataRS485->LengthDataMessage = ReceiveTempData[2]-4;
		if ((ReceiveTempData[2] == 0xAA) && (ReceiveTempData[3] == 0x55)) {
			ReceiveTempData[0] = 0;
			return FALSE;		
		}
		if (ReceiveTempData[2] == 0xAA)	{	Offset++;	}

		//
		CS += ReceiveTempData[ 3+Offset ];
		pDataRS485->IDDevice = ReceiveTempData[ 3+Offset ];
		if ((ReceiveTempData[ 3+Offset ] == 0xAA) && (ReceiveTempData[ 4+Offset ] == 0x55))	{
			ReceiveTempData[0] = 0;
			return FALSE;		
		}
		if (ReceiveTempData[ 3+Offset] == 0xAA) {	Offset++;	}

		//
		CS += ReceiveTempData[4 + Offset];
		pDataRS485->TypeMessage = ReceiveTempData[4 + Offset];
		if ((ReceiveTempData[ 4+Offset] == 0xAA) && (ReceiveTempData[ 5+Offset ] == 0x55)) {
			ReceiveTempData[0] = 0;
			return FALSE;		
		}
		OldReceiveByte = ReceiveTempData[ 4+Offset ];
		
		// информация? 5->...
		UCHAR* pData = pDataRS485->pDataMessage;
		for (int i = 5; i < LenghtRD;  i++) {
			if (OldReceiveByte == 0xAA)	{
				if (ReceiveTempData[ i+Offset ] == 0x55) {
					ReceiveTempData[0] = 0;
					break;
				}
				Offset++;
			}
			*pData = ReceiveTempData[ i+Offset];
			OldReceiveByte = ReceiveTempData[ i+Offset ];

			pData++;

			if ( ReceiveTempData[2] == (i-1) )	{
				ReceiveTempData[0] = 0;
				if (CS == ReceiveTempData[ i+Offset ])	{ 
					return TRUE;  // похоже единственная true-шная ветка
				}
				else {
					BadMess++;
					return FALSE;
				}
			}
			else { CS += ReceiveTempData[ i+Offset ]; }
		}
	}

	//
	return FALSE;
}

// Сперва проверяем указатель на ноль. А потом валидность хэндла
const BOOL IProtocol::testValidTransLayer() const {
	return _rs485.channalIsOpened();
}

// отправка сообщения
#ifdef CONTENT_UDP_CLIENT
void IProtocol::sendMsg_( const FormatMsg& scTx ) {
	
	//::EnterCriticalSection( &csUDPSender_ );

	// отправляем 
	hift_->Tx( scTx );
	//::LeaveCriticalSection( &csUDPSender_ );
	
}
#endif

// создание udp-клиента
void IProtocol::init_() {
#ifdef CONTENT_UDP_CLIENT
	::InitializeCriticalSection(&this->csUDPSender_);
	// настраеваем соединение
	IfaceSettings pifs(2, 2, 'u'); // два порта два адреса
	// ports
	int isPort = S_PORT; int idPort = D_PORT;
	pifs.TypeSet(0, isPort); pifs.TypeSet(1, idPort);  
	// string
	string sIP = SIP; 
	pifs.TypeSet( 0, sIP ); pifs.TypeSet( 1, sIP ); 
		// соединеняемся
	hift_.reset( new transport_layers::hardIfaceUdp() ); 
	hift_->Create( &pifs );
#endif
}


IProtocol::~IProtocol(void){
#ifdef CONTENT_UDP_CLIENT
		// разрываем соединение
		hift_->Remove();
		::DeleteCriticalSection(&this->csUDPSender_);
#endif
	}
IProtocol::IProtocol( ){ init_(); }

ErrCode IProtocol::freeTransportLayer(){ 
	return _rs485.freeChannal();
}
ErrCode IProtocol::resetLayerParameters( Settings comPortName ) {
	// Перезагружаем транспортный слой
	string stringPortName = Convertors::CString2string(comPortName);
	vector<unsigned int> v;
	if ( _rs485.resetPortParam(stringPortName, v, Ifaces::changePortParametersStruct) ) { return true;} 
	else { return false; }
}

//#ifdef отладка
/// /// /// /// ///
/// Функции форматирования ввывода на консоль
void protocols::printTx(UCHAR* Data, int iLenBuf){
	printWarning(">>> Tx : ");
	for( int j = 0; j < iLenBuf; j++) { 
		// переводим в хекс строку
		string byteValue = hl(static_cast<char>(Data[j]));

		// если пришла комманда, то сигнализируем
		if(Data[j-1] == 0x03)	printWarning( byteValue ); 
		else printSimple(byteValue+" ");
		if((j+1)%4 == 0) cout << " ";
	}
	cout << endl;
	//::Sleep(500);
}
void protocols::printRx(UCHAR* Data, int Readen){
	for( int j = 0; j < Readen; j++) { 
		if((j)%16 == 0)	cout << endl << "Rx : ";

		// сами данные форматируем четверками
		cout << hl(static_cast<char>(Data[j])) << " "; 
		if((j+1)%4 == 0) cout << " ";
	}
	cout << endl;

}

// просто заглошки
void protocols::p0(UCHAR*, int){}
//#endif // отладка