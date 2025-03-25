#include <cassert>
#include <chrono>
#include <iostream>
#include <map>

#include "learned_index.h"

using namespace std;

// 测量插入时间（单位：毫秒）
template<typename T>
double measure_insert_time(T& container, const vector<DataPoint>& data) {
    auto start = chrono::high_resolution_clock::now();
    for (const auto& item : data) {
        container[item.key] = item.value;
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}

// 测量查找时间（单位：毫秒）
template<typename T>
double measure_lookup_time(T& container, const vector<DataPoint>& data) {
    auto start = chrono::high_resolution_clock::now();
    for (const auto& item : data) {
        auto v = container[item.key];
        assert(v == item.value); // 验证正确性
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}
double measure_index_lookup_time(const LearnedIndex& index,const vector<DataPoint>& data){
    auto start = chrono::high_resolution_clock::now();
    for (const auto& item : data) {
        auto v = index[item.key];
        assert(v == item.value); // 验证正确性
    }
    auto end = chrono::high_resolution_clock::now();
    return chrono::duration<double, milli>(end - start).count();
}
void test_std_map(std::string data_path) {
  DataLoader data_loader;

  data_loader.load_data(data_path);
  std::vector<DataPoint> data = data_loader.get_data();

  std::map<KeyType, ValueType> tree;

  double insert_time = measure_insert_time(tree,data);
  double lookup_time = measure_lookup_time(tree,data);
    cout << "std::map:\n";
    cout << "  insert time: " << insert_time << " ms\n";
    cout << "  lookup time: " << lookup_time << " ms\n";
}

void test_linear_model(std::string data_path) {
    LearnedIndex index("Linear", data_path);

    double lookup_time = measure_index_lookup_time(index,index.data());

    auto start_linear = chrono::high_resolution_clock::now();
    LearnedIndex linear_temp("Linear", data_path); // 重建以测量插入时间
    auto end_linear = chrono::high_resolution_clock::now();
    double insert_time = chrono::duration<double, milli>(end_linear - start_linear).count();
    cout << "LinearModel:\n";
    cout << "  insert time: " << insert_time << " ms\n";
    cout << "  lookup time: " << lookup_time << " ms\n";
}

void test_decision_tree_model(std::string data_path) {
    LearnedIndex index("DecisionTree", data_path);

    double lookup_time = measure_index_lookup_time(index,index.data());

    auto start_des = chrono::high_resolution_clock::now();
    LearnedIndex linear_temp("DecisionTree", data_path); // 重建以测量插入时间
    auto end_des = chrono::high_resolution_clock::now();
    double insert_time = chrono::duration<double, milli>(end_des - start_des).count();
    cout << "DecisionTreeModel:\n";
    cout << "  insert time: " << insert_time << " ms\n";
    cout << "  lookup time: " << lookup_time << " ms\n";
}

int main(int argc, char **argv) {
  std::string test_name = "normal_500000";
  std::string data_path = "./data/" + test_name + ".csv";
  cout<<"in "<<test_name<<" test: \n";

  test_std_map(data_path);
  test_linear_model(data_path);
  test_decision_tree_model(data_path);

  test_name = "uniform_500000";
  data_path = "./data/" + test_name + ".csv";
  cout<<"in "<<test_name<<" test:\n";

  test_std_map(data_path);
  test_linear_model(data_path);
  test_decision_tree_model(data_path);
}
