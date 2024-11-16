#include "rbTree.h"
#include "game.h"
#include <stdio.h>
#include <stdlib.h>

// Create a red-black tree
rbNode *rbtCreateNode(State *state)
{
  rbNode *node = malloc(sizeof(rbNode));
  node->color = RED;
  node->state = state;
  node->link[0] = NULL;
  node->link[1] = NULL;
  return node;
}

// Insert an node
rbNode *rbtInsert(rbNode *root, State *state)
{
  rbNode *stack[100], *ptr, *newnode, *sauv;
  int dir[100], ht = 0, index = 0, end = 0;
  ptr = root;
  if (!root)
  {
    root = rbtCreateNode(state);
    return root;
  }

  stack[ht] = root;
  dir[ht++] = 0;
  while (ptr != NULL)
  {
    if (ptr->state->hash == state->hash)
    {
      printf("Duplicates Not Allowed!!\n");
      return root;
    }
    index = (state->hash - ptr->state->hash) > 0 ? 1 : 0;
    stack[ht] = ptr;
    ptr = ptr->link[index];
    dir[ht++] = index;
  }
  stack[ht - 1]->link[index] = newnode = rbtCreateNode(state);
  stack[ht] = newnode; // ajout du nouveau noeud dans stack (plus pratique)
  while ((ht >= 3) && (stack[ht - 1]->color == RED) && (end == 0))
  {
    // CAS 1 : (ne met pas fin a la boucle)
    if (stack[ht - 2]->link[dir[ht - 2] == 0 ? 1 : 0] != NULL && stack[ht - 2]->link[dir[ht - 2] == 0 ? 1 : 0]->color == RED)
    { // cas où l'oncle est rouge (il faut faire attention au cas où l'oncle est nul)
      stack[ht - 2]->link[dir[ht - 2] == 0 ? 1 : 0]->color = BLACK;
      stack[ht - 1]->color = BLACK;
      stack[ht - 2]->color = RED;
      ht = ht - 2; // on continue la boucle while avec le grand pere
    }

    // CAS 2 : (met fin a la boucle)
    else if (dir[ht - 2] == 1)
    { // sous-arbre droit du grand pere -> rotation à gauche sur le grand pere
      if (dir[ht - 1] == 0)
      { // formation en triangle -> rotation à droite sur le pere
        // rotation à droite pere
        stack[ht - 2]->link[dir[ht - 2]] = stack[ht];
        stack[ht - 1]->link[0] = stack[ht]->link[1];
        stack[ht]->link[1] = stack[ht - 1];
        // modif de stack et dir en conséquence de la rotation (pour etre raccord avec la suite du code)
        sauv = stack[ht];
        stack[ht] = stack[ht - 1];
        stack[ht - 1] = sauv;
        dir[ht - 1] = 1;
      }
      // modif des couleurs
      stack[ht - 1]->color = BLACK;
      stack[ht - 2]->color = RED;
      // rotation à gauche grand pere
      if (ht > 3)
      { // le noeud possede un arriere grand pere
        stack[ht - 3]->link[dir[ht - 3]] = stack[ht - 1];
      }
      else
      { // changement de root dans la rotation
        root = stack[ht - 1];
      }
      stack[ht - 2]->link[1] = stack[ht - 1]->link[0];
      stack[ht - 1]->link[0] = stack[ht - 2];
      end = 1;
    }

    // CAS 3 : (met fin a la boucle)
    else if (dir[ht - 2] == 0)
    { // sous-arbre gauche du grand pere -> rotation à droite sur le grand pere
      if (dir[ht - 1] == 1)
      { // formation en triangle -> rotation à gauche sur le pere
        // rotation à gauche pere
        stack[ht - 2]->link[dir[ht - 2]] = stack[ht];
        stack[ht - 1]->link[1] = stack[ht]->link[0];
        stack[ht]->link[0] = stack[ht - 1];
        // modif de stack et dir en conséquence de la rotation (pour etre raccord avec la suite du code)
        sauv = stack[ht];
        stack[ht] = stack[ht - 1];
        stack[ht - 1] = sauv;
        dir[ht - 1] = 0;
      }
      // modif des couleurs
      stack[ht - 1]->color = BLACK;
      stack[ht - 2]->color = RED;
      // rotation à droite grand pere
      if (ht > 3)
      { // le noeud possede un arriere grand pere
        stack[ht - 3]->link[dir[ht - 3]] = stack[ht - 1];
      }
      else
      { // changement de root dans la rotation
        root = stack[ht - 1];
      }
      stack[ht - 2]->link[0] = stack[ht - 1]->link[1];
      stack[ht - 1]->link[1] = stack[ht - 2];
      end = 1;
    }
  }
  root->color = BLACK;
  return root;
}

