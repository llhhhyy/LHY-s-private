#pragma once

#include "base.h"

class LinearModel : public Model {
private:
    double slope;
    double intercept;
    KeyType base_key;
    
public:
    std::vector<DataPoint> data;

    void train(const std::vector<DataPoint>& data) override;
    double get_slope();
    double get_intercept();
    int32_t get_predicted_pos(KeyType key) const;
    std::optional<ValueType> predict(KeyType key) const override;
};
