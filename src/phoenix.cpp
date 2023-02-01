
#include "phoenix.h"
#include "helper.h"

#include <boost/phoenix.hpp>
#include <boost/spirit/include/qi.hpp>

namespace phoenix = boost::phoenix;
namespace qi = boost::spirit::qi;

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
