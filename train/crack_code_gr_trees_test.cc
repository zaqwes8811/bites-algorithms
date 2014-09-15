 // "Not all binary trees are binary search trees"

#include <gtest/gtest.h>

#include <iostream>

//
#include <stack>
#include <queue>

namespace crack_trees_and_gr {

using namespace std;

//template <typename T>
struct node
{
  node() : left(0), right(0), data(0) {}
  explicit node(int _data) 
    : left(0), right(0)  //node() - c++11 delegation ctor
    , data(_data) {}
  node* left;
  node* right;
  //T 
  int
  data;
};

//template <typename T> 
node* create_tree() {
  node* root = new node(1);

  root->left = new node(2);
  node* current = root->left;
  current->left = new node(4);
  current->right = new node(5);

  current = root;
  current->right = new node(6);
  return root;
}

void destroy_tree() {
  // снизу вверх, хотя может и не нужно
}

void print_tree_in_order(node* root) 
{
  // l -> current -> r
  // FIXME: как снять данные

  // out - лишний заход кстати
  if (!root)
    return;

  print_tree_in_order(root->left);
  //cout << root->left->data << endl;
  print_tree_in_order(root->right);
  
}

/// Traversal
//   http://en.wikipedia.org/wiki/Corecursion
//   http://programmers.stackexchange.com/questions/144274/whats-the-difference-between-recursion-and-corecursion
//
TEST(Crack, CreateTree) {
  using namespace crack_trees_and_gr;
  node* root = create_tree();
}

// std::stack and std::queue
TEST(Std, StackAndQueue) {
  // некоторые функции двухфазные как бы - но вообще это правильное разделение обяз.

  // Stack
  // for top and pop size > 0 - else UB

  // Queue - кстати deque по умолчанию, а в многопоточных использвал список
  // if size == 0 - front(), back(), pop() - UB!!
}

}