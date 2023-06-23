#include <iostream>
#include <string>
#include "node.h"
#include "token.h"
#include "printTree.h"

using namespace std;


string symbolTable[100] = "";
int checkFlag = 0;
bool verifyFlag;


void error(string symbol) {

  cout << "Error! " << symbol << " symbol already in symbol table!" << endl;
  exit(1);

}

void insertString(string symbol) {

  for(int i = 0; i < 100; i++) {
    if (symbolTable[i] == "") {
      symbolTable[i] = symbol;
      return;
    }
    if (symbolTable[i] == symbol) {
      error(symbol);
    }
  }
  
}

bool verify(string symbol) {


  for(int i = 0; i < 100; i++) {
    if (symbol == symbolTable[i]) {
      return true;
    }
  }
  
  return false;

}


void printTree(node_t* root, int level) {

  //checks for when tree is empty which signifies the tree is finished printing
  if (root == NULL) {
    return;
  } 
  //prints spaces to delineate levels for parse tree viewing 
  else {
    token t = root->tk;
    if(t.data == "^") {
      checkFlag = 1;
    }
    if(checkFlag == 1 && t.id != "token2") {
      insertString(t.data);
      checkFlag = 0;
      cout << t.data << " " ;
    }
    if(checkFlag == 0 && t.id == "token1") {
      verifyFlag = verify(t.data);
      if(verifyFlag == false) {
        cout << "Error! " << t.data << " was not initialized yet!" << endl;
        exit(1);
      }
    }

    //recursive printTree calls to pick up all the children
    printTree(root->child1, level);
    printTree(root->child2, level);
    printTree(root->child3, level);
    printTree(root->child4, level);
    printTree(root->child5, level);
  }

}