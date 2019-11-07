//
// Created by kczec on 30.10.2019.
//
#ifndef TSP_HPP
#define TSP_HPP

#include <vector>
#include <utility>
#include <algorithm>
#include <cmath>
#define  INF (NAN)
#include <numeric>
#include <sstream>
std::vector<int> tsp(std::vector<std::vector<double>>);
using possible_paths = std::vector<std::pair<int,int>>;
using rows_min  = std::vector<double>;
using col_min = std::vector<double>;
using path = std::pair<int,int>;

class CostMatrix {
public:
    CostMatrix(std::vector<std::vector<double>> m ) ;

    const std::vector<double>& operator[](std::size_t pos) const;

    std::vector<double>& operator[](std::size_t pos);

    const std::vector<int> col() const;

    const std::vector<int> row() const;

    void del_row(int);

    void del_col(int);

    auto len();

    auto rowbegin();

    auto rowend();

    auto colbegin();

    auto colend();
private:

    std::vector<std::vector<double>> Matrix_;
    std::vector<int> remain_col;
    std::vector<int> remain_rows;

};

rows_min find_min_in_rows(CostMatrix);

col_min find_min_in_col(CostMatrix);

CostMatrix reduce_value_in_rows(CostMatrix,rows_min);

CostMatrix reduce_value_in_col(CostMatrix,col_min);

possible_paths find_path(CostMatrix);

path choose_path(possible_paths, CostMatrix);

CostMatrix del_rows_and_col(CostMatrix, path);

double get_forbidden_cost();

bool isNan1(double);

#endif
