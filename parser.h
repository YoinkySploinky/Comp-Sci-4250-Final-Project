#ifndef PARSER_H
#define PARSER_H
#include "node.h"

using namespace std;

node_t* getNode(string);
node_t* parser(FILE *, int &);
node_t* S(FILE *, int &);
node_t* A(FILE *, int &);
node_t* C(FILE *, int &);
node_t* D(FILE *, int &);
node_t* E(FILE *, int &);
node_t* F(FILE *, int &);
node_t* G(FILE *, int &);
node_t* H(FILE *, int &);
node_t* J(FILE *, int &);
node_t* K(FILE *, int &);
node_t* L(FILE *, int &);
void error();



#endif