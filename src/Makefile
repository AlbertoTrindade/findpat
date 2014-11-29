MYPROGRAM = findpat

SOURCES = main.cpp StringMatcherProcessor.cpp StringMatcher.cpp BoyerMooreMatcher.cpp AhoCorasickMatcher.cpp UkkonenMatcher.cpp WuManberMatcher.cpp
INCLUDES = ../libs/
TARGET_DIR = ../bin/

CC = g++
CFLAGS = -std=c++0x -Wall -g

MKDIR = mkdir -p
RM = rm -r

#---------------------------------------------

all:
	$(MKDIR) $(TARGET_DIR)
	$(CC) $(CFLAGS) $(SOURCES) -I $(INCLUDES)  -Wall -o $(TARGET_DIR)$(MYPROGRAM)

clean:
	$(RM) $(TARGET_DIR)