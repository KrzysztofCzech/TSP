//
// Created by kczec on 30.10.2019.
//
#include <tsp.hpp>

#include "tsp.hpp"


CostMatrix::CostMatrix(const std::vector<std::vector<double>> m ) : Matrix_(m) {
    for (int i=0;i<m.size();i++){
        remain_col.push_back(i);
        remain_rows.push_back(i);
    }
}

const std::vector<double>& CostMatrix::operator[](std::size_t pos) const {
    return Matrix_[pos];
}

std::vector<double>& CostMatrix::operator[](std::size_t pos) {
    return Matrix_[pos];
}

auto CostMatrix::len() {
    return Matrix_.size();
}

const std::vector<int> CostMatrix::col() const{
    return remain_col;
}

const std::vector<int> CostMatrix::row() const{
    return remain_rows;
}

auto CostMatrix::colbegin() {
    return remain_col.begin();
}

auto CostMatrix::colend() {
    return remain_col.end();
}

auto CostMatrix::rowbegin() {
    return remain_rows.begin();
}


auto CostMatrix::rowend() {
    return remain_rows.end();
}
void CostMatrix::del_row(int n){
    remain_rows.erase(remain_rows.begin()+n);
    Matrix_.erase(Matrix_.begin()+n);
}

void CostMatrix::del_col(int n){
    remain_col.erase(remain_col.begin()+n);
    for (auto& x : Matrix_){
        x.erase(x.begin()+n);
    }
}

rows_min find_min_in_rows(CostMatrix M){
    rows_min min;
    for (auto i = 0; i < M.len(); i++){
        double smallest = M[i][0];
        for (auto j = 1; j< M.col().size(); j++){
            if (M[i][j] < smallest or std::isnan(smallest)){
                smallest = M[i][j];
            }
        }
        min.push_back(smallest);
    }
    return min;
}

col_min find_min_in_col(CostMatrix M){
    col_min min;
    for (auto i = 0; i < M.col().size(); i++){
        double smallest = M[0][i];
        for (auto j = 1; j< M.row().size(); j++){
            if (M[j][i] < smallest || std::isnan(smallest)){
                smallest = M[j][i];
            }
        }
        min.push_back(smallest);
    }
    return min;
}

CostMatrix reduce_value_in_rows(CostMatrix M,rows_min min){
    for (auto i = 0; i < M.row().size(); i++){
        for (auto j = 0; j< M.col().size(); j++){
            M[i][j] -= min[i];
        }
    }
    return M;
}

CostMatrix reduce_value_in_cols(CostMatrix M,col_min min){
    for (auto i = 0; i < M.row().size(); i++){
        for (auto j = 0; j< M.col().size(); j++){
            M[i][j] -= min[j];
        }
    }
    return M;
}

possible_paths find_path(CostMatrix M){
    possible_paths paths;
    for (auto i = 0; i < M.row().size(); i++){
        for (auto j = 0; j< M.col().size(); j++){
            if (M[i][j] == 0){
                paths.push_back(std::make_pair(i,j));
            }
        }
    }
    return paths;
}

bool isNan1(double x){
    return std::isnan(x);
}

path choose_path(possible_paths paths, CostMatrix M) {
    std::vector<double> paths_value;
    double min_col, min_row;
    for (auto &el : paths) {
        min_col = INF;
        min_row = INF;
        for (int i = 0; i < M.row().size(); i++) {
            if ((M[i][el.second] < min_col || std::isnan(min_col)) && i != el.first) {
                min_col = M[i][el.second];
            }
            if ((min_row > M[el.first][i] || std::isnan(min_row)) && i != el.second) {
                min_row = M[el.first][i];
            }
        }
        paths_value.push_back(min_col + min_row);
    }
    if (std::find_if(paths_value.begin(), paths_value.end(), isNan1) != paths_value.end()) {
        return paths[std::find_if(paths_value.begin(), paths_value.end(), isNan1) - paths_value.begin()];
    } else {
        return paths[std::max_element(paths_value.begin(), paths_value.end()) - paths_value.begin()];
    }
}



CostMatrix del_rows_and_col(CostMatrix M, path p){
    if (std::find(M.rowbegin(), M.rowend(), M.col()[p.second]) != M.rowend() && std::find(M.colbegin(), M.colend(), M.row()[p.first]) != M.colend()){
        auto j = std::distance(M.colbegin(),std::find(M.colbegin(), M.colend(), M.row()[p.first]));
        auto i = std::distance(M.rowbegin(),std::find(M.rowbegin(), M.rowend(), M.col()[p.second]));
        M[i][j] = INF;
    }
    M.del_col(p.second);
    M.del_row(p.first);
    return M;
}


std::vector<int> tsp(std::vector<std::vector<double>> m) {
    std::vector<int> chosen_path;
    CostMatrix matrix = CostMatrix(m);
    possible_paths  paths;
    rows_min   min1;
    col_min   min2;
    path path1;
    double value = 0;
    std::vector<std::pair<int,int>> unsorted_paths;
    while (matrix.row().size()){
        min1 = find_min_in_rows(matrix);
        matrix = reduce_value_in_rows(matrix , min1);
        min2 = find_min_in_col(matrix);
        matrix = reduce_value_in_cols(matrix, min2);
        value += std::accumulate(min1.begin(), min1.end(), 0.0);
        value += std::accumulate(min2.begin(),min2.end(),0.0);
        paths = find_path(matrix);
        path1 = choose_path(paths, matrix);
        unsorted_paths.emplace_back(std::make_pair(matrix.row()[path1.first],matrix.col()[path1.second]));
        matrix = del_rows_and_col(matrix, path1);
    }

    chosen_path.push_back(unsorted_paths[0].first+1);
    chosen_path.push_back(unsorted_paths[0].second+1);
    int temp = unsorted_paths[0].second;
    unsorted_paths.erase(unsorted_paths.begin());
    while (!unsorted_paths.empty()){
        int i =0;
        for (auto& el : unsorted_paths){
            if (el.first == temp){
                chosen_path.push_back(el.second+1);
                temp = el.second;
                unsorted_paths.erase(unsorted_paths.begin()+i);
                break;
            }
            i++;
        }
    }

    return  chosen_path;
}

double get_forbidden_cost(){
    return NAN;
}