// Rotation à gauche
rbNode *rbtRotateLeft(rbNode *root, rbNode *pere, int dir)
{
  rbNode *newRoot = root->link[1];
  root->link[1] = newRoot->link[0];
  newRoot->link[0] = root;
  if (pere != NULL)
  {
    pere->link[dir] = newRoot;
  }
  return newRoot;
}

// Rotation à droite
rbNode *rbtRotateRight(rbNode *root, rbNode *pere, int dir)
{
  rbNode *newRoot = root->link[0];
  root->link[0] = newRoot->link[1];
  newRoot->link[1] = root;
  if (pere != NULL)
  {
    pere->link[dir] = newRoot;
  }
  return newRoot;
}

rbNode *rbtResolutionDoubleNoir(rbNode *stack[100], int dir[100], int ht)
{
  if (ht != 0)
  {
    if ( // Cas Frere NOIR et il existe un neveu rouge
        (stack[ht - 1]->link[1 - dir[ht - 1]]->color == BLACK) &&
        ((
             (stack[ht - 1]->link[1 - dir[ht - 1]]->link[0] != NULL) &&
             (stack[ht - 1]->link[1 - dir[ht - 1]]->link[0]->color == RED)) ||
         ((stack[ht - 1]->link[1 - dir[ht - 1]]->link[1] != NULL) &&
          (stack[ht - 1]->link[1 - dir[ht - 1]]->link[1]->color == RED))))
    {

      if ((stack[ht - 1]->link[1 - dir[ht - 1]]->link[0] != NULL) && // Le neveu gauche est rouge
          (stack[ht - 1]->link[1 - dir[ht - 1]]->link[0]->color == RED) &&
          (dir[ht - 1] == 1))
      {
        stack[ht - 1]->link[1 - dir[ht - 1]]->color = stack[ht - 1]->color;
        stack[ht - 1]->color = BLACK;
        stack[ht - 1]->link[1 - dir[ht - 1]]->link[0]->color = BLACK;
        if (ht - 1 == 0)
        {
          stack[0] = rbtRotateRight(stack[ht - 1], NULL, -1);
        }
        else
        {
          rbtRotateRight(stack[ht - 1], stack[ht - 2], dir[ht - 2]);
        }
      }
      else if ((stack[ht - 1]->link[1 - dir[ht - 1]]->link[1] != NULL) && // Le neveu droit est rouge
               (stack[ht - 1]->link[1 - dir[ht - 1]]->link[1]->color == RED) &&
               (dir[ht - 1] == 0))
      {
        stack[ht - 1]->link[1 - dir[ht - 1]]->color = stack[ht - 1]->color;
        stack[ht - 1]->color = BLACK;
        stack[ht - 1]->link[1 - dir[ht - 1]]->link[1]->color = BLACK;
        if (ht - 1 == 0)
        {
          stack[0] = rbtRotateLeft(stack[ht - 1], NULL, -1);
        }
        else
        {
          rbtRotateLeft(stack[ht - 1], stack[ht - 2], dir[ht - 2]);
        }
      }
      else if ((stack[ht - 1]->link[1 - dir[ht - 1]]->link[0] != NULL) && // Le neveu gauche est rouge double rotation
               (stack[ht - 1]->link[1 - dir[ht - 1]]->link[0]->color == RED) &&
               (dir[ht - 1] == 0))
      {

        rbtRotateRight(stack[ht - 1]->link[1 - dir[ht - 1]], stack[ht - 1], 1 - dir[ht - 1]);
        stack[ht - 1]->link[1 - dir[ht - 1]]->color = stack[ht - 1]->color;
        stack[ht - 1]->color = BLACK;
        if (ht - 1 == 0)
        {
          stack[0] = rbtRotateLeft(stack[ht - 1], NULL, -1);
        }
        else
        {
          rbtRotateLeft(stack[ht - 1], stack[ht - 2], dir[ht - 2]);
        }
      }
      else if ((stack[ht - 1]->link[1 - dir[ht - 1]]->link[1] != NULL) && // Le neveu droit est rouge double rotation
               (stack[ht - 1]->link[1 - dir[ht - 1]]->link[1]->color == RED) &&
               (dir[ht - 1] == 1))
      { // pareil

        rbtRotateLeft(stack[ht - 1]->link[1 - dir[ht - 1]], stack[ht - 1], 1 - dir[ht - 1]);
        stack[ht - 1]->link[1 - dir[ht - 1]]->color = stack[ht - 1]->color;
        stack[ht - 1]->color = BLACK;
        if (ht - 1 == 0)
        {
          stack[0] = rbtRotateRight(stack[ht - 1], NULL, -1);
        }
        else
        {
          rbtRotateRight(stack[ht - 1], stack[ht - 2], dir[ht - 2]);
        }
      }
    }
    else if ((stack[ht - 1]->link[1 - dir[ht - 1]]->color == BLACK))
    { // Cas Frere NOIR pas de neveu rouge
      stack[ht - 1]->link[1 - dir[ht - 1]]->color = RED;
      if (stack[ht - 1]->color == RED)
      {
        stack[ht - 1]->color = BLACK;
      }
      else
      {
        stack[0] = rbtResolutionDoubleNoir(stack, dir, ht - 1);
      }
    }
    else
    { // Cas Frere rouge
      rbNode *sauv = NULL;
      stack[ht - 1]->link[1 - dir[ht - 1]]->color = BLACK;
      stack[ht - 1]->color = RED;
      sauv = stack[ht - 1]->link[1 - dir[ht - 1]];
      if (dir[ht - 1] == 1)
      {
        rbtRotateRight(stack[ht - 1], ht == 1 ? NULL : stack[ht - 2], dir[ht - 2]);
      }
      else
      {
        rbtRotateLeft(stack[ht - 1], ht == 1 ? NULL : stack[ht - 2], dir[ht - 2]);
      }
      stack[ht] = stack[ht - 1];
      stack[ht - 1] = sauv;
      dir[ht] = dir[ht - 1];
      rbtResolutionDoubleNoir(stack, dir, ht + 1);
    }
  }

  return stack[0];
}

