#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <utility>
#include <cmath>

// Assign random
// task-priority
// not use d

using namespace std;
using Pair = std::pair<int, int>;
constexpr int end_days {2000};

struct TaskType {
    int task_id;
    vector<int> weights;
    int is_available; // 0 is executable
    int urgency = -1;
    vector<int> next;
    vector<int> prev;
};
bool operator<(const TaskType& l, const TaskType& r) {
    return l.urgency < r.urgency;
}
int update_urgency(vector<TaskType>& tasks, int idx) {
    if (tasks[idx].urgency != -1) return tasks[idx].urgency;
    int val {tasks[idx].weights[0]};
    for (int& e : tasks[idx].weights) val = std::max(val, e);
    int tmp {};
    for (const int& e : tasks[idx].next) {
        tmp = std::max(tmp, update_urgency(tasks, e));
    }
    tasks[idx].urgency = val + tmp;
    return val + tmp;
}

int main(int argc, char** argv) {
    int n, m, k, r;
    cin >> n >> m >> k >> r;
    vector<TaskType> tasks(n);
    for (int i=0;i<n;++i) {
        tasks[i].task_id = i;
        vector<int> a(k);
        for (int j=0;j<k;++j) cin >> a[j];
        tasks[i].weights = a;
        tasks[i].is_available = 0;
    }
    for (int i=0;i<r;++i) {
        int l, r;
        cin >> l >> r;
        tasks[l-1].next.push_back(r-1);
        tasks[r-1].prev.push_back(l-1);
    }
    for (int i=0;i<n;++i) {
        update_urgency(tasks, i);
    }

    priority_queue<TaskType> executable_task;
    for (int i=0;i<n;++i) {
        tasks[i].is_available = tasks[i].prev.size();
        if (tasks[i].is_available == 0) executable_task.push(tasks[i]);
    }

    queue<int> person_available;
    for (int i=0;i<m;++i) person_available.push(i);

    queue<int> end_task;
    map<int, int> member_to_task;

    for (int i=0;i<end_days;++i) {
        int cnt = std::min(person_available.size(), executable_task.size());
        vector<Pair> output_ans(cnt);
        for (int j=0;j<cnt;++j) {
            int person = person_available.front(); person_available.pop();
            TaskType exec_task = executable_task.top(); executable_task.pop();
            member_to_task[person] = exec_task.task_id;
            output_ans[j] = make_pair(person, exec_task.task_id);
        }
        cout << cnt;
        for (Pair& e : output_ans) {
            cout << ' ' << e.first+1 << ' ' << e.second+1;
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
            int task_num = member_to_task[input_data[j]-1];
            person_available.push(input_data[j]-1);
            for (auto& e : tasks[task_num].next) {
                --tasks[e].is_available;
                if (tasks[e].is_available == 0) executable_task.push(tasks[e]);
            }
        }
    }
}
