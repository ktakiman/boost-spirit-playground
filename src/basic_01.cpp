#include <iostream>
#include <string>

#include <boost/spirit/include/qi.hpp>

void TestBuildInSingleCharParsers(char ch) {
  char buf[] = {ch, 0};

  bool is_digit = boost::spirit::qi::parse(std::begin(buf), std::end(buf),
                                           boost::spirit::ascii::digit);

  bool is_xdigit = boost::spirit::qi::parse(std::begin(buf), std::end(buf),
                                            boost::spirit::ascii::xdigit);

  bool is_alpha = boost::spirit::qi::parse(std::begin(buf), std::end(buf),
                                           boost::spirit::ascii::alpha);

  bool is_alnum = boost::spirit::qi::parse(std::begin(buf), std::end(buf),
                                           boost::spirit::ascii::alnum);

  std::cout << "is_digit:   " << std::boolalpha << is_digit << std::endl;
  std::cout << "is_xdigit:  " << std::boolalpha << is_xdigit << std::endl;
  std::cout << "is_alpha:   " << std::boolalpha << is_alpha << std::endl;
  std::cout << "is_alnum:   " << std::boolalpha << is_alnum << std::endl;
}

int main(int argc, char *argv[]) {

  while (true) {
    std::string s;
    std::cout << "> ";
    std::cin >> s;

    if (s[0] == 'q') {
      break;
    }

    TestBuildInSingleCharParsers(s[0]);
  }

  return 0;
}
