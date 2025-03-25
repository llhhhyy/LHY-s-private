#pragma once

#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <stdexcept>
#include <algorithm>

class DataLoader {
 private:
  std::vector<DataPoint> data;

 public:
    void load_data(const std::string& data_path) {
        // 打开文件
        std::ifstream file(data_path);
        if (!file.is_open()) {
            throw std::runtime_error("Unable to open file: " + data_path);
        }

        // 清空现有数据
        data.clear();

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty()) {
                continue;
            }

            std::stringstream ss(line);
            std::string value_str, key_str;

            if (!std::getline(ss, key_str, ',') || !std::getline(ss, value_str)) {
                file.close();
                throw std::runtime_error("Invalid data format in file: " + data_path);
            }

            try {
                ValueType value = std::stoi(value_str);
                KeyType key = std::stoi(key_str);

                data.push_back({key, value});
            } catch (const std::exception& e) {
                file.close();
                throw std::runtime_error("Error parsing numbers in file: " +
                                         data_path + " line: " + line);
            }
        }

        file.close();
        std::sort(data.begin(), data.end(),
                  [](const DataPoint& a, const DataPoint& b) {
                      return a.key < b.key;
                  });
    }

  std::vector<DataPoint>& get_data() { return data; }
};
