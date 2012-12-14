// file : #include "TypConv.h"
#pragma once
#include "NativeCode.h"
#include <string>
//System:
namespace Convertors {
  int HexStrToChar(System::String^ Src, char*, 
			   int* len);
  char HexWchar_tToChar(wchar_t);
  System::String^ CharToHexStr(char, System::String^);
    ///

    /// String^ -> xstring
  void marshalStringA(System::String^ is, std::string & os);
  void marshalStringW(System::String^ is, std::wstring & os);
  /// Old
    // преобразователи
  int MStringToArrChar(System::String ^s, char* cs);
}
/*  Use:
  //
  tbMainParam->Text = "A1 22 33 44   44 55 66 77   88 99 99 99   11 22 33 44 "
    +"11 22 33 44   44 55 66 77   88 99 99 99   11 22 33 44 ";
  String^ Src = tbMainParam->Text;
  char* outStr = new char[100];
  tc_->HexStrToChar(Src, outStr, 100);
  
  // обратное преобразование
  char tmp = -0x68;
  tbMainParam->Text = "";
  for(int i = 0; i < 10; i++) {
    String^ stmp = tc_->CharToHexStr(tmp, stmp);
    tbMainParam->Text += stmp;
  }
 
  // удаление массива
  if(outStr) delete [] outStr;

*/
 /** String ^mps = gcnew String("Test");  // mps - managed pointer string
	    int iStLen = mps->Length;
	    char* pcs = new char[mps->Length+1];
	    int iErr;  // пусть пока будет
	    iErr = MStringToArrChar(mps, pcs);
	    String^ mpsTest = gcnew String(pcs); 
	    delete pcs;	*/