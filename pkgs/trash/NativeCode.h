/**
  file : #include "NativeCode.h"
  using namespace NativeCode;
*/
#pragma once
#include <string>
#include < map >
#include < windows.h >
namespace NativeCode {
  #pragma unmanaged 
  /// <MyQueue>
  /// </MyQueue>
  class MyQueue {
  private : std::multimap<char, char> mm_;
	public : void Add(char Key, char Value, bool OneNote); 
			 // ключ; значение; нужно ли следить, чтобы была одна запись
			 // с данным ключем
	public : char Get(void);  // получение кода команды
			 // возвращает и удаляет первый элемент в контейнере
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
  public : void iTypeSet(int iPoint, int iValue);
  public : int iTypeGet(int iPoint);
  public : void cTypeSet(int iPos, const char* pstr, int iStrLen);
  public : char* cTypeGet(int iPos);
};

#pragma managed 
}
