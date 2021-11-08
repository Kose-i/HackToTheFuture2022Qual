#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <utility>

using namespace std;
using Pair = std::pair<int, int>;

int main(int argc, char** argv) {
    int n, m, k, r;
    cin >> n >> m >> k >> r;
    vector<vector<int>> d(n, vector<int>(k));
    for (int i=0;i<n;++i) for (int j=0;j<k;++j) cin >> d[i][j];
    vector<Pair> path(r);
    for (int i=0;i<r;++i) cin >> path[i].first >> path[i].second;

}
