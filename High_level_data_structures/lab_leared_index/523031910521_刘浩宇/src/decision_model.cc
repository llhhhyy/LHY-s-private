#include "decision_model.h"

#include "linear_model.h"

void DecisionTreeModel::train(const std::vector<DataPoint>& data) {
    root = build_tree(data);
}

std::optional<ValueType> DecisionTreeModel::predict(KeyType key) const {
  return root->predict(key);
}

DecisionTreeModel::InternalNode::InternalNode(int key, DecisionTreeModel::TreeNode *left,
                                              DecisionTreeModel::TreeNode *right)
                                              :split_key(key),left_child(left),right_child(right){}


//TODO
std::unique_ptr<DecisionTreeModel::TreeNode> DecisionTreeModel::build_tree(const std::vector<DataPoint> &data) {
    if(data.size()<=default_max_leaf_samples){
        auto leaf = std::make_unique<DecisionTreeModel::LeafNode>();
        leaf->leaf_data = data;

        leaf->model.train(data);
        return leaf;
    }
    size_t split_idx = data.size()/2;

    std::vector<DataPoint> left_data(data.begin(), data.begin() + split_idx);
    std::vector<DataPoint> right_data(data.begin() + split_idx, data.end());

    auto left_child = build_tree(left_data);
    auto right_child = build_tree(right_data);
    return std::make_unique<InternalNode>(data[split_idx].key,left_child.release(),right_child.release());
}

//TODO
std::optional<ValueType> DecisionTreeModel::InternalNode::predict(KeyType key) const {
    if (key < split_key) {
        return left_child->predict(key); // 递归查询左子树
    } else {
        return right_child->predict(key); // 递归查询右子树
    }
}
//TODO
std::optional<ValueType> DecisionTreeModel::LeafNode::predict(KeyType key) const {
    if(leaf_data.empty()) return std::nullopt;
    int32_t predicted_position = model.get_predicted_pos(key);
    if(leaf_data[predicted_position].key == key)return leaf_data[predicted_position].value;

   for(size_t i = predicted_position - epsilon; i <= predicted_position + epsilon && i >=0 && i < leaf_data.size(); i++){
       if(leaf_data[i].key == key)
           return leaf_data[i].value;
   }

    // 如果未找到，遍历整个叶子节点的数据
    for (const auto& data_point : leaf_data) {
        if (data_point.key == key) {
            return data_point.value; // 找到目标 key
        }
    }
    return std::nullopt;
}
