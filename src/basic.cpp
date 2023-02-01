#include "basic.h"
#include "helper.h"

#include <iostream>
#include <string>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

// --------------------------------------------------------------------------------
// #1 spirit.qi basic
// --------------------------------------------------------------------------------

// trying out a single character parser (are these called parsers, too?)
void TestBuildInSingleCharParsers(const std::string &s) {
  std::cout << "'" << s[0] << "'" << std::endl;

  // character parser -
  // https://www.boost.org/doc/libs/1_81_0/libs/spirit/doc/html/spirit/qi/quick_reference/qi_parsers/char.html

  bool is_digit = TryParse(s, ascii::digit);
  bool is_xdigit = TryParse(s, ascii::xdigit);
  bool is_alpha = TryParse(s, ascii::alpha);
  bool is_alnum = TryParse(s, ascii::alnum);

  PrintBool("is_digit:   ", is_digit);
  PrintBool("is_xdigit:  ", is_xdigit);
  PrintBool("is_alpha:   ", is_alpha);
  PrintBool("is_alnum:   ", is_alnum);

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

  PrintBool("is_int:    ", is_int);
  PrintBool("is_double: ", is_double);
  PrintBool("is_hex:    ", is_hex);
  PrintBool("is_foo:    ", is_foo);
}

// simple naked function
void F(double d) {
  std::cout << "Semantic action call to naked function!! " << d << std::endl;
}

// class with operator
class C {
public:
  void operator()(const double &d, qi::unused_type, qi::unused_type) const {
    std::cout << "Semantic action call to operator()!! " << d << std::endl;
  }

  void print(const double &d) {
    std::cout << "Semantic action call to C::print()!! " << d << std::endl;
  }
};

void TestSemanticAction(const std::string &s) {
  std::cout << "'" << s << "'" << std::endl;

  qi::parse(std::begin(s), std::end(s), qi::double_[F]);

  // can I use lambda? Yes!!
  auto sem_act = [](double d) {
    std::cout << "Semantic action call to lambda!! " << d << std::endl;
  };

  qi::parse(std::begin(s), std::end(s), qi::double_[sem_act]);

  qi::parse(std::begin(s), std::end(s), qi::double_[C()]);

  // this works but boost/bind.hpp generates compilation warning
  // C c;
  // qi::parse(std::begin(s), std::end(s),
  //           qi::double_[boost::bind(&C::print, &c, qi::_1)]);
}

void TestSimpleRule(const std::string &s) {
  // rule is composed of parsers or other rules
  auto rule = qi::char_('[') >> qi::double_ >>
              *(qi::char_(',') >> qi::double_) >> qi::char_(']');

  PrintResult((s + " (parse)       ").c_str(), TryParse(s, rule));

  // try phrase parser, too
  PrintResult((s + " (phrase_parse)").c_str(), TryPhraseParse(s, rule));
}

void TestAttributes(const std::string &s) {
  auto rule = qi::double_ % qi::char_(',');
  std::vector<double> v;

  auto iter = std::begin(s);
  auto end = std::end(s);

  bool parsed = qi::phrase_parse(iter, end, rule, ascii::space, v);

  PrintDoubles(v, parsed);
}
