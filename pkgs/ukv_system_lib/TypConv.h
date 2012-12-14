// file : #include "TypConv.h"
#pragma once
//#include <marshal_cppstd.h>
#include < string >
//System:
namespace Convertors {
  public ref class TypConv 
  {
    public : TypConv(void);
    public : int charToInt(char);  // один символ 
  public : int HexStrToChar(System::String^ Src, char*, 
			   int* len);
    public : char HexWchar_tToChar(wchar_t);
    public : System::String^ CharToHexStr(char, System::String^);
    ///
    public : std::string hl(char b1);  // не дороговато ли возвращать строку, может лучше ссылку передать
    public : std::string hlh(char b1, char b2);  // еще один простейший преобразователь
    public : std::string lhl(char b1, char b2); 
    public : void stringProcessing(std::string &s, unsigned int pos);  // xx(pos c 1) = x0x1(,pos)xpos...
    public : char hbCharToChar(char);  // 0x0y -> char число

    /// String^ -> xstring
    public : void marshalStringA(System::String^ is, std::string & os);
    public : void marshalStringW(System::String^ is, std::wstring & os);
  };

  /// Old
  public ref class MyTypeConvertor {
    // преобразователи
    public : int MStringToArrChar(System::String ^s, char* cs);
  
  };
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