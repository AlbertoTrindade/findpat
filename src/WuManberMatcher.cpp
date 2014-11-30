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

  unsigned long long msb = pow(2, patternLen - 1); // 00..0010..00: (64 - patternLen) bits zero, 1 bit one, (patternLen - 1) bits zero
  unsigned long long maskFilter = pow(2, patternLen) - 1; // 00..0011..11: (64 - patternLen) bits zero, patternLen bits one
  
  unsigned long long* bitMask = new unsigned long long[editDistance + 1];
  bitMask[0] = pow(2, patternLen) - 1; // 00..0011..11

  unsigned long long bitMaskPrevious = 0;
  unsigned long long bitMaskPreviousTemp = 0;

  for (int q = 1; q <= editDistance; q++) {
    bitMask[q] = bitMask[q-1] << 1;
  }

  for (int j = 0; j < textLen; j++) {
    bitMaskPrevious = bitMask[0];

    // Shift-or
    bitMask[0] = (bitMask[0] << 1) | charMask[(int) text.at(j)]; 

    for (int q = 1; q <= editDistance; q++) {
      bitMaskPreviousTemp = bitMask[q];
      bitMask[q] = ((bitMask[q] << 1) | charMask[(int) text.at(j)]) & (bitMaskPrevious << 1) & (bitMask[q - 1] << 1) & bitMaskPrevious;
      bitMaskPrevious = bitMaskPreviousTemp;
    }

    // Checking if left-most bit is zero, what implies in an approximate occurrence
    if ((bitMask[editDistance] & maskFilter) < msb) { // maskFilter used to discard (64 - patterLen) bitMask's first bits
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