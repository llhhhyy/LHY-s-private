#pragma once

#include "base.h"
#include "linear_model.h"

class DecisionTreeModel : public Model {
  class TreeNode {
   public:
      TreeNode(){}
      virtual ~TreeNode() = default;
      virtual std::optional<ValueType> predict(KeyType key) const = 0;
  };

  class LeafNode : public TreeNode {
  public:
      LeafNode(){}

      LinearModel model;
      std::vector<DataPoint> leaf_data;
      virtual std::optional<ValueType> predict(KeyType key) const override;
  };

  class InternalNode : public TreeNode {
  public:
      InternalNode(int key=0,TreeNode* left=NULL,TreeNode *right=NULL);

      int split_key;
      TreeNode *left_child;
      TreeNode *right_child;

      virtual std::optional<ValueType> predict(KeyType key) const override;
  };

 private:
    double compute_split_loss(const std::vector<DataPoint>& data,
                            size_t split_idx);  // In README Others.
    double compute_variance(const std::vector<DataPoint>& data, size_t start,
                          size_t end);  // In README Others.

    std::unique_ptr<TreeNode> build_tree(const std::vector<DataPoint>& data);

 public:
    std::unique_ptr<TreeNode> root;
    DecisionTreeModel():root(nullptr) {};

    void train(const std::vector<DataPoint>& data) override;

    std::optional<ValueType> predict(KeyType key) const override;
};