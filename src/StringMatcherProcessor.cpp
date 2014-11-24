#include "StringMatcherProcessor.h"

#include "BoyerMooreMatcher.h"
#include "AhoCorasickMatcher.h"

void StringMatcherProcessor::processParameters (int editDistance, 
                                                string& patternFileName,
                                                bool count, 
                                                string& pattern, 
                                                vector<string>& textFilesName) {

  // Getting the list of patterns either from patternfile or pattern parameters
  vector<string> patterns;

  if (!pattern.empty()) {
    patterns.push_back(pattern);
  }
  else {
    ifstream patternFile(patternFileName);
    string patternInFile;

    while (std::getline(patternFile, patternInFile)) {
      patterns.push_back(patternInFile);
    }
  }
  
  vector<StringMatcher*> stringMatchers;
  int patternsCount = patterns.size();

  // Checking the type of matching algorithm to be used

  if (editDistance == 0) {// exact matching

    if (patternsCount <= MAX_PATTERNS_BOYER_MOORE) { // use BoyerMoore until this number of patterns

      // We have one string matcher for each pattern
      for (int i = 0; i < patternsCount; i++) {
        StringMatcher* stringMatcher = new BoyerMooreMatcher(patterns.at(i));
        stringMatchers.push_back(stringMatcher);
      }
    }
    else { // use Aho-Corasick for a larger number of patterns

      // We have only one string matcher for all the patterns
      StringMatcher* stringMatcher = new AhoCorasickMatcher(patterns);
      stringMatchers.push_back(stringMatcher);
    }
  }
  else { // approximate matching

    // TODO create matcher class for an algorithm which finds approximate matches (candidate: Wu-Manber)
  }

  // Finding matches for each textfile using the selected algorithm (implemented in stringMatcher)
  int totalMatchesCount;

  for (string textFileName : textFilesName) {
    totalMatchesCount = findMatchesTextFile(textFileName, stringMatchers, count);

    // if count option is set, the total matches count per file will be shown
    if(count) {
      cout << textFileName << ": " << totalMatchesCount << endl;
    } 
  }

  // Dealocating memory for string matchers
  for (StringMatcher* stringMatcher : stringMatchers) {
    delete stringMatcher;
  }
}


int StringMatcherProcessor::findMatchesTextFile (string& textFileName, 
                                                 vector<StringMatcher*>& stringMatchers,
                                                 bool count) {

  int totalMatchesCount = 0;
  ifstream textFile(textFileName);

  textFile.seekg(0, ios::end);
  ifstream::pos_type position = textFile.tellg();

  long long textFileSize = position;
  long long bufferSize = min((long long) BUFFER_SIZE, textFileSize);

  textFile.seekg(0, ios::beg);

  // Creating buffer to read text file by chunks
  char* buffer = new char[bufferSize];
  textFile.read(buffer, bufferSize);

  int stringEnd = -1;
  int stringStart;
  long long bufferPositionInFile = 0;

  while (bufferSize > 0) {
    int i = stringEnd + 1;
    stringStart = stringEnd;
    stringEnd = -1;
    
    for (; i < bufferSize && i + bufferPositionInFile < textFileSize; i++) {
      if (buffer[i] == '\n') {
        stringEnd = i;
        break;
      }
    }

    if (stringEnd == -1) { // we have to scroll the buffer

      if (stringStart == -1) { // line without \n to process
        totalMatchesCount += processTextFileLine(buffer + stringStart + 1, bufferSize, textFileName, stringMatchers, count);

        bufferPositionInFile += bufferSize;
        bufferSize = min(bufferSize, textFileSize - bufferPositionInFile);

        delete [] buffer;
        
        buffer = new char[bufferSize];
        textFile.read(buffer, bufferSize);
      }
      else{ // end of buffer reached
        int movedLength = bufferSize - stringStart - 1;
        memmove(buffer, buffer + stringStart + 1,movedLength);
        bufferPositionInFile += stringStart + 1;

        int readSize = min((long long) bufferSize - movedLength, textFileSize - bufferPositionInFile - movedLength);

        if (readSize != 0) {
          textFile.read(buffer + movedLength, readSize);
        }
          
        if (movedLength + readSize < bufferSize) {
          char *tempBuffer = new char[movedLength + readSize];
          memmove(tempBuffer, buffer, movedLength + readSize);

          delete [] buffer;
          buffer = tempBuffer;
          bufferSize = movedLength + readSize;
        }
          
        stringEnd = -1;
      }
    }
    else { // we have a complete text file line to process
      totalMatchesCount += processTextFileLine(buffer + stringStart + 1, stringEnd - stringStart, textFileName, stringMatchers, count);
    }
  }

  return totalMatchesCount;
}

int StringMatcherProcessor::processTextFileLine (char* buffer, 
                                                 int length,
                                                 string& textFileName, 
                                                 vector<StringMatcher*>& stringMatchers, 
                                                 bool count) {
  string nextTextFileLine = string(buffer, length);
  int matchesCount = 0;

  // Computing the matches for each string matcher (for one or more patterns)
  for (StringMatcher* stringMatcher : stringMatchers) {
    matchesCount += stringMatcher->findMatches(nextTextFileLine);
  }

  // print line if there are matches and count option is not set
  if (matchesCount > 0 && !count) {
    cout << textFileName << ": " << nextTextFileLine;

    if (nextTextFileLine.back() != '\n') {
      cout << endl;
    }
  }

  return matchesCount;
}
