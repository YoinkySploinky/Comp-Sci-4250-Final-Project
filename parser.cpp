#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <string>
#include <cctype>
#include "token.h"
#include "scanner.h"
#include "parser.h"
#include "node.h"

using namespace std;

token t;

/*
BNF grammar for parser
S -> <A> <J>
A -> ^t1 <A> || Empty
C -> * <K>
D -> [ t1
E -> ] <K> ~
F -> | <G> ~
G -> <K> <K> <H> | <K>
H -> { || }
J -> <C> ~ <J> ~ <L> || <D> ~ <J> || <E> ~ <J> || t1 <F> ~ <J> || Empty
K -> t1 || t3
L -> <D> ~ || <E> || <F> ~ || Empty  

Each function will create a node for the nonTerminal i.e. <S>
then in the function it will assign the children of its node to the nonTerminals and terminals i.e. the <A> and <J> in <S>
If the node is discovered to be token1 or token3 it will then also assign the token to the child node 
*/

//creates new node and applies label
node_t *getNode(string label) {

  node_t *node = new node_t;
  node->label = label;
  return node;

}

node_t* parser(FILE *inFile, int &lineNumber) {
  
  //create empty root node for parse tree all functions will return back to this root node creating a parse tree
  node_t *root;
  t = scanner(inFile, lineNumber);
  
  root = S(inFile, lineNumber);
  
  if (t.id == "EOFTkn") {
    cout << "Parse was good" << endl;
  }
  else {
  
    error();
  
  }
  
  return root;
  
}

node_t* S(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("S");
  n->child1 = A(inFile, lineNumber);
  n->child2 = J(inFile, lineNumber);
  return n;

}

node_t* A(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("A");
  if (t.data == "^") {
    n->child1 = getNode("^");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    if(t.id == "token1") {
      n->child2 = getNode("token1");
      n->child2->tk = t;
      t = scanner(inFile, lineNumber);
      n->child3 = A(inFile, lineNumber);
      return n;
    }
    else {
      error();
    }
  }
  else {
    n->child1 = getNode("Empty");
    return n;
  }
  
  return n;
}

node_t* C(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("C");
  if (t.data == "*") {
    n->child1 = getNode("*");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    n->child2 = K(inFile, lineNumber);
    return n;
  }
  else {
    error();
  }
  
  return n;
  
}

node_t* D(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("D");
  if (t.data == "[") {
    n->child1 = getNode("[");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    if (t.id == "token1") {
      n->child2 = getNode("token1");
      n->child2->tk = t;
      t = scanner(inFile, lineNumber);
      return n;
    }
      else {
        error();
      }
  }
  else {
    error();
  }
  
  return n;
  
}

node_t* E(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("E");
  if (t.data == "]") {
    n->child1 = getNode("]");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    n->child2 = K(inFile, lineNumber);
    if (t.data == "~") {
      n->child3 = getNode("~");
      n->child3->tk = t;
      t = scanner(inFile, lineNumber);
      return n;
    }
  }
  else {
    error();
  }
  
  return n;
  
}

node_t* F(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("F");
  if (t.data == "|") {
    n->child1 = getNode("|");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    n->child2 = G(inFile, lineNumber);
    if (t.data == "~") {
      n->child3 = getNode("~");
      n->child3->tk = t;
      t = scanner(inFile, lineNumber);
      return n;
    }
  }
  else {
    error();
  }
  
  return n;
  
}

node_t* G(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("G");
  n->child1 = K(inFile, lineNumber);
  if (t.id == "token1" || t.id == "token3") {
    n->child2 = K(inFile, lineNumber);
    n->child3 = H(inFile, lineNumber);
  }
  else {
    return n; 
  }
  
  return n;

}

node_t* H(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("H");
  if(t.data == "{") {
    n->child1 = getNode("{");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    return n;
  }
  else if(t.data == "}") {
    n->child1 = getNode("}");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    return n;
  }
  else {
    error();
  }
  
  return n;
}

node_t* J(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("J");
  if (t.data == "*") {
     n->child1 = C(inFile, lineNumber);
     n->child1->tk = t;
    if (t.data == "~") {
      n->child2 = getNode("~");
      n->child2->tk = t;
      t = scanner(inFile, lineNumber);
      n->child3 = J(inFile, lineNumber);
      if (t.data == "~") {
        n->child4 = getNode("~");
        n->child4->tk = t;
        t = scanner(inFile, lineNumber);
        n->child5 = L(inFile, lineNumber);
        return n;
      }
      else {
        error();
      }
    }
    else {
      error();
    }
  }
  else if (t.data == "[") {
    n->child1 = D(inFile, lineNumber);
    if (t.data == "~") {
      n->child2 = getNode("~");
      n->child2->tk = t;
      t = scanner(inFile, lineNumber);
      n->child3 = J(inFile, lineNumber);
      return n;
    }
    else {
      error();
    }
  }
  else if (t.data == "]") {
    n->child1 = E(inFile, lineNumber);
    if (t.data == "~") {
      n->child2 = getNode("~");
      n->child2->tk = t;
      t = scanner(inFile, lineNumber);
      n->child3 = J(inFile, lineNumber);
      return n;
    }
    else {
      error();
    }
  }
  else if (t.id == "token1") {
    n->child1 = getNode("token1");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    n->child2 = F(inFile, lineNumber);
    if (t.data == "~") {
      n->child3 = getNode("~");
      n->child3->tk = t;
      t = scanner(inFile, lineNumber);
      n->child4 = J(inFile, lineNumber);
      return n;
    }
    else {
      error();
    }
  }
  else {
    n->child1 = getNode("Empty");
    return n;
  }
  
  return n;
  
}

node_t* K(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("K");
  if(t.id == "token1") {
    n->child1 = getNode("token1");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    return n;
  }
  else if(t.id == "token3") {
    n->child1 = getNode("token3");
    n->child1->tk = t;
    t = scanner(inFile, lineNumber);
    return n;
  }
  else {
    error();
  }
  
  return n;
  
}

node_t* L(FILE *inFile, int &lineNumber) {

  node_t *n = getNode("L");
  if(t.data == "[") {
    n->child1 = D(inFile, lineNumber);
    if(t.data == "~") {
      n->child2 = getNode("~");
      n->child2->tk = t;
      t = scanner(inFile, lineNumber);
      return n;
    }
    else {
      error();
    }
  }
  else if(t.data == "]") {
    n->child1 = E(inFile, lineNumber);
    return n;
  }
  else if(t.data == "|") {
    n->child1 = F(inFile, lineNumber);
    if(t.data == "~") {
      n->child2 = getNode("~");
      n->child2->tk = t;
      t = scanner(inFile, lineNumber);
      return n;
    }
    else {
      error();
    }
  }
  else {
    n->child1 = getNode("Empty");
    return n;
  }
  
  return n;
  
}

void error() {
  cout << "ERROR! Could not parse on line " << t.line << " invalid token " << t.data << " was found" << endl;
  exit(1);

}