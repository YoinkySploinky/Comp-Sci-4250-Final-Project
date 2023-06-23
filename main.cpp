#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include "testScanner.h"
#include "parser.h"
#include "node.h"
#include "printTree.h"
#include "printASM.h"

using namespace std;

int main(int argc, char *argv[]) {

  FILE *inFile;
  ofstream outFile;
  string data, checker;
  string keyboardInputUsed = "out";
  string fileName;
  int lineNumber = 1;
  int level = 0;

//Checker to insure that only 2 arguments are passed
  if (argc > 2) {
    cout << "Fatal Error! Too many arguments given!" << endl;
    cout << "Correct form: P0 [filename]" << endl;
  }

//If no params found in command line uses keyboard inputs for parse tree
  if (argc == 1) {
    outFile.open(keyboardInputUsed);
    fileName = keyboardInputUsed;
    cout << "Enter words to be added to binary parse tree (once finished enter plsexit):" << endl;
    while(true) {
      cin >> data;
      cin.ignore();
      
//For loop to check through input data to insure that string is both contains no special characters or numbers and all lowercase. 
      for (int i = 0; i < data.length(); i++) {
        checker = checker + data[i];
      }
      if(checker == "plsexit" || data == "plsexit") {
        break;  
      }
      outFile << data << endl;
    }
    //add blank line after keyboard input so that the scanner can detect EOF
    outFile << endl;
    outFile.close();
  }
//if a input file was given sets up file for stream
  if (argc == 2) {
    fileName = argv[1];
  }
  
  inFile = fopen(fileName.c_str(), "r");
  node_t *root = parser(inFile, lineNumber);
  cout << "Displaying Symbol Table: " << endl;
  printTree(root, level);
  startASM(root, level);
  cout << endl;
  fclose(inFile);
  
  return 0;
}
