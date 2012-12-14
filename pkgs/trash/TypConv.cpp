/**
   compil. : с ключом /clr
*/
#include "StdAfx.h"
#include "TypConv.h"
using namespace Convertors;
using namespace System;
using namespace Runtime::InteropServices;
using namespace std;

///
int HexStrToChar(String^ Src, char* OutStr, int* len) {
    // удаляем пробелы
    int StrLen = Src->Length;
    String^ PureSrc = gcnew String("");  // сюда складываем непробелы
    for(int i = 0; i < StrLen; i++) {
        const wchar_t value = Src[i];
        if(value != ' ') {
          PureSrc += Src[i];
        }
    }
    int lenConv = PureSrc->Length;
    if((lenConv%2) != 0)
       return 1;  // нехватает пар
    //
    lenConv /= 2;  // сразу делем пополам
	*len = lenConv;  // длина результата
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
char HexWchar_tToChar(wchar_t src) {
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
    default : itog = 0; break;  // не найдено соответствие
  }
  //
  return itog;
}
///
String^ CharToHexStr(char src, String^ out) {
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
      sTmp += cTmp.ToString();  // старший байт
  }
  sTmp += " ";
  // сохраняем
  return sTmp;
}

/// <>
int MStringToArrChar(String ^s, char* cs) {
  using namespace NativeCode;
  cli::pin_ptr<const wchar_t> wch = PtrToStringChars(s); // чтобы работало нужно включить 
  // vcclr.h in stdafx.h 
  size_t Inp_size = wcslen(wch)+1;  // длинна юникодовского варианта
  size_t i = 0;
  wcstombs_s(&i, cs, Inp_size,  wch, _TRUNCATE);
  return 0;
}
///
void marshalStringA(System::String^ is, std::string & os){
  const char* chars = static_cast<const char*>((Marshal::StringToHGlobalAnsi(is)).ToPointer());
  os = chars;
  Marshal::FreeHGlobal(IntPtr((void*)(chars)));  // освобождаем        
}
void marshalStringW(System::String^ is, std::wstring & os){
  const wchar_t* chars = static_cast<const wchar_t*>((Marshal::StringToHGlobalUni(is)).ToPointer());
  os = chars;
  Marshal::FreeHGlobal(IntPtr((void*)(chars)));  // освобождаем        
}