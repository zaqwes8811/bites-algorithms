#include "StdAfx.h"
#include "TypConv.h"
#include "NativeCode.h"
using namespace Convertors;
using namespace System;
using namespace Runtime::InteropServices;
using namespace std;
TypConv::TypConv(void)
{
}
int TypConv::charToInt(char cTmp_0) {
  int Itog = 0;
    char cTmp_1 = cTmp_0;
  cTmp_1 %= 16; cTmp_1 &= 0x0f;
  Itog = static_cast<int>(cTmp_1);  // 0-16
  cTmp_1 = cTmp_0; cTmp_1 >>= 4; cTmp_1 &= 0x0f;
  Itog += (16*static_cast<int>(cTmp_1)); 
  return Itog;
}
///
int TypConv::HexStrToChar(String^ Src, char* OutStr, int* len) {
    // ������� �������
    int StrLen = Src->Length;
    String^ PureSrc = gcnew String("");  // ���� ���������� ���������
    for(int i = 0; i < StrLen; i++) {
        const wchar_t value = Src[i];
        if(value != ' ') {
          PureSrc += Src[i];
        }
    }
    int lenConv = PureSrc->Length;
    if((lenConv%2) != 0)
       return 1;  // ��������� ���
    //
    lenConv /= 2;  // ����� ����� �������
	*len = lenConv;  // ����� ����������
    int i = 0; 
    int j = 0;
    char cTmp_0 = 0;
    char cTmp_1 = 0;
    for(int i = 0; i < lenConv; i++) {
      cTmp_0 = HexWchar_tToChar(PureSrc[j]);
      cTmp_0 &= 0x0f; cTmp_0 <<= 4;
      cTmp_1 = HexWchar_tToChar(PureSrc[j+1]);
      cTmp_1 &= 0x0f; cTmp_1 ^= cTmp_0; 
      OutStr[i] = cTmp_1;
      j += 2;
    }
    //cTmp_1 = OutStr[lenConv-1];
    // 
    return 0;
}
///
char TypConv::HexWchar_tToChar(wchar_t src) {
  char itog = 0;
  ///
  switch(src) {
    case '0' : itog = 0; break;
    case '1' : itog = 1; break;
    case '2' : itog = 2; break;
    case '3' : itog = 3; break;
    case '4' : itog = 4; break;
    case '5' : itog = 5; break;
    case '6' : itog = 6; break;
    case '7' : itog = 7; break;
    case '8' : itog = 8; break;
    case '9' : itog = 9; break;
    case 'A' : itog = 10; break;
    case 'B' : itog = 11; break;
    case 'C' : itog = 12; break;
    case 'D' : itog = 13; break;
    case 'E' : itog = 14; break;
    case 'F' : itog = 15; break;
    default : itog = 0; break;  // �� ������� ������������
  }
  //
  return itog;
}
///
String^ TypConv::CharToHexStr(char src, String^ out) {
  char cTmp = 0; String^ sTmp = gcnew String("");
  for(int i = 0; i < 2; i++) { 
    cTmp = src;
    if(i == 0) { cTmp >>= 4; cTmp &= 0x0f; }
    else cTmp &= 0x0f; 
    if(cTmp > 9)
      switch(cTmp){
        case 10: sTmp += "A"; break;
        case 11: sTmp += "B"; break;
        case 12: sTmp += "C"; break;
        case 13: sTmp += "D"; break;
        case 14: sTmp += "E"; break;
        case 15: sTmp += "F"; break;
        default : sTmp += "X"; break;
      }
    else
      sTmp += cTmp.ToString();  // ������� ����
  }
  sTmp += " ";
  // ���������
  return sTmp;
}

/// <>
int MyTypeConvertor::MStringToArrChar(String ^s, char* cs) {
  using namespace NativeCode;
  cli::pin_ptr<const wchar_t> wch = PtrToStringChars(s); // ����� �������� ����� �������� 
  // vcclr.h in stdafx.h 
  size_t Inp_size = wcslen(wch)+1;  // ������ ������������� ��������
  size_t i = 0;
  wcstombs_s(&i, cs, Inp_size,  wch, _TRUNCATE);
  return 0;
}

//////////
std::string TypConv::hl(char b1){ 
  char tmp(b1);
  tmp >>= 4; tmp &= 0x0f;
  ///
  string s;
  s.push_back(hbCharToChar(tmp));
  tmp = b1; tmp &= 0x0f;
  s.push_back(hbCharToChar(tmp));
  
  //
  return s;
}
std::string TypConv::hlh(char b1, char b2) 
{
  char tmp(b1);
  tmp >>= 4; tmp &= 0x0f;  // ������� ��������
  string s;
  s.push_back(hbCharToChar(tmp));
  tmp = b1; tmp &= 0x0f;  // ������� �������
  s.push_back(hbCharToChar(tmp));
  tmp = b2;
  tmp >>= 4; tmp &= 0x0f;  // ������� �������� �������
  s.push_back(hbCharToChar(tmp));

  //
  return s;
}
std::string TypConv::lhl(char b1, char b2){
  char tmp(b1);
  tmp &= 0x0f;  // ������� �������� �������
  string s;
  s.push_back(hbCharToChar(tmp));
  tmp = b2; 
  tmp >>= 4; tmp &= 0x0f;  // ������� �������
  s.push_back(hbCharToChar(tmp));
  tmp = b2;
  tmp &= 0x0f;  // ������� �������
  s.push_back(hbCharToChar(tmp));

  //
  return s;
}
///
void TypConv::stringProcessing(std::string &s, unsigned int pos) {
  // ��������� �������
  if(pos != 0)
    s.insert(pos, ",");
  else 
    pos = s.size();

  // ������� �������� ����
  string::iterator ini(s.begin());
  for(unsigned int i = 0; i < pos-1; i++) {
    if(*s.begin() == '0') 
      s.erase(ini);
    else break;
  }

}
//
char TypConv::hbCharToChar(char tmp) {
  char out = '\0';
  tmp &= 0x0f; // �� ������ ������
  switch(tmp) {  // 0-15 - ������ ��������
    case 0 : { out = '0'; } break;
    case 1 : { out = '1'; } break;
    case 2 : { out = '2'; } break;
    case 3 : { out = '3'; } break;
    case 4 : { out = '4'; } break;
    case 5 : { out = '5'; } break;
    case 6 : { out = '6'; } break;
    case 7 : { out = '7'; } break;
    case 8 : { out = '8'; } break;
    case 9 : { out = '9'; } break;
    case 10 : { out = 'a'; } break;
    case 11 : { out = 'b'; } break;
    case 12 : { out = 'c'; } break;
    case 13 : { out = 'd'; } break;
    case 14 : { out = 'e'; } break;
    case 15 : { out = 'f'; } break;
    default : {} break;
  }

  ///
  return out;
}

///
void TypConv::marshalStringA(System::String^ is, std::string & os){
  const char* chars = static_cast<const char*>((Marshal::StringToHGlobalAnsi(is)).ToPointer());
  os = chars;
  Marshal::FreeHGlobal(IntPtr((void*)(chars)));  // �����������        
}
void TypConv::marshalStringW(System::String^ is, std::wstring & os){
  const wchar_t* chars = static_cast<const wchar_t*>((Marshal::StringToHGlobalUni(is)).ToPointer());
  os = chars;
  Marshal::FreeHGlobal(IntPtr((void*)(chars)));  // �����������        
}