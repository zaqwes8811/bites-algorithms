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
			 // ����; ��������; ����� �� �������, ����� ���� ���� ������
			 // � ������ ������
	public : char Get(void);  // ��������� ���� �������
			 // ���������� � ������� ������ ������� � ����������
  };
	  




/// <���������� �������>
/// ���� �������� ���������� ������, ��� ������ ������ �� ���������� ���������
/// RS, TCP. � Udp ��� ������, ��� �� ��� � � ��������� �� ������ ���������
/// ��� �� ���������� ������� ����������� ����� ������. ! ������ ���� ������� � ������ ������� �������
/// </���������� �������>
class simpleCmd {
  private : int NumCmd_;  // ����� ������ � ����� - ������
  private : int NumCmdDel_;
  private : int* pNumCharInCmd_;  // ����� ���� � ��������
  private : char* pCRC_;  // ������ � ������������ ������� �� ����
  private : char** ppCmds_;  // 2D ������ � ���������
  
  /// <Methods>
  public : simpleCmd(int NumCmd, int maxLen);
  public : ~simpleCmd();
  
  // ���������
  // ���������� �����
  public : void calkCRC(int Pos);
  public : char getCRC(int Pos);
  // ��������
  public : void setNumCmd(int Value);
  public : int getNumCmd(void);  // �������� ����� ������
  // ���������� � ������
  public : void setCmd(int Pos, char* inBuf); // �����(� ����); ��. ������; ��� �����;
  public : char* getCmdPtr(int Pos);  // �����. ����. �� ������ � ��������
  // ����� �������
  public : int getCmdLen(int Pos);  // 
  public : void setCmdLen(int Pos, int Value);  // 
  /// ��������� 
   // �� �������� 1. ����� �������. 2. ����� ������ 3� ��������
};






/// <��������� ���������>
/// �� ���� ���������. ��������� � ��������� ��� ����������� ����������
/// ������� �� �� smart-�����������
/// </��������� ���������>
class IfaceSettings {
  private : char cTypeConnect_;
  private : int* pType_;  // �������� ���� �� �����, � ���� � �������
  private : char** ppType_;  // ������ � ��������� �����
  private : int iNumStr_;
  
  /// <Methods>
  public : IfaceSettings();  // ����������� �� ��������� - �� ������ ������ 
    // ..����� ���� �����. ���. ������� ����� �� ��������
  public : IfaceSettings(int iNumInt, int iNumStr, char TypeConnect);
  public : ~IfaceSettings();
  
  /// ���������
  public : char getTypeConnect(void);
  public : void setTypeConnect(char Type);
  public : void iTypeSet(int iPoint, int iValue);
  public : int iTypeGet(int iPoint);
  public : void cTypeSet(int iPos, const char* pstr, int iStrLen);
  public : char* cTypeGet(int iPos);
};

#pragma managed 
}
