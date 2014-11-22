#include "StringMatcher.h"

using namespace std;

class BoyerMooreMatcher: public StringMatcher {

private:
  string pattern;

public:
  BoyerMooreMatcher(string& pattern);
  ~BoyerMooreMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 