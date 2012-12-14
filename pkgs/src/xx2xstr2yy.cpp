#include <win_snmp/stdafx.h>
#include "xx2xstr2yy.h"
using namespace std;
using namespace x2s2y;

/// /// ///
///
///
void x2s2y::getSysTime( vector<unsigned char>& trank ) {
  time_t now = time(0);
  tm *ltm = localtime(&now);
  int2uchar(trank, (1900 + ltm->tm_year), 4); trank.push_back('-');
  int2uchar(trank, 1 + ltm->tm_mon, 2); trank.push_back('-');
  int2uchar(trank, ltm->tm_mday, 2);  trank.push_back(' ');
  int2uchar(trank, ltm->tm_hour, 2);  trank.push_back(':');
  int2uchar(trank, ltm->tm_min, 2); trank.push_back(':');
  int2uchar(trank, ltm->tm_sec, 2);
}

/// ///
void x2s2y::int2uchar(vector<unsigned char>& trank, int value, int numSymb) {
  unsigned char uctmp = 0;
  int itmp = value;
  deque<unsigned char> dtmp;
  for(int i = 0; i < numSymb; i++) {
	  switch( itmp%10 ) 
	  {
	  case 0: uctmp = '0'; break;
	  case 1: uctmp = '1'; break;
	  case 2: uctmp = '2'; break;
	  case 3: uctmp = '3'; break;
	  case 4: uctmp = '4'; break;
	  case 5: uctmp = '5'; break;
	  case 6: uctmp = '6'; break;
	  case 7: uctmp = '7'; break;
	  case 8: uctmp = '8'; break;
	  case 9: uctmp = '9'; break;
	  }
	dtmp.push_front(uctmp);
	///
	itmp /= 10;  // Отбрасываем мледший 
  } 
  for(int i = 0; i < numSymb; i++) {
    trank.push_back(dtmp[i]);
  }
}
///

std::string x2s2y::to_utf8(const wchar_t* buffer, int len)
{
	int nChars = ::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		NULL,
		0,
		NULL,
		NULL);
	if (nChars == 0) return "";

	string newbuffer;
	newbuffer.resize(nChars) ;
	::WideCharToMultiByte(
		CP_UTF8,
		0,
		buffer,
		len,
		const_cast< char* >(newbuffer.c_str()),
		nChars,
		NULL,
		NULL); 

	return newbuffer;
}
std::string x2s2y::to_utf8(const std::wstring& str)
{
	return to_utf8(str.c_str(), (int)str.size());
}
