#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include "node.h"
#include "token.h"
#include "printASM.h"

using namespace std;


ofstream outFile;
string vars[100] = "";
string tempVar1 = "", tempVar2 = "", tempVar3 = "", tempVar4 = "";
int numVars = 0, varFlag = 0, readFlag = 0, writeFlag = 0, addFlag = 0, subFlag = 0, storeFlag = 0, brzNegFlag = 0;

//function to init outFile and print out STOP and init vars
void startASM(node_t* root, int level) {

    outFile.open("genCode.asm");
    printASM(root, level);
    outFile << "STOP" << endl;
    for(int i = 0; i < 100; i++) {
      if(vars[i] != ""){
        outFile << vars[i] << " 0" << endl;
      }
    }
    outFile.close();
    return;
}

void printASM(node_t* root, int level) {
  

  
  //checks for when tree is empty which signifies the tree is finished printing
  if (root == NULL) {
    return;
  } 
  //prints spaces to delineate levels for parse tree viewing 
  else {
    token t = root->tk;
    
    //stores temp vars for manipulation
    if (tempVar1 == "" && t.id != "token2") {
      tempVar1 = t.data;
      //removes +symbol from positive token3s
      tempVar1.erase(remove(tempVar1.begin(), tempVar1.end(), '+'), tempVar1.end());
    }
    else if(tempVar2 == "" && t.data == "|"){
      tempVar2 = t.data;
    }
    else if(tempVar3 == "" && t.id != "token2"){
      tempVar3 = t.data;
      //removes +symbol from positive token3s
      tempVar3.erase(remove(tempVar3.begin(), tempVar3.end(), '+'), tempVar3.end());
    }
    else if(tempVar4 == "" && t.id != "token2"){
      tempVar4 = t.data;
      //removes +symbol from positive token3s
      tempVar4.erase(remove(tempVar4.begin(), tempVar4.end(), '+'), tempVar4.end());
    }
    else{
    }

    //add vars
    if(t.data == "{") {
      addFlag = 1;
    }
    if(addFlag == 1) {
      addFlag = 0;
      outFile << "LOAD " << tempVar3 << endl;
      outFile << "ADD " << tempVar4 << endl;
      tempVar3 = "";
      tempVar4 = "";
      if (tempVar2 == "|") {
        storeFlag = 1;
      }
    }
    //sub vars
    if(t.data == "}") {
      subFlag = 1;
    }
    if(subFlag == 1) {
      subFlag = 0;
      outFile << "LOAD " << tempVar3 << endl;
      outFile << "SUB " << tempVar4 << endl;
      tempVar3 = "";
      tempVar4 = "";
      if (tempVar2 == "|") {
        storeFlag = 1;
      }
    }
    
        
    //store var
    if(storeFlag == 1) {
      storeFlag = 0;
      outFile << "STORE " << tempVar1 << endl;
      tempVar1 = "";
      tempVar2 = "";
      
    }
    
    
    //stores init variables to print at end of asm 
    if(t.data == "^") {
      varFlag = 1;
    }
    if(varFlag == 1 && t.id != "token2") {
      varFlag = 0;
      vars[numVars] = t.data;
      numVars++;
      outFile << "LOAD 0" << endl;
      outFile << "STORE " << t.data << endl;
      tempVar1 = "";
      tempVar2 = "";
      tempVar3 = "";
      tempVar4 = "";
    }
    //read vars
    if(t.data == "[" && brzNegFlag == 0) {
      readFlag = 1;
    }
    if(readFlag == 1 && t.id != "token2") {
      readFlag = 0;
      outFile << "READ " << t.data << endl;
      tempVar1 = "";
      tempVar2 = "";
      tempVar3 = "";
      tempVar4 = "";
    }
    
    //write vars
    if(t.data == "]" && brzNegFlag == 0) {
      writeFlag = 1;
    }
    if(writeFlag == 1 && (t.id == "token1" || t.id == "token3" )) {
      writeFlag = 0;
      outFile << "WRITE " << t.data << endl;
      tempVar1 = "";
      tempVar2 = "";
      tempVar3 = "";
      tempVar4 = "";
    }
    
    //brzneg stuff
    if(t.data == "*") {
      brzNegFlag = 1;
    }
    if(brzNegFlag == 1 && tempVar4 != "") {
      brzNegFlag = 0;
      outFile << "LOAD " << tempVar1 << endl;
      outFile << "BRZNEG negative" << endl;
      outFile << "WRITE " << tempVar3 << endl;
      outFile << "BR end" << endl;
      outFile << "negative: WRITE " << tempVar4 << endl;
      outFile << "end: NOOP" << endl;
    }
    


    //recursive printTree calls to pick up all the children
    printASM(root->child1, level);
    printASM(root->child2, level);
    printASM(root->child3, level);
    printASM(root->child4, level);
    printASM(root->child5, level);
  }
}