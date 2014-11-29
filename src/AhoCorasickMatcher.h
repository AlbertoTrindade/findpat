#include "StringMatcher.h"

#include <map>
#include <set>
#include <queue>

using namespace std;

struct AC_FSM {
  map<int, int*> goTo;
  map<int, int> fail;
  map<int, set<int>> occurrences; 
};

class AhoCorasickMatcher: public StringMatcher {

private:
  static const int ALPHABET_LEN = 256;

  vector<string> patterns;
  AC_FSM* fsm;

  AC_FSM* buildFSM();
  void buildGoTo(AC_FSM* fsm);
  void buildFail(AC_FSM* fsm);

public:
  AhoCorasickMatcher(vector<string>& patterns);
  ~AhoCorasickMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 