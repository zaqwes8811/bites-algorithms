#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

using namespace std;

int main() {
  string w1_1_filename = "../stanford_algoritms_part2/in_data/jobs.txt";
  fstream stream(w1_1_filename.c_str());
  if (!stream)
    throw runtime_error("Error: can't open file");
  
  return 0;
}