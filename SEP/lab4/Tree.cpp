#include "Tree.h"
#include <algorithm>
#include <cassert>
#include <limits>
#include <queue>
#include<algorithm>

#include "Calculator.h"
#include "Comparator.h"
#include "TreeNode.h"

using namespace std;

/*
 * You can use this struct to compare a certain dimension of the tree nodes!
 * For example, if you want to compare the first dimension, you can simply call
 * DimComparator(0), such as DimComparator(0)(node_a, node_b).
 */



bool cmp1(vector<double> a,vector<double> b)
{
    return a[0]<b[0];
}

bool cmp2(vector<double> a,vector<double> b)
{
    return a[1]<b[1];
}

struct DimComparator {
  int dim;
  DimComparator(int d) : dim(d) {}
  bool operator()(TreeNode *a, TreeNode *b) const {
    assert(a->dimension() == b->dimension() && a->dimension() > dim);
    return isLessThan((*a)[dim], (*b)[dim]);
  }
};


BinaryDimenTree::BinaryDimenTree(DistanceCalculator *calculator):calculator(calculator) {
  // TODO
  root=NULL;
}

BinaryDimenTree::~BinaryDimenTree() {
  // TODO
    if(root!=NULL)
    {
        std::queue<TreeNode*> q;
        TreeNode* f=NULL;
        q.push(root);
        while (!q.empty())
        {
            f = q.front();
            if (f->left != NULL) q.push(f->left);
            if (f->right != NULL) q.push(f->right);
            delete f;
            q.pop();
        }
    }
}

TreeNode *BinaryDimenTree::find_nearest_node(TreeNode *target) {
  // TODO
  TreeNode* nearnode=NULL;
  double mind=numeric_limits<double>::max();
  find(target,nearnode,root,mind);
  return nearnode;
}

void BinaryDimenTree::find(TreeNode *target,TreeNode* &nearnode,TreeNode *root,double& mind)
{

    if(root==NULL)
        return;
    double d=0;
    double vd=0;

    d = calculator->calculateDistance(*target,*root);
    vd =calculator->verticalDistance(*root,*target,root->getdim());

    if(isLessThan(d,mind))
    {
        mind=d;
        nearnode=root;
    }else if(isEqual(d,mind)){
        if(isLessThan(root->getCoordinates()[0],nearnode->getCoordinates()[0]))
        {
            nearnode=root;
        }else if(isEqual(root->getCoordinates()[0],nearnode->getCoordinates()[0])){
            if(isLessThan(root->getCoordinates()[1],nearnode->getCoordinates()[1])){
                nearnode=root;
            }
        }
    }
    if(isLessThan(target->getCoordinates()[root->getdim()],root->getCoordinates()[root->getdim()]))
    {
        find(target,nearnode,root->left,mind);
        if(isLessThanOrEqual(vd,mind))
        {
            find(target,nearnode,root->right,mind);
        }
    }else{
        find(target,nearnode,root->right,mind);
        if(isLessThanOrEqual(vd,mind))
        {
            find(target,nearnode,root->left,mind);
        }
    }
}


TreeNode* BinaryDimenTree::creattreenode(int left,int right,int dime)
{
    if (right < left)
        return NULL;

    int mid = (left + right+1) / 2;
    if(dime==0) {
        nth_element(data.begin()+left, data.begin() + mid, data.begin()+right+1, cmp1);
    }
    if(dime==1){
        nth_element(data.begin()+left, data.begin() + mid, data.begin()+right+1, cmp2);
    }

    TreeNode * r = new TreeNode(data[mid],dime);
    dime=(dime+1)%2;
    r->left = creattreenode(left, mid-1,dime);
    r->right = creattreenode(mid+1, right,dime);

    return r;
}

istream &operator>>(istream &in, BinaryDimenTree &tree) {
  // TODO
  int n=0;
  in>>n;
  int i=0;
  vector<double> tmp(2);
  for(i;i<n;i++)
  {
      in>>tmp[0];
      in>>tmp[1];
      tree.data.push_back(tmp);
  }
  //sort(tree.data.begin(),tree.data.end(),cmp);

  tree.creattree();
  return in;
}
