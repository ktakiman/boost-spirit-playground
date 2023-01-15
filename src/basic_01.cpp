#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>

// #include <boost/bind.hpp>
#include <boost/spirit/include/qi.hpp>

void Clear() { std::cout << "\x1B[2J\x1B[H"; }

void PrintBool(const char *header, bool b) {
  std::cout << header << std::boolalpha << b << std::endl;
}

void PrintResult(const char *text, bool result) {
  std::cout << (result ? "✓ " : "x ") << text << std::endl;
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
  //           qi::double_[boost::bind(&C::print, &c, _1)]);
}

std::vector<const char *> gPresetSimpleRules = {
    "[3]",
    "[-4.5]",
    "[1,2,3]",
    "[1.3,-4.4,1.2345]",
};

void TestSimpleRule(const std::string &s) {
  // rule is composed of parsers or other rules
  auto rule = qi::char_('[') >> qi::double_ >>
              *(qi::char_(',') >> qi::double_) >> qi::char_(']');

  PrintResult(s.c_str(), TryParse(s, rule));
}

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

int main(int argc, char *argv[]) {
  while (true) {
    Clear();
    std::cout << "'1' - test single char parser" << std::endl;
    std::cout << "'2' - test buildin parsers" << std::endl;
    std::cout << "'3' - test semantic action" << std::endl;
    std::cout << "'4' - test simple rule" << std::endl;
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
      SubLoop(TestSimpleRule, "type a list of doubles", gPresetSimpleRules);
    }
  }

  return 0;
}
