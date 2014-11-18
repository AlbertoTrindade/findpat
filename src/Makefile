MYPROGRAM = findpat

SOURCES = main.cpp
INCLUDES = ../libs/
TARGET_DIR = ../bin/

CC = g++
CFLAGS = -std=c++0x -Wall -g

RM = rm -r

#---------------------------------------------

all:
	mkdir -p $(TARGET_DIR)
	$(CC) $(CFLAGS) $(SOURCES) -I $(INCLUDES)  -Wall -o $(TARGET_DIR)$(MYPROGRAM)

clean:
	$(RM) $(TARGET_DIR)