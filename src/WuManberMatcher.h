#include "StringMatcher.h"
#include <cmath>

using namespace std;

class WuManberMatcher: public StringMatcher {

private:
  static const int ALPHABET_LEN = 256;

  string pattern;
  int editDistance;
  unsigned long long* charMask;

  unsigned long long* buildCharMask();

public:
  WuManberMatcher(string& pattern, int editDistance);
  ~WuManberMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 