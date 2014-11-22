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

  // Checking the type of matching algorithm to be used
  
  StringMatcher* stringMatcher = NULL;

  if (editDistance == 0) {// exact matching

    if (patterns.size() == 1) { // use algorithm for one pattern
      stringMatcher = new BoyerMooreMatcher(pattern);
    }
    else { // use algorithm for more than one patterns
      stringMatcher = new AhoCorasickMatcher(patterns);
    }
  }
  else { // approximate matching

    // TODO create matcher class for an algorithm which finds approximate matches (candidate: Wu-Manber)
  }

  // Finding matches for each textfile using the selected algorithm (implemented in stringMatcher)
  int totalMatchesCount;

  for (string textFileName : textFilesName) {
    totalMatchesCount = findMatchesTextFile(textFileName, stringMatcher, count);

    // if count option is set, the total matches count per file will be shown
    if(count) {
      cout << textFileName << ": " << totalMatchesCount << endl;
    } 
  }

  delete stringMatcher;
}


int StringMatcherProcessor::findMatchesTextFile (string& textFileName, 
                                                 StringMatcher* stringMatcher,
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
        totalMatchesCount += processTextFileLine(buffer + stringStart + 1, bufferSize, textFileName, stringMatcher, count);

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
      totalMatchesCount += processTextFileLine(buffer + stringStart + 1, stringEnd - stringStart, textFileName, stringMatcher, count);
    }
  }

  return totalMatchesCount;
}

int StringMatcherProcessor::processTextFileLine (char* buffer, 
                                                 int length,
                                                 string& textFileName, 
                                                 StringMatcher* stringMatcher, 
                                                 bool count) {
  string nextTextFileLine = string(buffer, length);
  int matchesCount = stringMatcher->findMatches(nextTextFileLine);

  // print line if there are matches and count option is not set
  if (matchesCount > 0 && !count) {
    cout << textFileName << ": " << nextTextFileLine;

    if (nextTextFileLine.back() != '\n') {
      cout << endl;
    }
  }

  return matchesCount;
}
