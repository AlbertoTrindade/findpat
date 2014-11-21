#include "BoyerMooreMatcher.h"

BoyerMooreMatcher::BoyerMooreMatcher(vector<string>& patterns) : StringMatcher(patterns){
  preProcessPatterns();
}

BoyerMooreMatcher::~BoyerMooreMatcher(){
  
}

int BoyerMooreMatcher::findMatches(string& text) {
  string pattern = patterns.at(0);
  int matchesCount = 0;

  return matchesCount;
}

void BoyerMooreMatcher::preProcessPatterns() {

}