// A_star_search.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

/*
given :
map 
starting location
goal location
cost function

find:
optimal path

Search( grid, initial_point, goal_point ) :
vector<vector<State>> Search(vector<vector<State>> &grid, const int initial_point[2], const int goal_point[2]){
Initialize an empty list of open nodes.
vector<vector<int>> openlist{};
Initialize a starting node with the following:

x and y values given by initial_point.
g = 0, where g is the cost for each move.
h given by the heuristic function (a function of the current coordinates and the goal).
Add the new node to the list of open nodes.
int x = initial_point[0];
int y = initial_point[1];
int g = 0;
int h = Heuristic(x,y,goal_point[0],goal_point[1]);
vector<int> starting_node = {x,y,g,h}
while the list of open nodes is nonempty:
while (openlist.empty() == false){
Sort the open list by f-value
ListSort(openlist)
Pop the optimal cell (called the current cell).
vector<int> node = openlist.back();
openlist.pop_back();
Mark the cell's coordinates in the grid as part of the path.
grid[node[0]][node[1]] = State::kPath;
if the current cell is the goal cell:
return the grid.
if (node[0] == goal_point[0] && node[1] == goal_point[1]){
    return grid;
}
else, expand the search to the current node's neighbors. This includes the following steps:
else {
    ExpandNeighbors(node, &openlist);
}
Check each neighbor cell in the grid to ensure that the cell is empty: it hasn't been closed and is not an obstacle.
void ExpandNeighbors(vector<int> node//{x,y,g,h}//, vector<vector<State>> &openlist){
    vector<int> cell_west =  {x-1,y,g+1,Heuristic(x-1,y,goal_point[0],goal_point[1])};
    vector<int> cell_south =  {x,y-1,g+1,Heuristic(x,y-1,goal_point[0],goal_point[1])};
    vector<int> cell_east =  {x+1,y,g+1,Heuristic(x+1,y,goal_point[0],goal_point[1])};
    vector<int> cell_north =  {x,y+1,g+1,Heuristic(x,y+1,goal_point[0],goal_point[1])};
    if ValidCell(cell_west){
        openlist.push_back(cell_west);
    }
    if ValidCell(cell_south){
        openlist.push_back(cell_south);
    }
    if ValidCell(cell_east){
        openlist.push_back(cell_east);
    }
    if ValidCell(cell_north){
        openlist.push_back(cell_north);
    }
}
If the cell is empty, compute the cost (g value) and the heuristic, and add to the list of open nodes.
Mark the cell as closed.
If you exit the while loop because the list of open nodes is empty, you have run out of new nodes to explore and haven't found a path.
cout << "Path not found!" << std::endl;
return grid;
}


*/
#include <iterator>
#include <locale>
#include <codecvt>
#include <string>

#include <algorithm>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

enum class State {kEmpty, kObstacle, kPath, kClosed};


string u32_to_ascii(u32string const &s) {
  string out;
  transform(begin(s), end(s), back_inserter(out), [](char32_t c) {
    return c < 128 ? static_cast<char>(c) : '?';
  });
  return out;
}

int u32toi(std::u32string const &s) { return stoi(u32_to_ascii(s)); }

vector<State> ParseLine(string input_line){
    vector<State> row;
    istringstream string_line(input_line);
    int n;
    char c;
    while(string_line >> n >> c && c == ','){
        if (n == 0){
            row.push_back(State::kEmpty);
        }
        else {
            row.push_back(State::kObstacle);
        }
    }
    return row;
}

vector<vector<State>> ReadBoardFile(string path){
    vector<vector<State>> board {};
    ifstream myfile(path);
    if (myfile){
    string line;
    while (getline(myfile,line)){
        vector<State> row = ParseLine(line);
        board.push_back(row);
    }
    }
    return board;
}

int Heuristic(int x1, int y1, int x2, int y2){
    return std::abs(x1-x2) + std::abs(y1-y2);
}

bool Compare (const vector<int> node1, const vector<int> node2){
    int f_value_1 = node1[2] + node1[3]; // node1[2] = g_value, node1[3] = h_value, f_value = g_value + h_value
    int f_value_2 = node2[2] + node2[3];
    return f_value_1 > f_value_2; 
}

void ListSort(vector<vector<int>> *openlist){
    sort(openlist->begin(),openlist->end(),Compare);
}


bool CheckValidCell(int x, int y, vector<vector<State>> &grid) {
  bool on_grid_x = (x >= 0 && x < grid.size());
  bool on_grid_y = (y >= 0 && y < grid[0].size());
  if (on_grid_x && on_grid_y)
    return grid[x][y] == State::kEmpty;
  return false;
}

