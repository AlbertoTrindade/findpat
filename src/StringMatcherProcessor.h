#include <string>
#include <vector>

#include <iostream>
#include <fstream>
#include <cstring>

#include "StringMatcher.h"

using namespace std;

class StringMatcherProcessor {

private:
  static const int BUFFER_SIZE = 50000;

  static int findMatchesTextFile (string& textFileName, 
                                  StringMatcher* stringMatcher,
                                  bool count);

  static int processTextFileLine (char* buffer, 
                                  int length, 
                                  string& textFileName, 
                                  StringMatcher* stringMatcher,
                                  bool count);

public:
  static void processParameters (int editDistance, 
                                string& patternFileName, 
                                bool count, 
                                string& pattern, 
                                vector<string>& textFilesName);

};