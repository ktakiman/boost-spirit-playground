
#include "phoenix.h"
#include "helper.h"

#include <boost/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;

void JustTestPhoenix(const std::string &) {
  // https://www.boost.org/doc/libs/1_81_0/libs/phoenix/doc/html/phoenix/starter_kit.html

  // val just returns the
  auto v1 = phoenix::val(10);
  auto v2 = phoenix::val("howdy");

  int i = 100;
  auto r1 = phoenix::ref(i);

  auto a1 = phoenix::arg_names::arg2(v1, r1);

  std::cout << v1() << " " << v2() << " " << r1() << " " << a1() << std::endl;

  auto is_odd = phoenix::arg_names::arg1 % 2 == 1;

  std::cout << is_odd(1) << " " << is_odd(2) << std::endl;

  // can I write phoenix object which lazily creates a pair?
  auto make_pair = phoenix::construct<std::pair<int, int>>(
      phoenix::arg_names::arg1, phoenix::arg_names::arg2);

  auto pair = make_pair(1, 4);

  std::cout << pair.first << ", " << pair.second << std::endl;
}

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
  // <parser> % <parser> is special notation of character delimited list
  auto rule =
      qi::double_[phoenix::push_back(phoenix::ref(v), qi::_1)] % qi::char_(',');

  bool parsed = TryParse(s, rule);

  PrintDoubles(v, parsed);
}

void TestPhoenixWithVectorOfPairs(const std::string &s) {

  std::pair<double, double> temp;
  std::vector<std::pair<double, double>> pairs;

  auto pair = qi::char_('[') >>
              qi::double_[phoenix::ref(temp.first) = qi::_1] >>
              qi::char_(',') >>
              qi::double_[phoenix::ref(temp.second) = qi::_1] >> qi::char_(']');

  auto parser =
      pair[phoenix::push_back(phoenix::ref(pairs), phoenix::ref(temp))] %
      qi::char_(',');

  bool parsed = TryParse(s, parser);

  PrintBool("success? ", parsed);
  std::cout << "size: " << pairs.size() << std::endl;
  for (auto &p : pairs) {
    std::cout << "  " << p.first << ", " << p.second << std::endl;
  }
}
