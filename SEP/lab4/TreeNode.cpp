#include <stdexcept>

#include "TreeNode.h"

TreeNode::TreeNode(vector<double> d,int dime):coordinates(d),left(NULL),right(NULL),dim(dime)
{}

TreeNode::TreeNode(initializer_list<double> coords) :coordinates(coords) ,left(NULL),right(NULL),dim(0){// TODO
}

const double &TreeNode::operator[](int index) const {
  // TODO
  return coordinates[index];
}

int TreeNode::dimension() const {
  // TODO
  return 2;
}

const vector<double> &TreeNode::getCoordinates() const {
  // TODO
  return coordinates;
}

TreeNode::~TreeNode() {
} // Even though the implementation might be empty, it's
                         // a good practice to define it.