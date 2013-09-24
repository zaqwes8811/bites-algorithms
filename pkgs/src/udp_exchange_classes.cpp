#include "stdafx.h"
#include "UdpExchangeClasses.h"
using namespace transport_layers;
using namespace std;
/// <UDP>
/// hardIfaceUdp::
/// </UDP>
OVERLAPPED olwr;  // будем использовать дл€ операций записи
OVERLAPPED olrd;  // будем использовать дл€ операций чтени€

hardIfaceUdp::hardIfaceUdp(){
  // other
  InitializeCriticalSection(&csTx_);  // захватываем критическую секцию
  InitializeCriticalSection(&csRx_);
  maxReadOnceUdp_ = 200;
}
 
hardIfaceUdp::~hardIfaceUdp(){
  ///
  if(iSockRx_) { closesocket(iSockRx_); }
  if(iSockTx_) { closesocket(iSockRx_); }
  // other
  DeleteCriticalSection(&csTx_);  // удал€ем критическую секции
  DeleteCriticalSection(&csRx_); 
}
int hardIfaceUdp::Create(IfaceSettings* pifs) 
{
	iResult_ = WSAStartup( WINSOCK_VER, &wsaData_);
  if (iResult_ != NO_ERROR) { WSACleanup(); return 4; }
  ///
  iSockRx_ = 0;  // лучше в конструеторе, но пока так
  iSockTx_ = 0;

  /// "распиновка"
  unsigned short port = pifs->iTypeGet(0);
  unsigned short dport = pifs->iTypeGet(1);
  const char* const sipaddr = pifs->cTypeGet(0);
  const char* const dipaddr = pifs->cTypeGet(1);

  /// Rx ///
  iSockRx_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(iSockRx_ >= 0) {  // сокет создан
    addrRx_.sin_family = AF_INET;
    addrRx_.sin_port = htons(port);
    addrRx_.sin_addr.s_addr = inet_addr(sipaddr);
    // св€зываем 
    if(bind(iSockRx_, (struct sockaddr *)&addrRx_, sizeof(addrRx_)) < 0) {
      return 2;  // ощибка св€зывани€ bind()
    }
  } 
  else { return 1; }  // не создан сокет чтени€
  
  /// Tx ///
  iSockTx_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(iSockTx_ >= 0) {  // по карйней мере создали
    addrTx_.sin_family = AF_INET;
    addrTx_.sin_port = htons(dport);
    addrTx_.sin_addr.s_addr = inet_addr(dipaddr);
  } 
  else { return 3; } // сокет записи не создан

  // разрашаем работу потоков
  setTxState(true);  
  setRxState(true);

  // все прошло успешно, раз мы здесь
  return 0;       
}
/// разрыв соединени€
void hardIfaceUdp::Remove(void) {
  /// останавливаем потоки, вернее мен€ем флаги
  setTxState(false);
  setRxState(false);
  ///
  if(iSockRx_) {
    closesocket(iSockRx_);  // если был заблокирован, то вернет -1
    iSockRx_ = 0;}
  if(iSockTx_) {
    closesocket(iSockTx_);
    iSockTx_ = 0; }
  /// зактываем библиотеку
  WSACleanup();
}

/// методы запускаемые в потоках
int hardIfaceUdp::Rx(transport_layers::simpleCmd* scRx) {
  int RxOut = 0;
  int lenRxCmd = 0;
  bool locbRx;
  EnterCriticalSection(&csRx_);
  locbRx = bRxMT_;
  LeaveCriticalSection(&csRx_); 
  if(locbRx) {
    if(scRx) {
      // читаем, сокет блокирующий, поэтому пока он спить
      lenRxCmd = recvfrom(iSockRx_, scRx->getCmdPtr(0), maxReadOnceUdp_, 0, NULL, NULL);
      ///
      scRx->setCmdLen(0, lenRxCmd);  // сохран€ем число прин€тых байт
      if(lenRxCmd >= 0) RxOut = 0;
      else RxOut = -1; 
      ///
    }  // if pointer no zero
  }
  else {
      RxOut = 1;  // выходим из потока
  }  
  ///
  return RxOut;
}
///
int hardIfaceUdp::Tx( const FormatMsg& scTx ) {

	// потокобезопасно ли это
	EnterCriticalSection(&csRx_);
	int TxOut = 0;
	int err = sendto( iSockTx_, scTx.pDataContent, scTx.dataLen, 0,
		(struct sockaddr *)&addrTx_,  sizeof(addrTx_) ); 
    if(err != -1) { TxOut = 0; } 
	else { TxOut = 2; }
	LeaveCriticalSection(&csRx_); 
	//

	return TxOut;
}

/// многопоточна€ часть
void hardIfaceUdp::setTxState(bool TxFlag) {
    EnterCriticalSection(&csTx_); bTxMT_ = TxFlag;
    LeaveCriticalSection(&csTx_); }
