#include "StringMatcher.h"

#include <map>
#include <set>
#include <queue>

using namespace std;

struct Ukk_FSM {
  map<int, int*> delta;
  set<int> acceptStates;
};

class UkkonenMatcher: public StringMatcher {

private:
  static const int ALPHABET_LEN = 256;

  string pattern;
  int editDistance;
  Ukk_FSM* fsm;

  Ukk_FSM* buildFSM();
  vector<int> computeNextState(vector<int>& currentState, char letter);

public:
  UkkonenMatcher(string& pattern, int editDistance);
  ~UkkonenMatcher();
  int findMatches(string& text);
  void preProcessPatterns();
}; 