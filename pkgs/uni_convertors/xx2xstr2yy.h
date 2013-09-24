/// file : #include "xx2xstr2yy.h"
#ifndef _X2S2Y
#define _X2S2Y

#include <string>
#include <iostream>
#include <memory>
#include <ctime>
#include <deque>
#include <vector>
#include < windows.h >
namespace x2s2y 
{
  
/// /// ///
///
/// ¬ класс собрать нужно бы, может потом пригодитс€
  void int2uchar(std::vector<unsigned char>& trank, int value, int numSymb);
  void getSysTime(std::vector<unsigned char>& trank);
  /*
   vector<unsigned char> dt;
   getSysTime(dt);
   vector<unsigned char>::iterator at;
   for(at = dt.begin(); at != dt.end(); at++)
     cout << *at;
  */
  std::string to_utf8(const wchar_t* buffer, int len);
  std::string to_utf8(const std::wstring& str);
};

#endif _X2S2Y