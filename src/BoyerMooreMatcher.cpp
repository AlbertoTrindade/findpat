#include "BoyerMooreMatcher.h"

BoyerMooreMatcher::BoyerMooreMatcher(string& pattern) {
  this->pattern = pattern;
  preProcessPatterns();
}

BoyerMooreMatcher::~BoyerMooreMatcher(){
  
}

int BoyerMooreMatcher::findMatches(string& text) {
  int matchesCount = 0;

  return matchesCount;
}

void BoyerMooreMatcher::preProcessPatterns() {

}