#include "linear_model.h"

#include <unordered_map>
#include <cmath>

void LinearModel::train(const std::vector<DataPoint>& data) {
    if (data.empty()) return;
    this->data = data;

    base_key = data[0].key;
    size_t n = data.size();
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_x2 = 0;

    for (size_t i = 0; i < n; ++i) {
        double x = static_cast<double>(data[i].key) - base_key;
        double y = static_cast<double>(i);
        sum_x += x;
        sum_y += y;
        sum_xy += x * y;
        sum_x2 += x * x;
    }

    slope = (n * sum_xy - sum_x * sum_y) / (n * sum_x2 - sum_x * sum_x);
    intercept = (sum_y - slope * sum_x) / n;
}

std::unordered_map<KeyType, int32_t> grade_key_to_predict_position{};

std::optional<ValueType> LinearModel::predict(KeyType key) const {
    if (data.empty()) return std::nullopt;
    int32_t predicted_position = get_predicted_pos(key);
    if(data[predicted_position].key == key)return data[predicted_position].value;

    for(size_t i = predicted_position - epsilon; i <= predicted_position + epsilon && i >=0 && i < data.size(); i++){
        if(data[i].key == key)
            return data[i].value;
    }
    for (const auto& data_point : data) {
        if (data_point.key == key) {
            return data_point.value;
        }
    }
    return std::nullopt;
}

double LinearModel::get_slope() {
    return slope;
}
double LinearModel::get_intercept() {
    return intercept;
}

int32_t LinearModel::get_predicted_pos(KeyType key) const {
    int32_t pos = 0;
    double x = key-base_key;
    double pred = slope * x + intercept;
    pos = static_cast<int32_t>(std::round(pred));
    if(pos<=0){
        pos = 0;
    }
//    grade_key_to_predict_position[key] = pos;
    return pos;
}
