#include <iomanip>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

// #include <boost/bind.hpp>
#include <boost/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;
namespace phoenix = boost::phoenix;

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

// don't really want to make it look extra complex, but this helper does both
// running parser and checking the iter position so it succeeds if the whole
// string is a match
template <typename Expr> bool TryParse(std::string_view s, const Expr &expr) {
  auto itr = std::begin(s);
  auto end = std::end(s);
  return qi::parse(itr, end, expr) && itr == end;
}

template <typename Expr>
bool TryPhraseParse(std::string_view s, const Expr &expr) {
  auto itr = std::begin(s);
  auto end = std::end(s);
  return qi::phrase_parse(itr, end, expr, ascii::space) && itr == end;
}

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

std::vector<const char *> gPresetSimpleRules = {
    "[3]", "[-4.5]", "[1,2,3]", "[1.3,-4.4,1.2345]", "[1, 2, 3, 4]", "[1 3, 2]",
};

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

// --------------------------------------------------------------------------------
// #2 boost.phoenix
// --------------------------------------------------------------------------------
void TestPhoenix(const std::string &s) {
  // takes two doubles separated by comma
  double d1 = 0.0;
  double d2 = 0.0;

  // use boost::phoenix to save parse result to a variable
  // not quite sure yet what else we can do inside '[...]'
  auto rule = qi::double_[phoenix::ref(d1) = qi::_1] >> qi::char_(',') >>
              qi::double_[phoenix::ref(d2) = qi::_1];

  bool parsed = TryParse(s, rule);

  std::cout << GetMark(parsed) << " " << d1 << ", " << d2 << std::endl;
}

void TestPhoenixWithVector(const std::string &s) {
  // takes two doubles separated by comma
  std::vector<double> v;

  // tring out a simpler syntax to define list of something
  auto rule =
      qi::double_[phoenix::push_back(phoenix::ref(v), qi::_1)] % qi::char_(',');

  bool parsed = TryParse(s, rule);

  PrintDoubles(v, parsed);
}

// --------------------------------------------------------------------------------
// #9 framework
// --------------------------------------------------------------------------------
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
    std::cout << "'5' - test attributes" << std::endl;
    std::cout << "'a' - test phoenix" << std::endl;
    std::cout << "'b' - test phoenix with vector" << std::endl;
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
    }
  }

  return 0;
}
