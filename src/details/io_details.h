#ifndef DETAILS_IO_DETAILS_H
#define DETAILS_IO_DETAILS_H

#include <string>
#include <vector>
#include <sstream>

namespace io_details {
struct Arrow {
  const int end;
  const int weight;
  
  Arrow(int end_, int weight_) : end(end_), weight(weight_) { } 
}; 

std::pair<int, std::vector<Arrow> > parse_node_data(const std::string& line, std::stringstream& ss);

std::vector<int> extract_records(const std::string& filename);

}
#endif