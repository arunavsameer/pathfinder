#include <bits/stdc++.h>
using namespace std;
struct block{
    int f_cost;
    pair <int, int> position;
    pair<int, int> parent;

    block(int f_c){
        f_cost = f_c;
        position = make_pair(0, 0);
        parent = make_pair(0, 0);
    }
};
bool compare_fc(block A, block B){
    return (A.f_cost > B.f_cost) ? 0 : 1;
}
vector <block> open;
int main(){
    open.push_back(block(1));
    open.push_back(block(0));
    open.push_back(block(3));
    sort(open.begin(), open.end(), compare_fc);
    for(auto i: open){
        cout << i.f_cost <<" ";
    }
}