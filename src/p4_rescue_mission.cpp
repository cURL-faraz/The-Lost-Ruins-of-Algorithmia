#include <iostream>
#include <vector>
#include <queue> 
#include <algorithm>
using namespace std; 

struct Edge{
int vertex_num; 
int rev; 
int cap;
}; 

vector<vector<int>> adj_cells = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

void add_neighbors(const vector<char>& cells, int i, int j, int r, int c, vector<vector<Edge>>& neighbors)
{
if(cells[i * c + j] != '#'){
for(const auto& cell : adj_cells){
int cell_r = i + cell[0], cell_c = j + cell[1]; 
if(cell_r >= 0 && cell_r < r && cell_c >= 0 && cell_c < c && cells[cell_r *c + cell_c] != '#')
{
neighbors[i * c + j + r * c].push_back({cell_r * c + cell_c, (int)neighbors[cell_r * c + cell_c].size()}); 
neighbors[cell_r * c + cell_c].push_back({i * c + j + r * c, (int)neighbors[i * c + j + r * c].size() - 1, 0}); 
}
}
}
}

int bfs(int source, int target, int r, int c, vector<vector<Edge>>& adj_list, vector<int>& level){
for(int i = 0; i < 2 * r * c; i++){
level[i] = -1; 
}
queue<int> q; 
level[source] = 0; 
q.push(source); 

while(!q.empty()){
int curr_v = q.front(); 
auto& neighbors = adj_list[curr_v];
for(auto& u : neighbors){
// the condition can be shorter I guess 
bool has_cap = (abs(u.vertex_num - curr_v) == r * c && curr_v != source - r * c && curr_v != target && 
curr_v != source && curr_v != target + r * c ) || (abs(u.vertex_num - curr_v) != r * c && curr_v < r * c);

if(level[u.vertex_num] == -1 && (!has_cap || u.cap > 0 )){
level[u.vertex_num] = level[curr_v] + 1; 
q.push(u.vertex_num); }}
q.pop(); 
}
return level[target]; 
}

int dfs(int source, int sink, int r, int c,  vector<vector<Edge>>& adj_list, vector<int>& level, vector<int>& ptr, int flow_in = 1){
if(source == sink)
{
return flow_in;     
}
auto& neighbors = adj_list[source]; 
for(int i = ptr[source]; i < neighbors.size(); i++){
auto& u = neighbors[i];  
// level condition - edges' capacity check
bool has_cap = (abs(source - u.vertex_num) == r * c && source != sink + r * c && level[u.vertex_num] != 0) || 
(abs(source - u.vertex_num) != r * c && source < r * c);  
if(level[u.vertex_num] != level[source] + 1 || (has_cap && u.cap == 0))
{
ptr[source] ++; 
}
else{
int max_flow = dfs(u.vertex_num, sink, r, c, adj_list, level, ptr, (has_cap) ? min(flow_in, u.cap) : flow_in); 
if(max_flow == 0)
{
ptr[source] ++; 
}
else{
// source =  x in ---> u = y out 
if(abs(source - u.vertex_num) != r * c && source < r * c){
adj_list[source][i].cap -= max_flow; 
}
// source = v in ---> u = v out , v != S,E 
else if(u.vertex_num - source == r * c && level[u.vertex_num] != 0){
adj_list[source][i].cap -= max_flow; 
adj_list[source + r * c][u.rev].cap += max_flow; 
}
// source = x out ---> u = y in 
else if(abs(source - u.vertex_num) != r * c && source >= r * c){
adj_list[u.vertex_num][u.rev].cap += max_flow; 
}
// source = v out ---> u = v in , v != S,E 
else if(source - u.vertex_num == r * c && source != sink + r * c && level[source] != 0){
adj_list[source][i].cap -= max_flow; 
adj_list[u.vertex_num][u.rev].cap += max_flow; 
}
return max_flow; 
}
}
}
return 0; 
}

int main(){

ios::sync_with_stdio(false);
cin.tie(nullptr);

int r, c; 
cin >> r >> c; 
vector<char> cells(r * c); 
for(int i = 0; i < r; i++)
{
for(int j = 0; j < c; j++){
cin >> cells[i * c + j]; 
}
}

vector<vector<Edge>> adj_list(2 * r * c); 
int sr, sc, er, ec; 
cin >> sr >> sc >> er >> ec;

if(sr == er && sc == ec){
cout << 1; 
return 0; 
}
for(int v = 0; v < r * c; v++)
{
adj_list[v].push_back({v + r * c}); 
if(v != sr * c + sc && v != er * c + ec)
{
adj_list[v][0].rev = 0;
adj_list[v][0].cap = 1; 
adj_list[v + r * c].push_back({v, 0, 0}); 
}
}
for(int i = 0; i < r; i++)
{
for(int j = 0; j < c; j++)
{
add_neighbors(cells, i, j, r, c, adj_list); 
}}

vector<int> level(2 * r * c); 
vector<int> ptr(2 * r * c); 
long long int num_p = 0; 

while(bfs(sr * c + sc + r * c, er * c + ec, r, c, adj_list, level) != -1){
for(int v = 0; v < 2 * r * c; v++){
ptr[v] = 0; 
}
while(ptr[sr * c + sc + r * c] < adj_list[sr * c + sc + r * c].size())
{
num_p += dfs(sr * c + sc + r * c, er * c + ec, r, c, adj_list, level, ptr);  
}
}

cout << num_p; 
}

