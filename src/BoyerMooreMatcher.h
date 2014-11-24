#include "StringMatcher.h"

using namespace std;

class BoyerMooreMatcher: public StringMatcher {

private:
  static const int ALPHABET_LEN = 256;

  string pattern;
  int* badChar;
  int* goodSuffix;

  int* buildBadChar();
  int* buildGoodSuffix();
  int* buildBorder(string& pattern);

public:
  BoyerMooreMatcher(string& pattern);
  ~BoyerMooreMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 