#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std; 

vector<vector<int>> adj_cells = {{-1, 0}, {0, -1}, {0, 1}, {1, 0}};

void add_neighbors(const vector<char>& cells, int i, int j, int r, int c, vector<int>& neighbors)
{
if(cells[i * c + j] != '#'){
for(const auto& cell : adj_cells){
int cell_r = i + cell[0], cell_c = j + cell[1]; 
if(cell_r >= 0 && cell_r < r && cell_c >= 0 && cell_c < c && cells[cell_r * c + cell_c] != '#')
{
neighbors.push_back(cell_r * c + cell_c); 
}
}
}
}

void bfs(int source, const vector<vector<int>>& adj_list, vector<int>& distance){
 
for(int i = 0; i < distance.size(); i++){
distance[i] = 1e6; 
}
distance[source] = 0; 
queue<int> q; 
q.push(source); 

while(!q.empty()){
int current_v = q.front(); 
for(auto u : adj_list[current_v])
{
if(distance[current_v] + 1 < distance[u]){
distance[u] = distance[current_v] + 1; 
q.push(u); 
}}
q.pop(); 
}

}

int main(){

ios::sync_with_stdio(false);
cin.tie(nullptr);

int r, c; 
cin >> r >> c; 
vector<char> cells(r * c); 
vector<int> treasures; 
for(int i = 0; i < r; i++)
{
for(int j = 0; j < c; j++){
cin >> cells[i * c + j]; 
if(cells[i * c + j] == 'T'){
treasures.push_back(i * c + j); 
}
}
}

vector<vector<int>> adj_list(r * c); 
for(int i = 0; i < r; i++){
for(int j = 0; j < c; j++){
add_neighbors(cells, i, j, r, c, adj_list[i * c + j]); 
}
}

int sr, sc, er, ec, K; 
cin >> sr >> sc >> er >> ec >> K; 
vector<int> source_distance(r * c); 
bfs(sr * c + sc, adj_list, source_distance); 

if(source_distance[er * c + ec] == 1e6 || K > treasures.size()){
cout << -1; 
}
else if(K == 0){
cout << source_distance[er * c + ec]; 
}
else{
vector<int> reachable_treasures; 
vector<vector<int>> vertex_distance; 
vertex_distance.push_back(source_distance); 
for(auto t : treasures)
{
if(source_distance[t] != 1e6){
bfs(t, adj_list, source_distance); 
vertex_distance.push_back(source_distance); 
reachable_treasures.push_back(t); 
}
}
int min_M = 1e6; 
if(!reachable_treasures.empty()){
int t = reachable_treasures.size(); 
vector<int> dp((1 << t) * t, 1e6); 
for(int mask = 1; mask < (1 << t); mask++){
int num_t = __builtin_popcount(mask); 
for(int m_last = mask; m_last; m_last &= m_last - 1){
int last = __builtin_ctz(m_last);
int subset_mask = mask ^ (1 << last); 
if(subset_mask){
for(int m_prev_last = subset_mask; m_prev_last; m_prev_last &= m_prev_last - 1){
int prev_last = __builtin_ctz(m_prev_last); 
dp[mask * t + last] = min(dp[mask * t + last], dp[subset_mask * t + prev_last] + vertex_distance[prev_last + 1][reachable_treasures[last]]);
}
}
else{
dp[mask * t + last] = vertex_distance[0][reachable_treasures[last]];  
}
if(num_t >= K){
min_M = min(min_M, dp[mask * t + last] + vertex_distance[last + 1][er * c + ec]); 
}
}
}}
min_M = (min_M != 1e6) ? min_M : -1; 
cout << min_M; 
}

}