#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

#include <boost/spirit/include/qi.hpp>

void Clear() { std::cout << "\x1B[2J\x1B[H"; }

void OutBool(const char *header, bool b) {
  std::cout << header << std::boolalpha << b << std::endl;
}

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

// don't really want to make it look extra complex, but this helper does both
// running parser and checking the iter position so it succeeds if the whole
// string is a match
template <typename Expr> bool TryParse(std::string_view s, const Expr &expr) {
  auto itr = std::begin(s);
  auto end = std::end(s);
  return qi::parse(itr, end, expr) && itr == end;
}

// trying out a single character parser (are these called parsers, too?)
void TestBuildInSingleCharParsers(const std::string &s) {
  std::cout << "'" << s[0] << "'" << std::endl;

  // character parser -
  // https://www.boost.org/doc/libs/1_81_0/libs/spirit/doc/html/spirit/qi/quick_reference/qi_parsers/char.html

  bool is_digit = TryParse(s, ascii::digit);
  bool is_xdigit = TryParse(s, ascii::xdigit);
  bool is_alpha = TryParse(s, ascii::alpha);
  bool is_alnum = TryParse(s, ascii::alnum);

  OutBool("is_digit:   ", is_digit);
  OutBool("is_xdigit:  ", is_xdigit);
  OutBool("is_alpha:   ", is_alpha);
  OutBool("is_alnum:   ", is_alnum);

  // other character sets
  //   boost::spirit::iso8859_1
  //   boost::spirit::standard
  //   boost::spirit::standard_wide
}

void TestBuildInParsers(const std::string &s) {
  std::cout << "'" << s << "'" << std::endl;

  // numeric parsers -
  // https://www.boost.org/doc/libs/1_81_0/libs/spirit/doc/html/spirit/qi/quick_reference/qi_parsers/numeric.html

  bool is_int = TryParse(s, qi::int_);
  bool is_double = TryParse(s, qi::double_);
  bool is_hex = TryParse(s, qi::hex);
  bool is_foo = TryParse(s, qi::lit("foo"));

  OutBool("is_int:    ", is_int);
  OutBool("is_double: ", is_double);
  OutBool("is_hex:    ", is_hex);
  OutBool("is_foo:    ", is_foo);
}

typedef void (*CALLBACK)(const std::string &);

void SubLoop(CALLBACK callback, const char *prompt) {
  while (true) {
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

int main(int argc, char *argv[]) {
  while (true) {
    Clear();
    std::cout << "'1' - test single char parser" << std::endl;
    std::cout << "'2' - test buildin parsers" << std::endl;
    std::cout << "'q' - quit" << std::endl;
    std::cout << "> ";

    std::string s;
    std::getline(std::cin, s);

    if (s == "q") {
      return 0;
    } else if (s == "1") {
      SubLoop(TestBuildInSingleCharParsers, "type a character");
    } else if (s == "2") {
      SubLoop(TestBuildInParsers, "type a word");
    }
  }

  return 0;
}
