#include <iostream>

inline void log_error(int line, std::string message) {
  std::cerr << "Error on line " << line << ": " << message << std::endl;
}
