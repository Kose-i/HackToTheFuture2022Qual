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
using ll = long long;

constexpr LLINF {998244353};

struct TaskType {
    int task_id;
    vector<int> weights;
    int is_available; // 0 is executable
    int urgency = -1;
    int start_time, end_time;
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
struct PersonType {
    int person_id;
    vector<vector<int>> params;
    PersonType(const int& id, const int& param_size) : person_id(id), param(param_size, ) {}
};
ll calc(const TaskType& task, const PersonType& person) {
    if (person.params.size() == 0) return -1;
    vector<ll> param_sum(task.weights.size(), 0);
    for (const auto& e : person.params) {
        for (auto i=0;i<e.size();++i) param_sum[i] += e[i];
    }
    ll ans {};
    for (int i=0;i<task.weights.size();++i) {
        int p = param_sum[i] / (person.params.size()+1);
        ans += std::max(0, task.weights[i] - p);
    }
    return ans;
}
void update_person(PersonType& person, const TaskType& task) {
    vector<int> p = task.weights;
    for (int i=0;i<p;++i) p[i] = task.end_time - task.start_time - p[i];
    person.params.push_back();
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
    vector<PersonType> people(m);
    for (int i=0;i<m;++i) {
        PersonType tmp(i, k);
        people[i] = tmp;
    }

    queue<int> person_available;
    for (int i=0;i<m;++i) {
        person_available.push(i);
    }

    queue<int> end_task;
    map<int, int> member_to_task;

    for (int i=0;i<end_days;++i) {
        int cnt = std::min(person_available.size(), executable_task.size());
        vector<Pair> output_ans(cnt);
        for (int j=0;j<cnt;++j) {//Assign executable_task[j]
            map<int, int> mp_person_use;
            for (;person_available.size() != 0;) {
                mp_person_use[person_available.front()] = 1;
                person_available.pop();
            }
            ll min_val {LLINF};
            TaskType exec_task = executable_task.top(); executable_task.pop();
            for (auto& p : mp_person_use) min_val = std::min(min_val,calc(exec_task, people[p.first]));

            int person_id {-1};

            for (auto& p : mp_person_use) {
                if (person_id == -1 && min_val == calc(exec_task, people[p.first])) {
                    person_id = p.first;
                } else {
                    person_available.push(p.first)
                }
            }
            exec_task.start_time = i;
            member_to_task[person_id] = exec_task.task_id;
            output_ans[j] = make_pair(person_id, exec_task.task_id);
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
