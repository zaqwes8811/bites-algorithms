/**
	file : #include "utils.h" 
*/
#ifndef _UTILS
#define _UTILS

#include <string>
#include <iostream>
#include <sstream>

void print( int* array, int N );
void print( int, int );
void print( std::string, int, int );
void print( std::string , int );
void print( std::string );
void print( std::string , int, std::string , int );
void print( std::string , int, std::string , int, std::string , int );

std::string int2string( int );

#endif //_UTILS