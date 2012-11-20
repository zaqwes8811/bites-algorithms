 
#include "utils.h" 
using namespace std;
void print( int* array, int N ){
	//for (int i = 0; i < N; ++i)	cout << array[i] << " ";
	//cout << endl;
}
void print( int p, int q ) {
	cout << "Pair: " << p << " "<< q << endl;
}

void print( std::string name, int i ) {
	cout << name+" : " << i << endl;
}

void print( std::string str ) {
	cout << str;
}

void print( std::string msg, int p, int q ) {
	cout << msg+" : " << p << " "<< q << endl;
}

void print( std::string m1, int a1, std::string m2, int a2 ) {
	cout << m1+" : " << a1 << " " << m2+" : " << a2 << endl;
}
void print( std::string m1, int a1, std::string m2, int a2,
	std::string m3, int a3
 ) {
	cout << m1+" : " << a1 << 
	  " " << m2+" : " << a2 << 
	  " " << m3+" : " << a3 << 
	  endl;
}
std::string int2string( int value ) {
	std::stringstream out;
	out << value;
	return out.str();
}