///
void hardIfaceUdp::setRxState(bool RxFlag) {
    EnterCriticalSection(&csRx_); bRxMT_ = RxFlag;
    LeaveCriticalSection(&csRx_); }
///

/// <SimpleCmd>
/// </SimpleCmd>
simpleCmd::~simpleCmd() {
  // 1D
  if(pCRC_) {delete [] pCRC_; pCRC_ = NULL;}
  if(pNumCharInCmd_) delete [] pNumCharInCmd_;
 
  // 2D
  for(int i = 0; i < 1; i++) {
    if(ppCmds_[i]) delete [] ppCmds_[i];
  }
  if(ppCmds_) delete [] ppCmds_;
  
}
// конструктор комманды
simpleCmd::simpleCmd(int NumCmd, int maxLen) {
  /// 1D 
  pCRC_ = new char[NumCmd];  // значение контольных сумм
  pNumCharInCmd_ = new int[NumCmd];  // число байт в комманде

  // инициализаци€
  for(int i = 0; i < NumCmd; i++) {
    pCRC_[i] = '\0'; 
  }

  /// 2D array
  ppCmds_ = new char*[NumCmd];   // массив указателей на сами комманды
  for(int i = 0; i < NumCmd; i++) {  // выдел€ем
    if(ppCmds_) ppCmds_[i] = new char[maxLen*2];  // с запасом
  }
  NumCmdDel_  = NumCmd;
}

/// расчет контрольных сумм
void simpleCmd::calkCRC(int Pos) {  // раз вызвали то считаем
  if(pCRC_) {
    pCRC_[Pos] = 0;  // обнуление значени€ суммы
    for(int i = 0; i < pNumCharInCmd_[Pos]; i++) {
      pCRC_[Pos] += ppCmds_[Pos][i];
    }
  }  // if(pCR..
}
/// получение контрольных сумм
char simpleCmd::getCRC(int Pos) { 
    calkCRC(Pos);
    return pCRC_[Pos]; }

///  установка комманды
void simpleCmd::setCmd(int Pos, char* inBuf) {
  int Len = pNumCharInCmd_[Pos];
  for(int i = 0; i < Len; i++)
  {
    ppCmds_[Pos][i] = inBuf[i];  // заполн€ем
  }
}
// указатель
char* simpleCmd::getCmdPtr(int Pos){  // получение указател€
  if(ppCmds_[Pos]) {
    return ppCmds_[Pos];  // возвращаем указатель
  }
  else return NULL;  
}

/// число байт в комманде
int simpleCmd::getCmdLen(int Pos) { return pNumCharInCmd_[Pos]; }
void simpleCmd::setCmdLen(int Pos, int Value) { pNumCharInCmd_[Pos] = Value; }

/// число комманд
void simpleCmd::setNumCmd(int Value) { NumCmd_ = Value; }
int simpleCmd::getNumCmd(void) { return NumCmd_; }



/// <IfaceSettings>
/// </IfaceSettings>
void IfaceSettings::TypeSet(int iPoint, int iValue) { 
  if(pType_)
    pType_[iPoint] = iValue; }

IfaceSettings::~IfaceSettings() {
  if(pType_)
    delete [] pType_;
  for(int i = 0; i < iNumStr_; i++)
    if(ppType_[i])
      delete [] ppType_[i];  // delete rows
    if(ppType_)
      delete [] ppType_;  // delete array pointers
}
///
int IfaceSettings::iTypeGet(int iPoint) { 
  if(pType_)
    return pType_[iPoint]; 
  else
  return 0; 
}
///
void IfaceSettings::TypeSet(int iPos, string cValue) {  // длина перед. без нул.-симв.
	int iStrLen = cValue.length();
	const char* pstr = cValue.c_str();
	if(ppType_) {
		ppType_[iPos] = new char[iStrLen+1];  // все символы 
		for(int i = 0; i < iStrLen; i++)  // можно и strcpy()
			ppType_[iPos][i] = pstr[i];
		ppType_[iPos][iStrLen] = '\0'; 
	}
}
///
char* IfaceSettings::cTypeGet(int iPos) {
  return ppType_[iPos];  // непо€тно как защитить!
}

///
IfaceSettings::IfaceSettings(int iNumInt, int iNumStr, char TypeConnect) { 
  cTypeConnect_ = TypeConnect;
  pType_ = new int[iNumInt];
  ppType_ = new char* [iNumStr];
  for(int i = 0; i < iNumInt; i++) {
    ppType_[i] = 0; }
  iNumStr_ = iNumStr;  // будет предполагатс€, что все выделено
}
IfaceSettings::IfaceSettings() {  // конструктор по умолчанию - на случай вызова 
  cTypeConnect_ = '\0';  // ..много чего вызыв. дин. поэтому нужно бы защитить
  pType_ = NULL; ppType_ = NULL;
}
void IfaceSettings::setTypeConnect(char Type) { cTypeConnect_ = Type; }
char IfaceSettings::getTypeConnect(void) { return cTypeConnect_; }


