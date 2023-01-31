#include "helper.h"

void Clear() { std::cout << "\x1B[2J\x1B[H"; }

void PrintBool(const char *header, bool b) {
  std::cout << header << std::boolalpha << b << std::endl;
}

std::string GetMark(bool result) { return result ? "✓" : "x"; }

void PrintResult(const char *text, bool result) {
  std::cout << GetMark(result) << " " << text << std::endl;
}

void PrintDoubles(const std::vector<double> &v, bool result) {
  std::cout << GetMark(result) << " size = " << v.size() << " | ";
  for (double d : v) {
    std::cout << d << ", ";
  }
  std::cout << std::endl;
}
