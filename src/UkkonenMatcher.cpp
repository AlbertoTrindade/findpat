#include "UkkonenMatcher.h"

UkkonenMatcher::UkkonenMatcher(string& pattern, int editDistance) {
  this->pattern = pattern;
  this->editDistance = editDistance;

  preProcessPatterns();
}

UkkonenMatcher::~UkkonenMatcher(){
  int statesCount = fsm->delta.size();

  for (int i = 0; i < statesCount; i++) {
    delete fsm->delta[i + 1];
  }

  delete fsm;
}

int UkkonenMatcher::findMatches(string& text) {
  int matchesCount = 0;

  int textLen = text.size();
  int currentState = 0;

  for (int i = 0; i < textLen; i++) {
    currentState = fsm->delta[currentState][(int) text.at(i)];

    if (fsm->acceptStates.count(currentState) > 0) { // currentState is an accept state
      matchesCount++;
    }
  }

  return matchesCount;
}

void UkkonenMatcher::preProcessPatterns() {
  fsm = buildFSM();
}

Ukk_FSM* UkkonenMatcher::buildFSM() {
  Ukk_FSM* fsm = new Ukk_FSM;

  int patternLen = pattern.size();
  vector<int> currentState;

  for (int i = 0; i < (patternLen + 1); i++) {
    currentState.push_back(i);
  }

  // index that maps a column (a m+1 array) to corresponding state's number
  map<vector<int>, int> index;
  int nextIndex = 0;

  index[currentState] = nextIndex;

  // Initializing array with next states' index for every alphabet letter for first state
  fsm->delta[nextIndex] = new int[ALPHABET_LEN]();

  // queue with columns represented by states
  queue<vector<int>> states;
  states.push(currentState);

  if (currentState.at(patternLen) <= editDistance) {
    fsm->acceptStates.insert(index[currentState]);
  }

  while (!states.empty()) {
    currentState = states.front();
    states.pop();

    for (int i = 0; i < ALPHABET_LEN; i++) {
      vector<int> nextState = computeNextState(currentState, (char) i);

      if (index.count(nextState) == 0) {
        states.push(nextState);

        nextIndex++;
        index[nextState] = nextIndex;

        // Creating array for each new state
        fsm->delta[nextIndex] = new int[ALPHABET_LEN]();

        if (nextState.at(patternLen) <= editDistance) {
          fsm->acceptStates.insert(index[nextState]);
        }
      }

      fsm->delta[index[currentState]][i] = index[nextState];
    }
  }

  return fsm;
}

vector<int> UkkonenMatcher::computeNextState(vector<int>& currentState, char letter) {
  int patternLen = pattern.size();
  vector<int> nextState;

  nextState.push_back(0);

  for (int i = 1; i < (patternLen + 1); i++) {
    int nextValue;

    // nextState[i] is defined as a minimum of four values:
    nextValue = min(currentState.at(i - 1) + ((pattern.at(i - 1) == letter)? 0 : 1), currentState.at(i) + 1);
    nextValue = min(nextValue, nextState.at(i - 1) + 1);
    nextValue = min(nextValue, editDistance + 1);

    nextState.push_back(nextValue);
  }

  return nextState;
}