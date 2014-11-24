#include "AhoCorasickMatcher.h"

AhoCorasickMatcher::AhoCorasickMatcher(vector<string>& patterns) {
  this->patterns = patterns;
  preProcessPatterns();
}

AhoCorasickMatcher::~AhoCorasickMatcher(){
  int statesCount = fsm->goTo.size();

  for (int i = 0; i < statesCount; i++) {
    delete fsm->goTo[i + 1];
  }

  delete fsm;
}

int AhoCorasickMatcher::findMatches(string& text) {
  int matchesCount = 0;
  int currentState = 1;

  int textLen = text.size();

  for (int i = 0; i < textLen; i++) {
    while (fsm->goTo[currentState][(int) text.at(i)] == 0) {
      currentState = fsm->fail[currentState];
    }

    currentState = fsm->goTo[currentState][(int) text.at(i)];

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
  int nextState = 1;
  int patternsCount = patterns.size();

  // Initializing array with next states for every alphabet letter for first state
  fsm->goTo[nextState] = new int[ALPHABET_LEN]();

  for (int i = 0; i < patternsCount; i++) {
    string pattern = patterns.at(i);

    int currentState = 1; // first state is numbered as 1
    int j = 0;
    int patternLen = pattern.size();

    while (j < patternLen && fsm->goTo.count(currentState) > 0 && fsm->goTo[currentState][(int) pattern.at(j)] != 0) {
      currentState = fsm->goTo[currentState][(int) pattern.at(j)];
      j++;
    }

    while (j < patternLen) {
      nextState++;

      fsm->goTo[currentState][(int) pattern.at(j)] = nextState;
      currentState = nextState;
      j++;

      // Creating array for each new state
      fsm->goTo[nextState] = new int[ALPHABET_LEN]();
    }

    set<int> currentStateOccurrences = fsm->occurrences[currentState];
    currentStateOccurrences.insert(i);
    fsm->occurrences[currentState] = currentStateOccurrences;
  }

  for (int i = 0; i < ALPHABET_LEN; i++) {
    if (fsm->goTo[1][i] == 0) { // 0 is the fail state
      fsm->goTo[1][i] = 1;
    }
  }
}

void AhoCorasickMatcher::buildFail(FSM* fsm) {
  queue<int> states;
  int nextState, failState;

  for (int i = 0; i < ALPHABET_LEN; i++) {
    if (fsm->goTo[1][i] != 1) {
      states.push(fsm->goTo[1][i]);
      fsm->fail[fsm->goTo[1][i]] = 1;
    }
  }

  while (!states.empty()) {
    int currentState = states.front();
    states.pop();

    for (int i = 0; i < ALPHABET_LEN; i++) {
      if (fsm->goTo[currentState][i] != 0) {
        nextState = fsm->goTo[currentState][i];

        states.push(nextState);
        failState = fsm->fail[currentState];

        while (fsm->goTo[failState][i] == 0) {
          failState = fsm->fail[failState];
        }

        fsm->fail[nextState] = fsm->goTo[failState][i];

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