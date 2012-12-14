#include "stdafx.h"
#include "UdpExchangeClasses.h"
using namespace transport_layers;
using namespace std;
/// <UDP>
/// hardIfaceUdp::
/// </UDP>
OVERLAPPED olwr;  // ����� ������������ ��� �������� ������
OVERLAPPED olrd;  // ����� ������������ ��� �������� ������

hardIfaceUdp::hardIfaceUdp(){
  // other
  InitializeCriticalSection(&csTx_);  // ����������� ����������� ������
  InitializeCriticalSection(&csRx_);
  maxReadOnceUdp_ = 200;
}
 
hardIfaceUdp::~hardIfaceUdp(){
  ///
  if(iSockRx_) { closesocket(iSockRx_); }
  if(iSockTx_) { closesocket(iSockRx_); }
  // other
  DeleteCriticalSection(&csTx_);  // ������� ����������� ������
  DeleteCriticalSection(&csRx_); 
}
int hardIfaceUdp::Create(IfaceSettings* pifs) 
{
	iResult_ = WSAStartup( WINSOCK_VER, &wsaData_);
  if (iResult_ != NO_ERROR) { WSACleanup(); return 4; }
  ///
  iSockRx_ = 0;  // ����� � ������������, �� ���� ���
  iSockTx_ = 0;

  /// "����������"
  unsigned short port = pifs->iTypeGet(0);
  unsigned short dport = pifs->iTypeGet(1);
  const char* const sipaddr = pifs->cTypeGet(0);
  const char* const dipaddr = pifs->cTypeGet(1);

  /// Rx ///
  iSockRx_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(iSockRx_ >= 0) {  // ����� ������
    addrRx_.sin_family = AF_INET;
    addrRx_.sin_port = htons(port);
    addrRx_.sin_addr.s_addr = inet_addr(sipaddr);
    // ��������� 
    if(bind(iSockRx_, (struct sockaddr *)&addrRx_, sizeof(addrRx_)) < 0) {
      return 2;  // ������ ���������� bind()
    }
  } 
  else { return 1; }  // �� ������ ����� ������
  
  /// Tx ///
  iSockTx_ = socket(AF_INET, SOCK_DGRAM, 0);
  if(iSockTx_ >= 0) {  // �� ������� ���� �������
    addrTx_.sin_family = AF_INET;
    addrTx_.sin_port = htons(dport);
    addrTx_.sin_addr.s_addr = inet_addr(dipaddr);
  } 
  else { return 3; } // ����� ������ �� ������

  // ��������� ������ �������
  setTxState(true);  
  setRxState(true);

  // ��� ������ �������, ��� �� �����
  return 0;       
}
/// ������ ����������
void hardIfaceUdp::Remove(void) {
  /// ������������� ������, ������ ������ �����
  setTxState(false);
  setRxState(false);
  ///
  if(iSockRx_) {
    closesocket(iSockRx_);  // ���� ��� ������������, �� ������ -1
    iSockRx_ = 0;}
  if(iSockTx_) {
    closesocket(iSockTx_);
    iSockTx_ = 0; }
  /// ��������� ����������
  WSACleanup();
}

