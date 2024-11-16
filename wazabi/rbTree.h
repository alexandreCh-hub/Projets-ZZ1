#ifndef RBT_H
#define RBT_H
#include "game.h"

enum nodeColor
{
  RED,
  BLACK
};

typedef struct rbNode
{
  int color;
  struct State *state;
  struct rbNode *link[2];
} rbNode;

struct rbNode *rbtCreateNode(State *state);
struct rbNode *rbtInsert(struct rbNode *root, State *state);
struct rbNode *rbtDelete(struct rbNode *root, State *state);
void rbtDisplay(struct rbNode *node);
State *rbtSearch(rbNode *root, int hash);

#endif
