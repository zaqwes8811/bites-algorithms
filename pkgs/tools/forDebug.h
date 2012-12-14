/**
  file : #include "forDebug.h"
*/
#ifndef _FOR_DEBUG
#define _FOR_DEBUG
  #include <fcntl.h>
  #include <iostream>
  #include <io.h>
#include <string>
#include <windows.h>
#include <uni_convertors/convertors.h>
namespace cpp_debug {
	/*
BOOL WINAPI GetConsoleScreenBufferInfo(
  __in   HANDLE hConsoleOutput,
  __out  PCONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo
);


typedef struct _CONSOLE_SCREEN_BUFFER_INFO {
  COORD dwSize;
  COORD dwCursorPosition;
  WORD wAttributes;
  SMALL_RECT srWindow;
  COORD dwMaximumWindowSize;
} CONSOLE_SCREEN_BUFFER_INFO;

*/
  // палитра
#ifdef WIN32
  typedef unsigned short Color;
  const Color NORMAL  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE;
   const Color WHITE   = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_INTENSITY;
            const Color RED     = FOREGROUND_RED|FOREGROUND_INTENSITY;
            const Color DKRED     = FOREGROUND_RED;
            const Color BLUE    = FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
            const Color DKBLUE    = FOREGROUND_BLUE|FOREGROUND_GREEN;
            const Color GREEN   = FOREGROUND_GREEN|FOREGROUND_INTENSITY;
            const Color DKGREEN   = FOREGROUND_GREEN;
            const Color YELLOW  = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
            const Color BROWN   = FOREGROUND_RED|FOREGROUND_GREEN;
#endif

  // 
  //extern CONSOLE_SCREEN_BUFFER_INFO gcsbiInfo;  // хранит исходные параметры консоли
  static HANDLE ghCrt;  // хэндл консоли
  void CreateConsole();

  // Errors
  void printError(std::string, unsigned char);

  // Warnings
  void printWarning(std::string, unsigned char);
  void printWarning(unsigned char);
  void printWarning(std::string strMsg);

  // Обычные сообщения
  void printSimple(std::string, unsigned char);
  void printSimple (std::string msg);
  void printSimple (unsigned char msg);

  // зеленое

}  // namespace
#endif
