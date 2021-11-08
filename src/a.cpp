#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <utility>

// Assign random
// not use d

using namespace std;
using Pair = std::pair<int, int>;
const int end_days {2000};

int main(int argc, char** argv) {
    int n, m, k, r;
    cin >> n >> m >> k >> r;
    vector<vector<int>> d(n, vector<int>(k));
    for (int i=0;i<n;++i) for (int j=0;j<k;++j) cin >> d[i][j];
    vector<Pair> path(r);
    for (int i=0;i<r;++i) cin >> path[i].first >> path[i].second;

    queue<int> person_available;
    for (int i=0;i<m;++i) person_available.push(i+1);

    map<int, int> task_before_cnt;
    vector<vector<int>> task_relation(n+1);
    for (int i=0;i<r;++i) {
        task_relation[path[i].first].push_back(path[i].second);
        ++task_before_cnt[path[i].second];
    }

    queue<int> executable_task;
    for (int i=0;i<n;++i) {
        if (task_before_cnt[i+1] == 0) executable_task.push(i+1);
    }

    queue<int> end_task;
    map<int, int> member_to_task;

    for (int i=0;i<end_days;++i) {
        int cnt = std::min(person_available.size(), executable_task.size());
        vector<Pair> output_ans(cnt);
        for (int j=0;j<cnt;++j) {
            int person = person_available.front(); person_available.pop();
            int exec_task = executable_task.front(); executable_task.pop();
            member_to_task[person] = exec_task;
            output_ans[j] = make_pair(person, exec_task);
        }
        cout << cnt;
        for (Pair& e : output_ans) {
            cout << ' ' << e.first << ' ' << e.second;
        }
        cout << std::endl;

        int input_cnt;
        cin >> input_cnt;
        if (input_cnt == -1) break;
        vector<int> input_data(input_cnt);
        for (int j=0;j<input_cnt;++j) {
            cin >> input_data[j];
        }
        for (int j=0;j<input_cnt;++j) {
            int task_num = member_to_task[input_data[j]];
            person_available.push(input_data[j]);
            //cout << task_num << '-' << task_relation[task_num].size() << std::endl;
            for (auto& e : task_relation[task_num]) {
                --task_before_cnt[e];
                if (task_before_cnt[e] == 0) executable_task.push(e);
            }
        }
    }
}
