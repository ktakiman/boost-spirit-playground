#pragma once

#include <iostream>
#include <string_view>
#include <vector>

#include <boost/spirit/include/qi.hpp>

namespace qi = boost::spirit::qi;
namespace ascii = boost::spirit::ascii;

std::string GetMark(bool result);

void Clear();

void PrintBool(const char *header, bool b);
void PrintResult(const char *text, bool result);
void PrintDoubles(const std::vector<double> &v, bool result);

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
