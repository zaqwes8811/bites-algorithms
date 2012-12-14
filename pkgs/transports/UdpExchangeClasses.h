/**
file : #include "UdpExchangeClasses.h"
	using namespace transport_layers;
*/
#ifndef UDP_EXCHANGE_CLASSES
#define UDP_EXCHANGE_CLASSES
#include <vector>
#include <string>
#include <windows.h>
#include <winsock.h>
#include "TransportStructurs.h"
#pragma comment(lib, "wsock32.lib")  // for Ethernet
extern OVERLAPPED olwr;  // будем использовать для операций чтения 
extern OVERLAPPED olrd;  // будем использовать для операций чтения 
namespace transport_layers
{
	/// <Настройки интерфеса>
	/// По сути контейнер. Создается и удаляется при организации соединения
	/// сделать бы со smart-указателями
	/// </Настройки интерфеса>
	class IfaceSettings {
		  private : char cTypeConnect_;
		  private : int* pType_;  // проверку пока не делаю, а беру с запасом
		  private : char** ppType_;  // строки с признаком конца
		  private : int iNumStr_;
		  
		  /// <Methods>
		  public : IfaceSettings();  // конструктор по умолчанию - на случай вызова 
			// ..много чего вызыв. дин. поэтому нужно бы защитить
		  public : IfaceSettings(int iNumInt, int iNumStr, char TypeConnect);
		  public : ~IfaceSettings();
		  
		  /// интерфейс
		  public : char getTypeConnect(void);
		public : void setTypeConnect(char Type);
		public : void TypeSet(int iPoint, int iValue);
		public : int iTypeGet(int iPoint);
		public : void TypeSet( int iPos, std::string sValue );
		public : char* cTypeGet(int iPos);
	};

	class simpleCmd;
	// any ifaces
	class IHardIfaceTop {
		  public : virtual ~IHardIfaceTop() {}
		  /// iface ///
		  public : virtual int Create(IfaceSettings* pifs) = 0;  // создание соединения, захват ресурсов
		  public : virtual int Rx(simpleCmd* scRx) = 0;  // читать
		  public : virtual int Tx(const FormatMsg& scTx) = 0;  // писать
		  public : virtual void Remove(void) = 0;  // закрытие соединения, отпускает нек. ресурсы
	};

	// just udp
	const WORD WINSOCK_VER = 0x0101;  // init static
	
	class hardIfaceUdp : public IHardIfaceTop {
		  /// API Data
		  private : WSADATA wsaData_;
		  private : int iResult_;  // результат открытия библиотеки
		  private : CRITICAL_SECTION csTx_;  // критическая секция для udp
		  private : CRITICAL_SECTION csRx_;
		  private : int iSockRx_;
		  private : struct sockaddr_in addrRx_;
		  private : int iSockTx_;
		  private : struct sockaddr_in addrTx_;
		  
		  // data
		  private : int maxReadOnceUdp_;  // размер буффера, которым черпаем
		  
		  // ThreadMT
		  private : bool bTxMT_ ;  // разрешение работы потока отправки
		  private : bool bRxMT_ ;  // потока чтения
		  private : void setTxState(bool TxFlag);  // функции установки
		  private : void setRxState(bool RxFlag);
			
		  /// <Methods>
		  public : ~hardIfaceUdp();
		  public : hardIfaceUdp();
		  public : int Create(IfaceSettings* pifs);  // создание соединения, захват ресурсов
		  public : int Rx(simpleCmd* scRx);  // заполнить бланк
		  public : int Tx(const FormatMsg& scTx);  // отправить бланк
		  public : void Remove(void);  // закрытие соединения, отпускает нек. ресурсы 
	};

	/// <Простейшая команда>
	/// есть проблема заполнения блонка, при приеме данных по потоковому протоколу
	/// RS, TCP. с Udp все хорошо, так же как и с отправкой по людому протоколу
	/// так же неизвестно сколько максимально может прийти. ! ставим макс таймаут и читаем большим буффеом
	/// </Простейшая команда>
	class simpleCmd {
		private : int NumCmd_;  // число команд в серии - оценка
		private : int NumCmdDel_;
		private : int* pNumCharInCmd_;  // число байт в комманде
		private : char* pCRC_;  // массив с конторльными суммами он один
		private : char** ppCmds_;  // 2D массив с командами
		  
		/// <Methods>
		public : simpleCmd(int NumCmd, int maxLen);
		public : ~simpleCmd();
		  
		// интерфейс
		// контольные суммы
		public : void calkCRC(int Pos);
		public : char getCRC(int Pos);
		// комманды
		public : void setNumCmd(int Value);
		public : int getNumCmd(void);  // получить число коммнд
		// заполнение и чтение
		public : void setCmd(int Pos, char* inBuf); // номер(с нуля); вх. массив; его длина;
		public : char* getCmdPtr(int Pos);  // возвр. указ. на массив с командой
		// длины комманд
		public : int getCmdLen(int Pos);  // 
		public : void setCmdLen(int Pos, int Value);  // 
		/// алгоритмы 
		// на отправку 1. число комманд. 2. длина каждой 3ю значение
	};
};

#endif