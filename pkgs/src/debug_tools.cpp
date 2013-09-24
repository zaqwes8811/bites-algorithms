#include "stdafx.h"
#include "forDebug.h"
using namespace cpp_debug;
using namespace std;

/// /// ///
CONSOLE_SCREEN_BUFFER_INFO gcsbiInfo;
void cpp_debug::CreateConsole() {
#ifdef _CONSOLE 
	
	if (AllocConsole()) 
    { 
	  ghCrt = GetStdHandle(STD_OUTPUT_HANDLE);
      int ighCrt = (_open_osfhandle((long)ghCrt, 4)); 
      *stdout = *(::_fdopen(ighCrt, "w")); 
      ::setvbuf(stdout, NULL, _IONBF, 0); 
      *stderr = *(::_fdopen(ighCrt, "w")); 
      ::setvbuf(stderr, NULL, _IONBF, 0);
      std::ios::sync_with_stdio();

	  // 
	  GetConsoleScreenBufferInfo(ghCrt, &gcsbiInfo);
    }
#endif
}

void cpp_debug::printSimple(std::string msg)
{
#ifdef _CONSOLE
	cout << msg;// << endl;
#endif
}

void cpp_debug::printSimple (unsigned char msg) 
{
#ifdef _CONSOLE
	cout << Convertors::hl( static_cast<int>( msg ) );
#endif
}

void cpp_debug::printError(std::string strMsg, unsigned char charMsg)
{
#ifdef _CONSOLE

	// настраиваем цвет консоли
	SetConsoleTextAttribute(ghCrt, FOREGROUND_RED|FOREGROUND_INTENSITY);
	cout << strMsg << Convertors::hl( static_cast<int>( charMsg ) );// << endl;
	
	// откатываемся
	SetConsoleTextAttribute(ghCrt, gcsbiInfo.wAttributes);
#endif
}
void cpp_debug::printWarning(std::string strMsg, unsigned char charMsg) 
{
#ifdef _CONSOLE
	
	// настраиваем цвет консоли
	SetConsoleTextAttribute(ghCrt, YELLOW);
	cout << strMsg << Convertors::hl( static_cast<int>( charMsg ) );// << endl;
	
	// откатываемся
	SetConsoleTextAttribute(ghCrt, gcsbiInfo.wAttributes);
#endif
}

void cpp_debug::printSimple(std::string strMsg, unsigned char charMsg) 
{
#ifdef _CONSOLE
	cout << strMsg << Convertors::hl( static_cast<int>( charMsg ) );// << endl;
#endif
}

void cpp_debug::printWarning(unsigned char charMsg) 
{
	// настраиваем цвет консоли
	SetConsoleTextAttribute(ghCrt, YELLOW);
	cout << Convertors::hl( static_cast<int>( charMsg ) )  << " ";
	
	// откатываемся
	SetConsoleTextAttribute(ghCrt, gcsbiInfo.wAttributes);
}
void cpp_debug::printWarning(std::string strMsg) {
	// настраиваем цвет консоли
	SetConsoleTextAttribute(ghCrt, YELLOW);
	cout << strMsg << " ";
	
	// откатываемся
	SetConsoleTextAttribute(ghCrt, gcsbiInfo.wAttributes);
}