/// ������ ����������� � �������
int hardIfaceUdp::Rx(transport_layers::simpleCmd* scRx) {
  int RxOut = 0;
  int lenRxCmd = 0;
  bool locbRx;
  EnterCriticalSection(&csRx_);
  locbRx = bRxMT_;
  LeaveCriticalSection(&csRx_); 
  if(locbRx) {
    if(scRx) {
      // ������, ����� �����������, ������� ���� �� �����
      lenRxCmd = recvfrom(iSockRx_, scRx->getCmdPtr(0), maxReadOnceUdp_, 0, NULL, NULL);
      ///
      scRx->setCmdLen(0, lenRxCmd);  // ��������� ����� �������� ����
      if(lenRxCmd >= 0) RxOut = 0;
      else RxOut = -1; 
      ///
    }  // if pointer no zero
  }
  else {
      RxOut = 1;  // ������� �� ������
  }  
  ///
  return RxOut;
}
///
int hardIfaceUdp::Tx( const FormatMsg& scTx ) {

	// ��������������� �� ���
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

/// ������������� �����
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
// ����������� ��������
simpleCmd::simpleCmd(int NumCmd, int maxLen) {
  /// 1D 
  pCRC_ = new char[NumCmd];  // �������� ���������� ����
  pNumCharInCmd_ = new int[NumCmd];  // ����� ���� � ��������

  // �������������
  for(int i = 0; i < NumCmd; i++) {
    pCRC_[i] = '\0'; 
  }

  /// 2D array
  ppCmds_ = new char*[NumCmd];   // ������ ���������� �� ���� ��������
  for(int i = 0; i < NumCmd; i++) {  // ��������
    if(ppCmds_) ppCmds_[i] = new char[maxLen*2];  // � �������
  }
  NumCmdDel_  = NumCmd;
}

/// ������ ����������� ����
void simpleCmd::calkCRC(int Pos) {  // ��� ������� �� �������
  if(pCRC_) {
    pCRC_[Pos] = 0;  // ��������� �������� �����
    for(int i = 0; i < pNumCharInCmd_[Pos]; i++) {
      pCRC_[Pos] += ppCmds_[Pos][i];
    }
  }  // if(pCR..
}
/// ��������� ����������� ����
char simpleCmd::getCRC(int Pos) { 
    calkCRC(Pos);
    return pCRC_[Pos]; }

///  ��������� ��������
void simpleCmd::setCmd(int Pos, char* inBuf) {
  int Len = pNumCharInCmd_[Pos];
  for(int i = 0; i < Len; i++)
  {
    ppCmds_[Pos][i] = inBuf[i];  // ���������
  }
}
// ���������
char* simpleCmd::getCmdPtr(int Pos){  // ��������� ���������
  if(ppCmds_[Pos]) {
    return ppCmds_[Pos];  // ���������� ���������
  }
  else return NULL;  
}

/// ����� ���� � ��������
int simpleCmd::getCmdLen(int Pos) { return pNumCharInCmd_[Pos]; }
void simpleCmd::setCmdLen(int Pos, int Value) { pNumCharInCmd_[Pos] = Value; }

/// ����� �������
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
void IfaceSettings::TypeSet(int iPos, string cValue) {  // ����� �����. ��� ���.-����.
	int iStrLen = cValue.length();
	const char* pstr = cValue.c_str();
	if(ppType_) {
		ppType_[iPos] = new char[iStrLen+1];  // ��� ������� 
		for(int i = 0; i < iStrLen; i++)  // ����� � strcpy()
			ppType_[iPos][i] = pstr[i];
		ppType_[iPos][iStrLen] = '\0'; 
	}
}
///
char* IfaceSettings::cTypeGet(int iPos) {
  return ppType_[iPos];  // �������� ��� ��������!
}

///
IfaceSettings::IfaceSettings(int iNumInt, int iNumStr, char TypeConnect) { 
  cTypeConnect_ = TypeConnect;
  pType_ = new int[iNumInt];
  ppType_ = new char* [iNumStr];
  for(int i = 0; i < iNumInt; i++) {
    ppType_[i] = 0; }
  iNumStr_ = iNumStr;  // ����� �������������, ��� ��� ��������
}
IfaceSettings::IfaceSettings() {  // ����������� �� ��������� - �� ������ ������ 
  cTypeConnect_ = '\0';  // ..����� ���� �����. ���. ������� ����� �� ��������
  pType_ = NULL; ppType_ = NULL;
}
void IfaceSettings::setTypeConnect(char Type) { cTypeConnect_ = Type; }
char IfaceSettings::getTypeConnect(void) { return cTypeConnect_; }


