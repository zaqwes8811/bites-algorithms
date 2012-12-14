// C++
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>
#include <iterator>

// App
#include <utils/utils.h>

namespace utils {
using std::vector;
using std::string;
using std::ifstream;
using std::cout;
using std::endl;
using std::istream_iterator;
using std::istringstream;
using std::back_inserter;

void print( int* array, int N ){
  //for (int i = 0; i < N; ++i)  cout << array[i] << " ";
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

void print( std::vector<std::string> content ) {
  vector<string>::iterator at = content.begin();
  for(; at != content.end(); ++at ) {
    cout << *at << endl;
  }
}

std::string int2string( int value ) {
  std::stringstream out;
  out << value;
  return out.str();
}

std::vector<std::string> file2list( std::string fname ) {
  ifstream infile(fname.c_str());
  const int RD_BUFFER_SIZE = 80;
  char b[RD_BUFFER_SIZE];
  vector<string> result;
  while(1) {
    infile.getline(b, sizeof(b));
    string oneLine(b);
    
    // обрабатываем результаты
    result.push_back(oneLine);
    
    if(infile.eof()) {
      break;  
    }
  }
  return result;
}

std::vector<std::string> split_spaces( std::string str ) {
  vector<string> tokens;
  istringstream iss( str );
  copy(istream_iterator<string>(iss),
         istream_iterator<string>(),
         back_inserter<vector<string> >(tokens));
  return tokens;
}
}