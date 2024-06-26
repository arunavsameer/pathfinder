#include <bits/stdc++.h>
#include <fstream>
using namespace std;

#define height 8
#define width 20

struct block{
    pair <int, int> position;
    int g_cost = 0;
    int h_cost = 0;
    block* parent = nullptr;
};

char maze[height][width];
static block grid[height][width];
vector <block*> open;
vector <block*> closed;
vector <block*> neighbours;
block *start, *finish ;

bool in_open(block* A){
    for(auto i: open){
        if(i -> position == A -> position){
            return true;
        }
    }
    return false;
}

bool in_closed(block* A){
    for(auto i: closed){
        if(i -> position == A -> position){
            return true;
        }
    }
    return false;
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

void reset_grid(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            grid[i][j].position = (make_pair(i, j));
        }
    }
}

bool can_go(pair <int, int> posn){
    return (maze[posn.first][posn.second] == ' ' || maze[posn.first][posn.second] == 'e') ? true : false;
}

int dist(pair <int, int> A, pair <int, int> B){
    int base = abs(B.second - A.second);
    int hgt = abs(B.first - A.first);
    float distance = sqrt((base * base) + (hgt * hgt));
    int h = (distance * 10);
    return h;
}

void get_neighbours(pair <int, int> position){
    neighbours.clear();
    for(int i = position.first - 1; i <= position.first + 1; i++){
        for(int j = position.second - 1; j <= position.second + 1; j++){
            pair <int, int> posn = make_pair(i, j);
            if(can_go(posn) && !(i == position.first && j == position.second)){
                neighbours.push_back(&grid[i][j]);
            }
        }
    }
}

bool compare_fc(block *A, block *B){
    return ((A -> g_cost + A -> h_cost) >= (B -> g_cost + B -> h_cost)) ? 0 : 1;
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

void find_path(){
    //cout << open.size() <<" : ";
    // for(auto i: open){
    //     cout << i->g_cost + i -> h_cost <<" ";
    // }
    sort(open.begin(), open.end(), compare_fc);
    block* current = open.at(0);
    if(current == finish){
        return;
    }
    //cout << " : ";
    //cout << current->position.first <<" "<<current->position.second <<" "<< current->g_cost <<endl;
    open.erase(open.begin());
    closed.push_back(current);
    get_neighbours(current -> position);
    for(auto & neighbour: neighbours){
        if(in_open(neighbour)){
            if((current -> g_cost + dist(neighbour->position, current->position)) < neighbour -> g_cost){
                neighbour -> g_cost = (current -> g_cost + dist(neighbour->position, current->position));
                neighbour -> parent = current;
            }
        }else if(!(in_closed(neighbour) || in_open(neighbour))){
            int g_c = (current -> g_cost + dist(neighbour->position, current->position));
            int h_c = dist(neighbour -> position, finish -> position);
            neighbour -> g_cost = g_c;
            neighbour -> h_cost = h_c;
            neighbour -> parent = current;
            open.push_back(neighbour);
        }
    }
    find_path();
}

pair <int, int>  parent(pair <int, int> posn){
    for(auto i: closed){
        if(i -> position == posn){
            return i -> parent->position;
        }
    }
}

void add_path(){
    block* current = closed.at(closed.size() - 1);
    while(current -> position != start -> position){
        maze[current-> position.first][current->position.second] = '*';
        current = current -> parent;
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

int main(){
    reset_grid();
    get_maze();
    print_maze();
    open.clear();
    closed.clear();
    neighbours.clear();

    int* ptr = get_points();
    int start_x = *ptr; ptr++;
    int start_y = *ptr; ptr++;
    int final_x = *ptr; ptr++;
    int final_y = *ptr;

    start = &grid[start_x][start_y];
    finish = &grid[final_x][final_y];
    start -> g_cost = 0;
    start -> parent = nullptr;
    open.push_back(start);
    find_path();
    cout << "path found: " <<endl;
    add_path();
    print_maze();
}