#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include "testScanner.h"
#include "token.h"
#include "scanner.h"

using namespace std;

void testScanner(FILE *inFile) {

  token t;
  int lineNum = 1;
  
  while(!feof(inFile)) {
  
    t = scanner(inFile, lineNum);
    //having t.id display first causes some weird output errors
    cout << t.data << "    " << t.id << "    on line " << t.line << endl;
  
  }
}