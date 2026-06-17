#include <iostream>
#include <vector>
#include <queue> 
#include <cmath>
#include <iomanip>

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
distance[i] = 10000; 
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
for(int i = 0; i < r; i++){
for(int j = 0; j < c; j++)
{
cin >> cells[i * c + j]; 
}
}

vector<vector<int>> adj_list(r * c); 
for(int i = 0; i < r; i++){
for(int j = 0; j < c; j++){
add_neighbors(cells, i, j, r, c, adj_list[i * c + j]); 
}
}

int num_p = 0; 
long long int sum_pl = 0; 
vector<int> distance(r * c);
for(int v = 0; v < r * c; v++){
bfs(v, adj_list, distance); 
for(int u = 0; u < r * c; u++)
{
if(distance[u] != 0 && distance[u] != 10000)
{
sum_pl += distance[u]; 
num_p ++; 
}
}
}

float ans = (num_p != 0 ) ? (float)sum_pl / (float)num_p : 0.00; 
ans = trunc(ans * 100.0) / 100.0;  
cout << fixed << setprecision(2) << ans; 
}