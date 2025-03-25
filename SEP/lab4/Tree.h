#pragma once

#include <iostream>
#include <vector>

#include "Calculator.h"
#include "Comparator.h"
#include "TreeNode.h"

using namespace std;





class BinaryDimenTree {
  /* DO NOT CHANGE SIGNATURE */
  friend istream &operator>>(istream &in,
                             BinaryDimenTree &tree); 

private:
  /* data */
  DistanceCalculator *calculator;
  TreeNode *root;
  vector<vector<double>> data;
public:
  /* methods */
  int cal;
  /* DO NOT CHANGE SIGNATURE */
  BinaryDimenTree(DistanceCalculator *calculator);

  /* DO NOT CHANGE SIGNATURE */
  TreeNode *find_nearest_node(TreeNode *target);

  void find(TreeNode* target,TreeNode* &nearnode,TreeNode* root,double &mind);
  /* DO NOT CHANGE SIGNATURE */

  void creattree()
  {
      root=creattreenode(0,data.size()-1,0);
  }

  TreeNode* creattreenode(int left,int right,int dime);

  ~BinaryDimenTree(); /* DO NOT CHANGE */
};