void ExpandNeighbors(const vector<int> &node,const int goal_point[2], vector<vector<int>> &openlist,vector<vector<State>> &grid){
    int x = node[0];
    int y = node[1];
    int g = node[2];

    vector<int> cell_west =  {x-1,y,g+1,Heuristic(x-1,y,goal_point[0],goal_point[1])};
    vector<int> cell_south =  {x,y-1,g+1,Heuristic(x,y-1,goal_point[0],goal_point[1])};
    vector<int> cell_east =  {x+1,y,g+1,Heuristic(x+1,y,goal_point[0],goal_point[1])};
    vector<int> cell_north =  {x,y+1,g+1,Heuristic(x,y+1,goal_point[0],goal_point[1])};
    if (CheckValidCell(cell_west[0],cell_west[1],grid)){
        int g2 = g + 1;
        int h2 = Heuristic(cell_west[0],cell_west[1],goal_point[0],goal_point[1]);
        openlist.push_back(vector<int> {cell_west[0],cell_west[1],g2,h2});
        grid[cell_west[0]][cell_west[1]] = State::kClosed;
    }
    if (CheckValidCell(cell_south[0],cell_south[1],grid)){
        int g2 = g + 1;
        int h2 = Heuristic(cell_south[0],cell_south[1],goal_point[0],goal_point[1]);
        openlist.push_back(vector<int> {cell_south[0],cell_south[1],g2,h2});
        grid[cell_south[0]][cell_south[1]] = State::kClosed;
    }
    if (CheckValidCell(cell_east[0],cell_east[1],grid)){
        int g2 = g + 1;
        int h2 = Heuristic(cell_east[0],cell_east[1],goal_point[0],goal_point[1]);
        openlist.push_back(vector<int> {cell_east[0],cell_east[1],g2,h2});
        grid[cell_east[0]][cell_east[1]] = State::kClosed;
    }
    if (CheckValidCell(cell_north[0],cell_north[1],grid)){
        int g2 = g + 1;
        int h2 = Heuristic(cell_north[0],cell_north[1],goal_point[0],goal_point[1]);
        openlist.push_back(vector<int> {cell_north[0],cell_north[1],g2,h2});
        grid[cell_north[0]][cell_north[1]] = State::kClosed;
    }

}

vector<vector<State>> Search(vector<vector<State>> &grid, const int initial_point[2], const int goal_point[2]){
vector<vector<int>> openlist{};
int x = initial_point[0];
int y = initial_point[1];
int g = 0;
int h = Heuristic(x,y,goal_point[0],goal_point[1]);
vector<int> node = {x,y,g,h};
openlist.push_back(node);
grid[x][y] = State::kClosed;

while (openlist.size() > 0){
ListSort(&openlist);
vector<int> node = openlist.back();
openlist.pop_back();
int x = node[0];
int y = node[1];
grid[x][y] = State::kPath;
if (node[0] == goal_point[0] && node[1] == goal_point[1]){
    return grid;
}
ExpandNeighbors(node,goal_point,openlist,grid);
}
  cout << "No path found!" << "\n";
  return vector<vector<State>>{};

}

/*

vector<vector<int>> search(vector<vector<State>> grid, vector<int> initial_point, vector<int> goal_point) {
    vector<vector<int>> empty_nodes;  // initialize empty list of nodes

    int x_value = initial_point[0]; // x value of starting location
    int y_value = initial_point[1]; // y value of starting location
    int cost = 0;  // cost for each move
    int heuristic_value = 0; // will be give value by heuristic_function(x_value,y_value,goal_point)

    empty_nodes.push_back(initial_point); // add initial_point to list of empty nodes

    while (empty_nodes.empty() == false) {
        cout << " i like it like it come on\n";
        break;
    }
    return { {1,1} };
}

*/

/*
u32string CellString(State cell) {
  switch(cell) {
    case State::kObstacle: return U"⛰️   ";
    case State::kPath: return U"🚗   ";
    default: return U"0   "; 
  }
}

*/
//  std::u32string str4 = U"This is a String";

char* char_utf32_to_utf8(char32_t utf32, const char* buffer)
// Encodes the UTF-32 encoded char into a UTF-8 string. 
// Stores the result in the buffer and returns the position 
// of the end of the buffer
// (unchecked access, be sure to provide a buffer that is big enough)
{
    char* end = const_cast<char*>(buffer);
    if(utf32 < 0x7F) *(end++) = static_cast<unsigned>(utf32);
    else if(utf32 < 0x7FF) {
        *(end++) = 0b1100'0000 + static_cast<unsigned>(utf32 >> 6);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    }
    else if(utf32 < 0x10000){
        *(end++) = 0b1110'0000 + static_cast<unsigned>(utf32 >> 12);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    } else if(utf32 < 0x110000) {
        *(end++) = 0b1111'0000 + static_cast<unsigned>(utf32 >> 18);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 12) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>((utf32 >> 6) & 0b0011'1111);
        *(end++) = 0b1000'0000 + static_cast<unsigned>(utf32 & 0b0011'1111);
    }
    else throw;
    *end = '\0';
    return end;
}


std::ostream& operator<<(std::ostream& os, const char32_t* s)
{
    const char buffer[5] {0}; // That's the famous "big-enough buffer"
    while(s && *s)
    {
        char_utf32_to_utf8(*(s++), buffer);
        os << buffer;
    }
    return os;
}




std::ostream& operator<<(std::ostream& os, const std::u32string& s)
{
    return (os << s.c_str());
}

void PrintBoard(const vector<vector<State>> board){
    
    
    for (int i = 0; i<board.size(); i++){
        for (int j = 0 ; j< board[i].size(); j++){
             if (board[i][j] == State::kObstacle){
                u32string mountain(U"⛰️   ");
                //for ( u32string::iterator it=mountain.begin(); it!=mountain.end(); ++it)
                //    wcout << *it;
                cout << u32string(U"\x26F0") ;
             }
            if (board[i][j] == State::kPath){
                u32string car(U"🚗   ");
                cout << u32string(U"\x1F697") ;
             }
            else {
                cout << "0   ";
            }
        }
        cout << "\n";
    }
}

int main()
{   


    int initial_point[2] = {0,0};
    int goal_point[2] = {4,5};
    vector<vector<State>> board = ReadBoardFile("board.txt");
    vector<vector<State>> solution_path = Search(board,initial_point,goal_point);
    PrintBoard(solution_path);
}



// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
