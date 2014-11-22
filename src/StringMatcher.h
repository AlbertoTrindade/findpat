#pragma once

#include <string>
#include <vector>

using namespace std;

class StringMatcher {

public:
  StringMatcher();
  virtual ~StringMatcher(){};
  virtual int findMatches(string& text)=0;
  virtual void preProcessPatterns()=0;
};