rbNode *rbtMax(rbNode *root)
{
  while (root->link[1] != NULL)
  {
    root = root->link[1];
  }
  return root;
}

// Fonction pour supprimer un nœud dans un arbre rouge-noir
rbNode *rbtDelete(rbNode *root, State *state)
{
  if (root == NULL)
  {
    printf("Arbre non disponible\n");
    return root;
  }

  rbNode *stack[100], *ptr = NULL;
  int dir[100], ht = 0, diff;

  ptr = root;
  // Recherche du nœud à supprimer
  while (ptr != NULL && ptr->state->hash != state->hash)
  {
    diff = (state->hash - ptr->state->hash) > 0 ? 1 : 0;
    stack[ht] = ptr;
    dir[ht++] = diff;
    ptr = ptr->link[diff];
  }

  while (ptr != NULL)
  {
    if (ptr->link[0] != NULL && ptr->link[1] != NULL)
    { // Cas 2 enfants
      rbNode *max_gauche = rbtMax(ptr->link[0]);
      ptr->state->hash = max_gauche->state->hash;
      stack[ht] = ptr;
      dir[ht++] = 0;
      ptr = ptr->link[0];
      while (ptr->state->hash != max_gauche->state->hash)
      {
        stack[ht] = ptr;
        dir[ht++] = 1;
        ptr = ptr->link[1];
      }
    }
    else if (ptr->link[0] != NULL)
    { // Cas 1 enfant gauche
      ptr->state->hash = ptr->link[0]->state->hash;
      free(ptr->link[0]);
      ptr->link[0] = NULL;
      ptr = NULL;
    }
    else if (ptr->link[1] != NULL)
    { // Cas 1 enfant droit
      ptr->state->hash = ptr->link[1]->state->hash;
      free(ptr->link[1]);
      ptr->link[1] = NULL;
      ptr = NULL;
    }
    else if (ptr->color == RED)
    { // Cas feuille rouge
      if (ht == 0)
      {
        root = NULL;
      }
      else
      {
        stack[ht - 1]->link[dir[ht - 1]] = NULL;
      }
      free(ptr);
      ptr = NULL;
    }
    else
    { // Cas feuille noir
      free(ptr);
      ptr = NULL;
      if (ht == 0)
      {
        root = NULL;
      }
      else
      {
        stack[ht - 1]->link[dir[ht - 1]] = NULL;
        root = rbtResolutionDoubleNoir(stack, dir, ht);
      }
    }
  }

  return root;
}

State *rbtSearch(rbNode *root, int hash)
{
  if (root == NULL)
  {
    return NULL;
  }
  else if (hash > root->state->hash)
  {
    return rbtSearch(root->link[1], hash);
  }
  else if (hash < root->state->hash)
  {
    return rbtSearch(root->link[1], hash);
  }
  else
  {
    return root->state;
  }
}

// Print the inorder traversal of the tree
void rbtDisplay(rbNode *node)
{
  int color;
  if (node != NULL)
  {
    printf("%d(%d) ", node->state->hash, color = node->color == BLACK ? 1 : 0);
    printf("[");
    if (node->link[0] != NULL)
    {
      rbtDisplay(node->link[0]);
    }
    printf("] ");
    printf("[");
    if (node->link[1] != NULL)
    {
      rbtDisplay(node->link[1]);
    }
    printf("]");
  }
  else
  {
    printf("Cet Arbre est vide.\n");
  }
}
