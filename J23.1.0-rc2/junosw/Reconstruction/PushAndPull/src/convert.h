#ifndef CONVERT_HH
#define CONVERT_HH
#include <sstream>

template<class out_type,class in_value>
out_type convert(const in_value& t)
{
  std::stringstream ss;
  ss << t;
  out_type result;
  ss >> result;
  return result;
}
#endif  // convert.h
