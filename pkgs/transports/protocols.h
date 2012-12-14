/**
	file : #include "Protocols.h"

	для размышления
	void updateInfo()
{
  ...
}

map <string, void (*)()> mFunctions;
mFunctions["updateInfo"] = updateInfo;
//и так далее для всех функций, что нужно использовать
...
string sFunctionName;
... 
//где-то там считали имя функции

mFunctions[sFunctionName]();//выполнили нужную функцию
mFunctions["updateInfo"]();//то же самое

Updated 11.04.2007
[Сразу несколько человек в комментариях заметили, что 
есть способ лучше, предложенный Александреску в "Современном проектировании".]
*/

#ifndef _PROTOCOLS
#define _PROTOCOLS

// for debug
#include <uni_convertors/convertors.h>
#include "forDebug.h"

// Интерфейсы
#include "RS485.h"
#include "UdpExchangeClasses.h"
// stl
#include <memory>
#include <string>
typedef int ErrCode;
typedef CString Settings;
/// /// ///
///
///
namespace protocols {
/**

  TODO : 
    сделать интерфейсом и наследовать 
	  поменять имена функций на общепринятые 
*/
class IProtocol {
	// получение указателя на передающий уровень
public : 
	virtual ~IProtocol(void);
	explicit IProtocol();
	
	// Функции отправки
	// передается массив с данными и методом их вывода на консоль
	BOOL send( _FormatRS485Data* pDataRS485, void (*printTx)(UCHAR*, int));
	BOOL receive( FormatDataToSend* pDataRS485, void (*printRx)(UCHAR*, int) );

	// проверка работоспособности транспортного уровня
	const BOOL testValidTransLayer() const;

	// для настройки транспортного уровня - очень плохая идея!!
	virtual ErrCode resetLayerParameters( Settings set );
	virtual ErrCode freeTransportLayer();

private : 
	// Объект отправитель данных
	mutable Ifaces::RS485 _rs485;

	// внутренние буфферы
	uchar ReceiveTempData[1024];
	int BadMess;
	uchar OldReceiveByte;

#ifdef CONTENT_UDP_CLIENT
	::CRITICAL_SECTION csUDPSender_;
	std::auto_ptr<transport_layers::IHardIfaceTop> hift_;
	// отправка сообщения
	void sendMsg_( const FormatMsg& scTx );
	// создание udp-клиента
	
#endif
	void init_();
};

	// функции раскраски 
void printTx(UCHAR*, int);
void printRx(UCHAR*, int);
void p0(UCHAR*, int);

}  // end namespace
#endif
