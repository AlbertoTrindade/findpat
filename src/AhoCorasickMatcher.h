#include "StringMatcher.h"

#include <map>
#include <set>
#include <queue>

using namespace std;

struct FSM {
  map<int, int*> goTo;
  map<int, int> fail;
  map<int, set<int>> occurrences; 
};

class AhoCorasickMatcher: public StringMatcher {

private:
  static const int ALPHABET_LEN = 256;

  vector<string> patterns;
  FSM* fsm;

  FSM* buildFSM();
  void buildGoTo(FSM* fsm);
  void buildFail(FSM* fsm);

public:
  AhoCorasickMatcher(vector<string>& patterns);
  ~AhoCorasickMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 