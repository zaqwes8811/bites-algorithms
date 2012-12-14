/**
  file : #include <uni_convertors/convertors.h>
*/
/*
  Конверторы стандартных типов

  thinks : 
    Для других наверное лучше сделать отдельное простр. 
	  или директивыми управлять
*/

#ifndef _CONVERTORS
#define _CONVERTORS
#define _GCC
#ifndef _GCC
/*
//
//
#ifndef _SECURE_ATL
  #define _SECURE_ATL 1
#endif
#ifndef VC_EXTRALEAN
  #define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit
// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS
  #include <afxwin.h>         // MFC core and standard components
  #include <afxext.h>         // MFC extensions
  #include <afxdisp.h>        // MFC Automation classes
#ifndef _AFX_NO_OLE_SUPPORT
  #include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
  #include <afxcmn.h>                     // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
  #include <afxsock.h>            // MFC socket extensions
#define _WIN32_DCOM 
//#define	DONT_SHOW_NOSYNC
#define		USE_EFW

// App cfg.
#define CODE_USE
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif
//
// 
*/
#endif  // _GCC

namespace Convertors {
#ifndef _GCC
  std::string CString2string(CString &);
#endif

  std::string replace_with(const std::string & src, const std::string & what, const std::string & with);
  double string2double( const std::string& src);

  ///
  std::string hl(char b1);  // не дороговато ли возвращать строку, может лучше ссылку передать
  std::string hlh(char b1, char b2);  // еще один простейший преобразователь
  std::string lhl(char b1, char b2); 
  void stringProcessing(std::string &s, unsigned int pos);  // xx(pos c 1) = x0x1(,pos)xpos...
  char hbCharToChar(char);  // 0x0y -> char число
  int charToInt(char);  // один символ 
  std::vector<std::string> SplitSpaces( std::string );

	// Prototype for conversion functions
	std::wstring	StringToWString(const std::string& s);
	std::string		WStringToString(const std::wstring& s);
#ifndef _GCC
	std::wstring Utf8ToUtf16(const std::string &s);
	std::string Utf16ToUtf8(const std::wstring &s);
	std::wstring ANSIToUtf16(const std::string &s);
	std::string Utf16ToANSI(const std::wstring &s);
	std::string ANSIToUtf8(const std::string &s);
	std::string Utf8ToANSI(const std::string &s);
#endif

	std::string stringAppendInt( std::string name, int value );

  template <typename T>
  std::string to_string(T const& value) {
    std::stringstream sstr;
    sstr << value;
    return sstr.str();
  }

  // из double в строку формата xxx.y
  //
  std::string double2str_dot_y(const double value);

  // из double в строку формата xxx.yy
  //
  std::string double2str_dot_yy(const double value);

  //
  //
  std::string int2str( int value );
}
#endif
