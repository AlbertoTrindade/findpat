#include "StringMatcher.h"

using namespace std;

class AhoCorasickMatcher: public StringMatcher {

private:
  vector<string> patterns;

public:
  AhoCorasickMatcher(vector<string>& patterns);
  ~AhoCorasickMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 