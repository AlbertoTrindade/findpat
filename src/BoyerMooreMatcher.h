#include "StringMatcher.h"

using namespace std;

class BoyerMooreMatcher: public StringMatcher {

public:
  BoyerMooreMatcher(vector<string>& patterns);
  ~BoyerMooreMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 