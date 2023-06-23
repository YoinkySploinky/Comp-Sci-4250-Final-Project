#ifndef NODE_H
#define NODE_H
#include <string>
#include "token.h"

using namespace std;

struct node_t{

  string label;
  token tk;
  node_t *child1, *child2, *child3, *child4, *child5;
};

#endif