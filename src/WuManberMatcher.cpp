#include "WuManberMatcher.h"

WuManberMatcher::WuManberMatcher(string& pattern, int editDistance) {
  this->pattern = pattern;
  this->editDistance = editDistance;

  preProcessPatterns();
}

WuManberMatcher::~WuManberMatcher(){
  delete [] charMask;
}

int WuManberMatcher::findMatches(string& text) {
  int matchesCount = 0;

  int patternLen = pattern.size();
  int textLen = text.size();

  unsigned long long msb = pow(2, patternLen-1);
  unsigned long long maskFilter = pow(2, patternLen) - 1;

  unsigned long long* bitMask = new unsigned long long[editDistance + 1];
  bitMask[0] = pow(2, patternLen) - 1;

  unsigned long long bitMaskTemp1 = 0;
  unsigned long long bitMaskTemp2 = 0;

  for (int q = 1; q <= editDistance; q++) {
    bitMask[q] = bitMask[q-1] << 1;
  }

  for (int j = 0; j < textLen; j++) {
    bitMaskTemp1 = bitMask[0];
    bitMask[0] = (bitMask[0] << 1) | charMask[(int) text.at(j)];

    for (int q = 1; q <= editDistance; q++) {
      bitMaskTemp2 = bitMask[q];
      bitMask[q] = ((bitMask[q] << 1) | charMask[(int) text.at(j)]) & (bitMaskTemp1 << 1) & (bitMask[q-1] << 1) & bitMaskTemp1;
      bitMaskTemp1 = bitMaskTemp2;
    }

    if ((bitMask[editDistance] & maskFilter) < msb) {
      matchesCount++;
    }
  }

  delete [] bitMask;

  return matchesCount;
}

void WuManberMatcher::preProcessPatterns() {
  charMask = buildCharMask();
}

unsigned long long*  WuManberMatcher::buildCharMask() {

  unsigned long long* charMask = new unsigned long long[ALPHABET_LEN]();
  unsigned long long mask = 1;

  int patternLen = pattern.size();

  for (int i = 0; i < ALPHABET_LEN; i++) {
    charMask[i] = pow(2, patternLen) - 1;
  }

  for (int k = 0; k < patternLen; k++) {
    charMask[(int) pattern.at(k)] &= ~mask;
    mask <<= 1;
  }

  return charMask;
}