#include <bits/stdc++.h>
#include <fstream>
using namespace std;

#define height 8
#define width 20

char maze[height][width];
int start_x, start_y, final_x, final_y;

struct block{
    int f_cost;
    pair <int, int> position;
    pair<int, int> parent;

    block(int f_c, pair<int, int> posn, pair<int, int> prt){
        f_cost = f_c;
        position = posn;
        parent = prt;
    }
};

vector <block> open;
vector <block> closed; 
vector <block> neighbours;

block* find(block &B, vector <block> &A){
    for(auto &i: A){
        if(B.position == i.position){
            return &i;
        }
    }
    return nullptr;
}

void get_maze(){
    fstream file;
    file.open("maze.txt");
    string line;
    int line_no = 0;
    while(getline(file, line)){
        for(int j = 0; j < width; j++){
            maze[line_no][j] = line[j];
        }
        line_no++;
    }
    file.close();
}

bool can_go(int i, int j){
    return (maze[i][j] == ' ' || maze[i][j] == 'e') ? true : false;
}

float dist(int i, int j, int x, int y){
    int base = abs(y - j);
    int hgt = abs(x - i);
    float distance = sqrt((base * base) + (hgt * hgt));
    int h = (distance * 10);
    return distance;
}

void get_neighbours(block A){
    neighbours.clear();
    for(int i = A.position.first - 1; i <= A.position.first + 1; i++){
        for(int j = A.position.second - 1; j <= A.position.second + 1; j++){
            if(can_go(i, j)){
                int f_c = dist(i, j, A.parent.first, A.parent.second) + dist(i, j, final_x, final_y);
                pair <int, int> pos = make_pair(i, j);
                pair <int, int> prt = A.position;
                neighbours.push_back(block(f_c, pos, prt));
            }
        }
    }
}

void print_maze(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            cout << maze[i][j];
        }
        cout << endl;
    }
}

int* get_points(){
    static int arr[4];
    int* p = &arr[0];
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            if(maze[i][j] == 's'){
                arr[0] = i;
                arr[1] = j;
            }else if(maze[i][j] == 'e'){
                arr[2] = i;
                arr[3] = j;
            }else{
                continue;
            }
        }
    }
    return p;
}

bool compare_fc(block A, block B){
    return (A.f_cost > B.f_cost) ? 0 : 1;
}

void find_path(block start, block final){
    //cout << start.position.first <<" "<<start.position.second <<endl;
    get_neighbours(start);
    closed.push_back(open.at(0));
    open.erase(open.begin());
    for(auto &neighbour: neighbours){
        //cout << neighbour.position.first << " "<<neighbour.position.second <<endl;
        if(neighbour.position == final.position){
            return;
        }

        block* found_open = find(neighbour, open);
        block* found_closed = find(neighbour, closed);
        //neighbour found in open
        if(found_open != nullptr){
            if(found_open -> position > neighbour.position){
                found_open -> f_cost = neighbour.f_cost;
                found_open -> parent = neighbour.parent;
            }
        }
        //neighbour not found in closed and open
        else if(found_closed == nullptr){
            open.push_back(neighbour);
        }
    }
    sort(open.begin(), open.end(), compare_fc);
    //cout << open.at(0).position.first <<" "<<open.at(0).position.second <<endl;
    find_path(open.at(0), final);
}

pair <int, int>  parent(pair <int, int> posn){
    for(auto i: closed){
        if(i.position == posn){
            return i.parent;
        }
    }
}

void add_path(){
    pair <int, int>  current = closed.at(closed.size() - 1).position;
    while(current != make_pair(start_x, start_y)){
        maze[current.first][current.second] = '*';
        current = parent(current);
    }
}


int main(){
    get_maze();
    open.clear();
    closed.clear();
    neighbours.clear();
    
    int* ptr = get_points();
    start_x = *ptr; ptr++;
    start_y = *ptr; ptr++;
    final_x = *ptr; ptr++;
    final_y = *ptr;
    
    pair <int, int> S, F;
    S = make_pair(start_x, start_y);
    F = make_pair(final_x, final_y);
    block start(0, S, S);
    open.push_back(start);
    block final(0, F, F);

    find_path(start, final);
    add_path();
    print_maze();
}