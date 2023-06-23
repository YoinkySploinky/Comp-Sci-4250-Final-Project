#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include "token.h"
#include "scanner.h"

using namespace std;

const int TABLEROWS = 9, TABLECOLS = 9, NUMSPECIALCHARS = 9;

int driverTable[TABLEROWS][TABLECOLS] = {
  {1, -1, -1, -1, 5, -1, 4, 0, 1004},
  {-1, -1, -1, 2, -1, -1, -1, -1, -1},
  {-1, -1, 3, -1, -1, -1, -1, -1, -1},
  {1001, 3, 3, 1001, 1001, 1001, 1001, 1001, 1001},
  {1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002, 1002},
  {-1, 6, 6, -1, -1, -1, -1, -1, -1},
  {1003, 6, 6, 1003, 1003, 7, 1003, 1003, 1003},
  {-1, 8, 8, -1, -1, -1, -1, -1, -1},
  {1003, 8, 8, 1003, 1003, 1003, 1003, 1003, 1003},
};

char charCheck[NUMSPECIALCHARS] = {"^~*|[]{}"};


//Converts the character from file to a column number for the FSA
int charConverter(char lookahead) {

  for(int i = 0; i < NUMSPECIALCHARS; i++) {
    if(charCheck[i] == lookahead)
      return 6;
  }
  if(isalpha(lookahead))
    return 0;
  else if(isdigit(lookahead)) {
    if(lookahead == 0) 
      return 1;
    return 2;
  }
  else if(lookahead == '.') {
    return 5;
  }
  else if(lookahead == '_') {
    return 3;
  }
  else if(lookahead == '+' || lookahead == '-') {
    return 4;
  }
  else if(isspace(lookahead)) {
    return 7;
  }
  else if(lookahead == EOF) {
    return 8;
  }
  else if(lookahead == '&') {
    return 420;
  }
  else {
    return -10;
  }
  return -1000;
}


token scanner(FILE *inFile, int &lineNum) {

  int currState = 0, nextState = 0, driverCol; 
  char lookahead;
  string data;
  token t;
  
  while (currState < 1000 && currState >= 0) {
  
    lookahead = fgetc(inFile);
    //if statement to catch comments if a & is found while continue looking until another & is found counting line numbers as it goes through
    if (lookahead == '&') {
      lookahead = fgetc(inFile);
      while(lookahead != '&') {
        lookahead = fgetc(inFile);
        if (lookahead == '\n') {
          lineNum++;
        }
      }
    }
    //final checks to insure lookahead isnt & or new line after handling comments
    if(lookahead == '&') {
      lookahead = fgetc(inFile);
    }      
    if (lookahead == '\n') {
        lookahead = fgetc(inFile);
        lineNum++;
    }
      
    
    driverCol = charConverter(lookahead);   
  
  //Error check for invalid characters
    if ( driverCol == -10) {
      cout << "Error! " << (char) lookahead << " on line " << lineNum << " is not a legal character!" << endl;
      exit(1);
    }
    
    //completely necessary second comment checker for what ever reason sometimes a & wont be caught
    if (driverCol == 420) {
        if (lookahead == '&') {
        lookahead = fgetc(inFile);
        while(lookahead != '&') {
          lookahead = fgetc(inFile);
          if (lookahead == '\n') {
            lineNum++;
          }
        }
      }
    //final checks to insure lookahead isnt & or new line after handling comments
      if(lookahead == '&') {
        lookahead = fgetc(inFile);
      }
      if(lookahead == '\n') {
        lookahead = fgetc(inFile);
        lineNum++;
      }
    }
    
        
    nextState = driverTable[currState][driverCol];

    //Correct final state handler
    if (nextState > 1000) {

      if(nextState == 1001) {
        t.id = "token1";
        t.data = data;
        t.line = lineNum;
        ungetc(lookahead, inFile);
        return t;
      }
      if(nextState == 1002) {
        t.id = "token2";
        t.data = data;
        t.line = lineNum;
        ungetc(lookahead, inFile);
        return t;
      }
      if(nextState == 1003) {
        t.id = "token3";
        t.data = data;
        t.line = lineNum;
        ungetc(lookahead, inFile);
        return t;
      }
      if(nextState == 1004) {
        t.id = "EOFTkn";
        t.data = "EOF";
        t.line = lineNum;
        ungetc(lookahead, inFile);
        return t;
      }      
    }
    //Error states
    else if(nextState < 0) {
      if (nextState == -1) {
       cout << "Error detected on line " << lineNum << " illegal token creation" << endl;
       exit(1);
      }
    }
    //Builds up token "word" and then continues through FSA
    else {
    //added whitespace checker thank you Tyler!
      if(!isspace(lookahead)) {  
        data = data + lookahead;
        currState = nextState;
      }
    }
  }

  return t;


}