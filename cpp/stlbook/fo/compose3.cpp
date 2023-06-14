#include <iostream>
#include <algorithm>
#include <functional>
#include <locale>
#include <string>

using namespace std;
using namespace std::placeholders;

char myTouper(char c)
{
  std::locale loc;
  return std::use_facet<std::ctype<char>>(loc).toupper(c);
}

int main()
{
  string s("Internationalization");
  string sub("Nation");

  string::iterator pos;
  pos = search(s.begin(), s.end(), sub.begin(), sub.end(),
               std::bind(equal_to<char>(), bind(myTouper, _1), bind(myTouper, _2)));
  if (pos != s.end()) {
    std::cout << "\"" << sub << "\" found at " << s << '\n';
  }
}