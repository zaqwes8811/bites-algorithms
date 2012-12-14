/**
	file : #include "utils.h" 
	
	TODO :
		из строки в число, желательно не берез itoa
		как сделать дробилку стандартной строки любым символом?
		
*/
#ifndef _UTILS
#define _UTILS

namespace utils {
void print( int* array, int N );
void print( int, int );
void print( std::string, int, int );
void print( std::string , int );
void print( std::string );
void print( std::string , int, std::string , int );
void print( std::string , int, std::string , int, std::string , int );
void print( std::vector<std::string> content );

std::string int2string( int );
std::vector<std::string> file2list( std::string fname );
std::vector<std::string> split_spaces( std::string str );
}
#endif //_UTILS