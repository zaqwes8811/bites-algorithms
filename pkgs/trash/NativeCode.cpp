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
// конструктор комманды
simpleCmd::simpleCmd(int NumCmd, int maxLen) {
  /// 1D 
  pCRC_ = new char[NumCmd];  // значение контольных сумм
  pNumCharInCmd_ = new int[NumCmd];  // число байт в комманде

  // инициализация
  for(int i = 0; i < NumCmd; i++) {
    pCRC_[i] = '\0'; 
  }

  /// 2D array
  ppCmds_ = new char*[NumCmd];   // массив указателей на сами комманды
  for(int i = 0; i < NumCmd; i++) {  // выделяем
    if(ppCmds_) ppCmds_[i] = new char[maxLen*2];  // с запасом
  }
  NumCmdDel_  = NumCmd;
}

/// расчет контрольных сумм
void simpleCmd::calkCRC(int Pos) {  // раз вызвали то считаем
  if(pCRC_) {
    pCRC_[Pos] = 0;  // обнуление значения суммы
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
    ppCmds_[Pos][i] = inBuf[i];  // заполняем
  }
}
// указатель
char* simpleCmd::getCmdPtr(int Pos){  // получение указателя
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
void IfaceSettings::cTypeSet(int iPos, const char* pstr, int iStrLen) {  // длина перед. без нул.-симв.
  if(ppType_) {
    ppType_[iPos] = new char[iStrLen+1];  // все символы 
    for(int i = 0; i < iStrLen; i++)  // можно и strcpy()
      ppType_[iPos][i] = pstr[i];
      ppType_[iPos][iStrLen] = '\0'; }
    }
///
char* IfaceSettings::cTypeGet(int iPos) {
  return ppType_[iPos];  // непоятно как защитить!
}

///
IfaceSettings::IfaceSettings(int iNumInt, int iNumStr, char TypeConnect) { 
  cTypeConnect_ = TypeConnect;
  pType_ = new int[iNumInt];
  ppType_ = new char* [iNumStr];
  for(int i = 0; i < iNumInt; i++) {
    ppType_[i] = 0; }
  iNumStr_ = iNumStr;  // будет предполагатся, что все выделено
}
IfaceSettings::IfaceSettings() {  // конструктор по умолчанию - на случай вызова 
  cTypeConnect_ = '\0';  // ..много чего вызыв. дин. поэтому нужно бы защитить
  pType_ = NULL; ppType_ = NULL;
}
void IfaceSettings::setTypeConnect(char Type) { cTypeConnect_ = Type; }
char IfaceSettings::getTypeConnect(void) { return cTypeConnect_; }

/// <MyQueue>
/// </MyQueue>
void MyQueue::Add(char Key, char Value, bool OneNote) {
  /*
    1. проверяем нужно ли добавить
    2. если да, то на том добавляем
    3. если нет, то считаем сколько с такими ключаме есть комманд
    4. если 0 - просто добавляем
    5. если 1 - ничего не далаем
    6. если >1 - удалем и просто добавляем
  */
  if(OneNote) {  // сразу добавляем
    mm_.insert(std::pair<char, char>(Key, Value));
  } else {  // команда должна быть единственной
    std::size_t count = mm_.count(Key);
    if(count == 1) { return; }
    else {  // в любом случае добавляем
      if(count > 1) {  // удаляем
        mm_.erase(Key);
      }
      mm_.insert(std::pair<char, char>(Key, Value)); 
    } 
  }
}
///
char MyQueue::Get(void) {
  if(mm_.begin() == mm_.end()) 
    return 127;  // вообще-то доступ к
  char Cmd(0);
  std::pair<char, char> beginPair= *(mm_.begin());
  Cmd = beginPair.second;
  mm_.erase(mm_.begin());
  return Cmd;
}