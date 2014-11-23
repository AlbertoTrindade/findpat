#include "StringMatcher.h"

using namespace std;

class BoyerMooreMatcher: public StringMatcher {

private:
  static const int ALPHABET_LEN = 256;

  string pattern;
  int* badChar;
  int* goodSuffix;

  int* createBadChar();
  int* createBorder(string& pattern);
  int* createGoodSuffix();

public:
  BoyerMooreMatcher(string& pattern);
  ~BoyerMooreMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 