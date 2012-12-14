#include "StdAfx.h"
#include "NativeCode.h"
using namespace NativeCode;
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
void IfaceSettings::iTypeSet(int iPoint, int iValue) { 
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
void IfaceSettings::cTypeSet(int iPos, const char* pstr, int iStrLen) {  // ����� �����. ��� ���.-����.
  if(ppType_) {
    ppType_[iPos] = new char[iStrLen+1];  // ��� ������� 
    for(int i = 0; i < iStrLen; i++)  // ����� � strcpy()
      ppType_[iPos][i] = pstr[i];
      ppType_[iPos][iStrLen] = '\0'; }
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

/// <MyQueue>
/// </MyQueue>
void MyQueue::Add(char Key, char Value, bool OneNote) {
  /*
    1. ��������� ����� �� ��������
    2. ���� ��, �� �� ��� ���������
    3. ���� ���, �� ������� ������� � ������ ������� ���� �������
    4. ���� 0 - ������ ���������
    5. ���� 1 - ������ �� ������
    6. ���� >1 - ������ � ������ ���������
  */
  if(OneNote) {  // ����� ���������
    mm_.insert(std::pair<char, char>(Key, Value));
  } else {  // ������� ������ ���� ������������
    std::size_t count = mm_.count(Key);
    if(count == 1) { return; }
    else {  // � ����� ������ ���������
      if(count > 1) {  // �������
        mm_.erase(Key);
      }
      mm_.insert(std::pair<char, char>(Key, Value)); 
    } 
  }
}
///
char MyQueue::Get(void) {
  if(mm_.begin() == mm_.end()) 
    return 127;  // ������-�� ������ �
  char Cmd(0);
  std::pair<char, char> beginPair= *(mm_.begin());
  Cmd = beginPair.second;
  mm_.erase(mm_.begin());
  return Cmd;
}