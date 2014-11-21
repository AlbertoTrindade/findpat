#pragma once

#include <string>
#include <vector>

using namespace std;

class StringMatcher {

protected:
  vector<string> patterns;

public:
  StringMatcher(vector<string>& patterns);
  ~StringMatcher();
  virtual int findMatches(string& text)=0;
  virtual void preProcessPatterns()=0;
};