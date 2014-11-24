#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <cstring>

#include "StringMatcher.h"

using namespace std;

class StringMatcherProcessor {

private:
  static const int MAX_PATTERNS_BOYER_MOORE = 100;
  static const int BUFFER_SIZE = 50000;

  static int findMatchesTextFile (string& textFileName, 
                                  vector<StringMatcher*>& stringMatchers,
                                  bool count);

  static int processTextFileLine (char* buffer, 
                                  int length, 
                                  string& textFileName, 
                                  vector<StringMatcher*>& stringMatchers,
                                  bool count);

public:
  static void processParameters (int editDistance, 
                                string& patternFileName, 
                                bool count, 
                                string& pattern, 
                                vector<string>& textFilesName);

};