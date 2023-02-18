#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

#include "basic.h"
#include "helper.h"
#include "phoenix.h"

typedef void (*CALLBACK)(const std::string &);

void SubLoop(CALLBACK callback, const char *prompt,
             const std::vector<const char *> *preset = nullptr) {
  while (true) {
    // try the preset if given
    if (preset != nullptr) {
      std::cout << std::endl << "--- preset ---" << std::endl;
      for (auto &s : *preset) {
        callback(s);
      }
      std::cout << "--------------" << std::endl;
    }

    std::cout << std::endl << prompt << " > ";
    std::string s;
    std::getline(std::cin, s);

    if (s[0] == 'q') {
      break;
    }

    Clear();
    callback(s);
  }
}

void SubLoop(CALLBACK callback, const char *prompt,
             const std::vector<const char *> &preset) {
  SubLoop(callback, prompt, &preset);
}

std::vector<const char *> gPresetSimpleRules = {
    "[3]", "[-4.5]", "[1,2,3]", "[1.3,-4.4,1.2345]", "[1, 2, 3, 4]", "[1 3, 2]",
};

int main(int argc, char *argv[]) {
  while (true) {
    Clear();
    std::cout << "'1' - test single char parser" << std::endl;
    std::cout << "'2' - test buildin parsers" << std::endl;
    std::cout << "'3' - test semantic action" << std::endl;
    std::cout << "'4' - test simple rule" << std::endl;
    std::cout << "'5' - test attributes" << std::endl;
    std::cout << "'a' - test phoenix" << std::endl;
    std::cout << "'b' - test phoenix with vector" << std::endl;
    std::cout << "'c' - test phoenix with vector of pairs" << std::endl;
    std::cout << "'x' - just test phoenix" << std::endl;
    std::cout << "'q' - quit" << std::endl;
    std::cout << "> ";

    std::string s;
    std::getline(std::cin, s);

    Clear();
    if (s == "q") {
      return 0;
    } else if (s == "1") {
      SubLoop(TestBuildInSingleCharParsers, "type a character");
    } else if (s == "2") {
      SubLoop(TestBuildInParsers, "type a word");
    } else if (s == "3") {
      SubLoop(TestSemanticAction, "type a number");
    } else if (s == "4") {
      SubLoop(TestSimpleRule, "type a list of doubles inide []",
              gPresetSimpleRules);
    } else if (s == "5") {
      SubLoop(TestAttributes, "type a list of doubles");
    } else if (s == "a") {
      SubLoop(TestPhoenix, "type two doubles");
    } else if (s == "b") {
      SubLoop(TestPhoenixWithVector, "type a list of doubles");
    } else if (s == "c") {
      SubLoop(TestPhoenixWithVectorOfPairs,
              "[double,double],[double,double],...");
    } else if (s == "x") {
      SubLoop(JustTestPhoenix, "type q to go back");
    }
  }

  return 0;
}
