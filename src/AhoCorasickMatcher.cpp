#include "AhoCorasickMatcher.h"

AhoCorasickMatcher::AhoCorasickMatcher(vector<string>& patterns) {
  this->patterns = patterns;
  preProcessPatterns();
}

AhoCorasickMatcher::~AhoCorasickMatcher(){
  delete fsm;
}

int AhoCorasickMatcher::findMatches(string& text) {
  int matchesCount = 0;
  int currentState = 0;

  int textLen = text.size();

  for (int i = 0; i < textLen; i++) {
    while (fsm->goTo.count(make_pair(currentState, text.at(i))) == 0) {
      currentState = fsm->fail[currentState];
    }

    currentState = fsm->goTo[make_pair(currentState, text.at(i))];

    if (fsm->occurrences.count(currentState) > 0) {
      matchesCount += fsm->occurrences[currentState].size();
    }
  }

  return matchesCount;
}

void AhoCorasickMatcher::preProcessPatterns() {
  fsm = buildFSM();
}

FSM* AhoCorasickMatcher::buildFSM() {
  FSM* fsm = new FSM;

  buildGoTo(fsm);
  buildFail(fsm);

  return fsm;
}

void AhoCorasickMatcher::buildGoTo(FSM* fsm) {
  int nextState = 0;
  int patternsCount = patterns.size();

  for (int i = 0; i < patternsCount; i++) {
    string pattern = patterns.at(i);

    int currentState = 0;
    int j = 0;
    int patternLen = pattern.size();

    while (j < patternLen && fsm->goTo.count(make_pair(currentState, pattern.at(j))) != 0) {
      currentState = fsm->goTo[make_pair(currentState, pattern.at(j))];
      j++;
    }

    while (j < patternLen) {
      nextState++;
      fsm->goTo[make_pair(currentState, pattern.at(j))] = nextState;
      currentState = nextState;
      j++;
    }

    set<int> currentStateOccurrences = fsm->occurrences[currentState];
    currentStateOccurrences.insert(i);
    fsm->occurrences[currentState] = currentStateOccurrences;
  }

  for (int i = 0; i < ALPHABET_LEN; i++) {
    char letter = (char) i;
    if (fsm->goTo.count(make_pair(0, letter)) == 0) {
      fsm->goTo[make_pair(0, letter)] = 0;
    }
  }
}

void AhoCorasickMatcher::buildFail(FSM* fsm) {
  queue<int> states;
  int nextState, failState;

  for (int i = 0; i < ALPHABET_LEN; i++) {
    char letter = (char) i;
    if (fsm->goTo[make_pair(0, letter)] != 0) {
      states.push(fsm->goTo[make_pair(0, letter)]);
      fsm->fail[fsm->goTo[make_pair(0, letter)]] = 0;
    }
  }

  while (!states.empty()) {
    int currentState = states.front();
    states.pop();

    for (int i = 0; i < ALPHABET_LEN; i++) {
      char letter = (char) i;
      
      if (fsm->goTo.count(make_pair(currentState, letter)) != 0) {
        nextState = fsm->goTo[make_pair(currentState, letter)];

        states.push(nextState);
        failState = fsm->fail[currentState];

        while (fsm->goTo.count(make_pair(failState, letter)) == 0) {
          failState = fsm->fail[failState];
        }

        fsm->fail[nextState] = fsm->goTo[make_pair(failState, letter)];

        set<int> nextStateOccurrences = fsm->occurrences[nextState];
        set<int> failStateOccurrences = fsm->occurrences[fsm->fail[nextState]];

        for(set<int>::iterator iterator = failStateOccurrences.begin(); iterator != failStateOccurrences.end(); iterator++) {
          nextStateOccurrences.insert(*iterator);
        }

        fsm->occurrences[nextState] = nextStateOccurrences;
      }
    }
  }
}