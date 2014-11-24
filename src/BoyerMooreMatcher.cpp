#include "BoyerMooreMatcher.h"

BoyerMooreMatcher::BoyerMooreMatcher(string& pattern) {
  this->pattern = pattern;
  preProcessPatterns();
}

BoyerMooreMatcher::~BoyerMooreMatcher(){
  delete [] badChar;
  delete [] goodSuffix;
}

int BoyerMooreMatcher::findMatches(string& text) {
  int matchesCount = 0;

  int patternLen = pattern.size();
  int textLen = text.size();

  int index = 0;
  int indexPat = 0;

  int indexText = 0;

  while(index <= textLen - patternLen) {
    indexPat = patternLen - 1;

    while((indexPat >= 0) && (text.at(index + indexPat) == pattern.at(indexPat))) {
      indexPat--;
    }

    if(indexPat < 0) {
      index += goodSuffix[0];
      matchesCount++;
    } 
    else {
      indexText = text.at(index + indexPat);
      index += max(indexPat - badChar[indexText], goodSuffix[indexPat]);
    }
  }

  return matchesCount;
}

void BoyerMooreMatcher::preProcessPatterns() {
  badChar = buildBadChar();
  goodSuffix = buildGoodSuffix();

}

int* BoyerMooreMatcher::buildBadChar() {

  int patternLen = pattern.size();
  int* badChar = new int[ALPHABET_LEN];
  int indexBC = 0;

  for (int i = 0; i < ALPHABET_LEN; i++) {
    badChar[i] = -1;
  }

  for (int j = 0; j < patternLen; j++) {
    indexBC = pattern.at(j);
    badChar[indexBC] = j;
  }

  return badChar;
}

int* BoyerMooreMatcher::buildGoodSuffix() {

  int patternLen = pattern.length();
  string reversedPattern = string(pattern.rbegin(), pattern.rend());

  int* border = buildBorder(pattern);
  int* reversedBorder = buildBorder(reversedPattern);

  int suffixLen = patternLen + 1;
  int* suffix = new int[suffixLen];

  for (int i = 0; i < suffixLen; i++)
    suffix[i] = patternLen - border[patternLen];

  int index = 0;

  for (int i = 1; i < patternLen; i++) {
    index = patternLen - 1 - reversedBorder[i];

    if((index < patternLen) && (index >= 0)) {
      if((suffix[index] > i - reversedBorder[i])) {
        suffix[index] = i - reversedBorder[i];
      }
    } 
    else {
      i = patternLen;
    }
  }

  delete [] border;
  delete [] reversedBorder;

  return suffix;
}

int* BoyerMooreMatcher::buildBorder(string& pattern) {

  int patternLen = pattern.size();
  int* border = new int[patternLen + 1];
  
  int index = 1;
  int indexPat = 0;

  border[0] = -1;

  for(int i = 1; i < (patternLen + 1); i++) {
    border[i] = 0;
  }

  while(index < patternLen) {
    indexPat = 0;

    while((index + indexPat < patternLen) && (pattern.at(index + indexPat) == pattern.at(indexPat))) {
      indexPat++;
      border[index + indexPat] = indexPat;
    }
    index += indexPat - border[indexPat];
  }

  return border;
}