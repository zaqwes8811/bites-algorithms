#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>


using namespace std;

// istream::sentry example
#include <iostream>     // std::istream, std::cout
#include <string>       // std::string
#include <sstream>      // std::stringstream
#include <locale>       // std::isspace, std::isdigit

struct Phone {
  std::string digits;
};

// custom extractor for objects of type Phone
std::istream& operator>>(std::istream& is, Phone& tel)
{
    std::istream::sentry s(is);
    if (s) while (is.good()) {
      char c = is.get();
      if (std::isspace(c,is.getloc())) break;
      if (std::isdigit(c,is.getloc())) tel.digits += c;
    }
    return is;
}

int main() {
  string w1_1_filename = "../stanford_algoritms_part2/in_data/jobs.txt";
  fstream stream(w1_1_filename.c_str());
  if (!stream)
    throw runtime_error("Error: can't open file");
  
  string line;
  while (!stream.fail()) {
    getline(stream, line);  // разделитель что угодно
    
    //if (stream.peek() == ' ')
     // stream.ignore();
    
    cout << line << endl;
  }
  
  /// Extract objects
  // http://www.cplusplus.com/reference/istream/istream/sentry/
  std::stringstream parseme ("   (555)2326");
  Phone myphone;
  parseme >> myphone;
  std::cout << "digits parsed: " << myphone.digits << '\n';
  return 0;
}