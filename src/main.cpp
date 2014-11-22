#include "StringMatcherProcessor.h"
#include "optionparser.h"

#include <iostream>

#include <glob.h>
#include <string>
#include <vector>

using namespace std;

void addExpandedFiles(string& wildcardedFile, vector<string>& files);

struct Arg : public option::Arg
{
  static option::ArgStatus NonEmpty (const option::Option& option, bool message);
  static option::ArgStatus NonNegative(const option::Option& option, bool message);
};

enum programOptions {UNKNOWN, HELP, EDIT, PATTERNFILE, COUNT};

const option::Descriptor usage[] =
  {
    {UNKNOWN, 0, "" , "", Arg::None, "USAGE: findpat [OPTIONS] PATTERN TEXTFILE [TEXTFILE...]" },
    {UNKNOWN, 0, "" , "", Arg::None, "Search for PATTERN in each TEXTFILE\n"
                                             "If --pattern option is set, a list of patterns will be used instead of PATTERN\n"
                                             "Multiple files can be indicated for TEXTFILE by using wildcards\n\n"
                                             "Options:" },
    {HELP, 0, "h" , "help", Arg::None, "  -h, --help  \tPrint usage and exit" },
    {EDIT, 0, "e", "edit", Arg::NonNegative, "  -e, --edit  \tSpecify a maximun edit distance to find approximate occurrences of PATTERN"
                                                              " or patterns specified by --pattern option, instead of the default exact ones" },
    {PATTERNFILE, 0, "p", "pattern", Arg::NonEmpty, "  -p, --pattern  \tSpecify a file with patterns to be searched, one per line," 
                                                                          " instead of using PATTERN" },
    {COUNT, 0, "c", "count", Arg::None, "  -c, --count  \tInstead of printing the lines in which the patterns occur,"
                                                             " the total count of occurrences per file will be shown" },
    {UNKNOWN, 0, "",  "", Arg::None, "\nExamples:\n"
                                             "  findpat ababc textfile1.txt textfile2.txt\n"
                                             "  findpat -e 2 -p patterfile.txt ababc textfile.txt\n" },
    {0,0,0,0,0,0}
 };

int main(int argc, char** argv) {

  // Optional parameters
  int editDistance;
  string patternFile;
  bool count;

  // Required parameters (positional options)
  string pattern;
  vector<string> textFiles;

  // Skip program name
  if (argc > 0) {
    argc -= 1;
    argv += 1;
  }

  // Initializing option parser structures
  option::Stats  stats(usage, argc, argv);
  option::Option options[stats.options_max], buffer[stats.buffer_max];
  option::Parser parse(usage, argc, argv, options, buffer);

  // Error in option parsing
  if (parse.error()){
    return 1;
  }

  // Print usage table if help option was set or no options at all
  if (options[HELP] || argc == 0) {
    option::printUsage(cout, usage);

    return 0;
  }

  // Help option
  if (options[EDIT]) {
    editDistance = strtol(options[EDIT].last()->arg, NULL, 10);
  }

  // Pattern file option
  if (options[PATTERNFILE]) {
    patternFile = options[PATTERNFILE].last()->arg;
  }

  // Count option
  count = options[COUNT];

  // Positional options (required parameters)

  int positionalOptionsCount = parse.nonOptionsCount();

  if (options[PATTERNFILE]) { // if patternfile is set, we have only TEXTFILE as positional options
    if (positionalOptionsCount == 0) {
      cerr << "You have to specity at least a textfile for TEXTFILE" << endl;

      return 1;
    }
    else{
      for (int i = 0; i < parse.nonOptionsCount(); i++){
        // all the positional options are TEXTFILE
        string textFile = parse.nonOption(i);

        if (textFile.find("*") != string::npos) { // textFile has wildcard, let us expand the files
         addExpandedFiles(textFile, textFiles);
        }
        else { // single file
          textFiles.push_back(textFile);
        }
      }
    }
  }
  else { // if patternfile is not set, positional option PATTERN needs to be
    if (positionalOptionsCount == 0) {
      cerr << "You have to specify a pattern for PATTERN" << endl;

      return 1;
    }
    else if (positionalOptionsCount == 1) {
      cerr << "You have to specity at least a textfile for TEXTFILE" << endl;

      return 1;
    }
    else {
      for (int i = 0; i < parse.nonOptionsCount(); i++){

        if (i == 0) { // PATTERN is the first one
          pattern = parse.nonOption(i);
        }
        else { // The remaining are TEXTFILE
          string textFile = parse.nonOption(i);

          if (textFile.find("*") != string::npos) { // textFile has wildcard
            addExpandedFiles(textFile, textFiles);
          }
          else {
            textFiles.push_back(textFile);
          }
        }
      }
    }
  }

  // Unknown options
  for (option::Option* option = options[UNKNOWN]; option; option = option->next()) {
    cout << "Unknown option: " << option->name << endl;
  }

  StringMatcherProcessor::processParameters(editDistance, patternFile, count, pattern, textFiles);

  return 0;
}

void addExpandedFiles(string& wildcardedFile, vector<string>& files) {
  
  glob_t globResult;

  glob(wildcardedFile.c_str(), GLOB_TILDE, NULL, &globResult);
    
  // Adding each expanded file to files
  for(unsigned int i = 0; i < globResult.gl_pathc; i++){
    files.push_back(string(globResult.gl_pathv[i]));
  }

  globfree(&globResult);
}

// Checking logic for non-empty argument
option::ArgStatus Arg::NonEmpty(const option::Option& option, bool message) {

  if (option.arg != 0 && option.arg[0] != 0) {
    return option::ARG_OK;
  }

  if (message) {
    (cerr << "Option ").write(option.name, option.namelen) << " requires a non-empty argument" << endl;
  }

  return option::ARG_ILLEGAL;
}

// Checking logic for non-negative argument
option::ArgStatus Arg::NonNegative(const option::Option& option, bool message) {

  char* p = 0;

  if (option.arg) {
    double v = strtol(option.arg, &p, 10);
    if (p != option.arg && !*p && v >= 0) {
      return option::ARG_OK;
    }
  }

  if (message) {
    (cerr << "Option ").write(option.name, option.namelen) << " requires a non-negative numeric argument" << endl;
  }

  return option::ARG_ILLEGAL;
}