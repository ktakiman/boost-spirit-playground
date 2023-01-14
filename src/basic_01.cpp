#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>

void Clear() { std::cout << "\x1B[2J\x1B[H"; }

void TestBuildInSingleCharParsers(const std::string &s) {
  Clear();

  std::cout << "'" << s[0] << "'" << std::endl;

  bool is_digit = boost::spirit::qi::parse(std::begin(s), std::begin(s) + 1,
                                           boost::spirit::ascii::digit);

  bool is_xdigit = boost::spirit::qi::parse(std::begin(s), std::begin(s) + 1,
                                            boost::spirit::ascii::xdigit);

  bool is_alpha = boost::spirit::qi::parse(std::begin(s), std::begin(s) + 1,
                                           boost::spirit::ascii::alpha);

  bool is_alnum = boost::spirit::qi::parse(std::begin(s), std::begin(s) + 1,
                                           boost::spirit::ascii::alnum);

  std::cout << "is_digit:   " << std::boolalpha << is_digit << std::endl;
  std::cout << "is_xdigit:  " << std::boolalpha << is_xdigit << std::endl;
  std::cout << "is_alpha:   " << std::boolalpha << is_alpha << std::endl;
  std::cout << "is_alnum:   " << std::boolalpha << is_alnum << std::endl;
}

typedef void (*CALLBACK)(const std::string &);

void SubLoop(CALLBACK callback, const char *prompt) {
  while (true) {
    std::cout << std::endl << prompt << " > ";
    std::string s;
    std::cin >> s;

    if (s[0] == 'q') {
      break;
    }

    callback(s);
  }
}

int main(int argc, char *argv[]) {
  while (true) {
    Clear();
    std::cout << "'1' - test single char parser" << std::endl;
    std::cout << "'q' - quit" << std::endl;
    std::cout << "> ";

    std::string s;
    std::cin >> s;

    switch (s[0]) {
    case 'q':
      return 0;
    case '1':
      SubLoop(TestBuildInSingleCharParsers, "type a character");
      break;
    }
  }

  return 0;
}
