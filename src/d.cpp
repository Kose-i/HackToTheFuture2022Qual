#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <utility>
#include <set>

using ll = long long;
constexpr ll LLINF {98244353};
constexpr int end_days {2000};

struct TaskType {
    int task_id;
    std::vector<int> d;
    int is_available;
    int begin_time, end_time;
    int urgency = -1;
    TaskType() {}
    TaskType(const int& id, const int& k) : task_id(id), d(k), is_available(0) {}
};
int search_urgency(std::vector<TaskType>& tasks, const int& idx, const std::vector<std::vector<int>>& path);
bool operator<(const TaskType& l, const TaskType& r);

struct PersonType {
    int person_id;
    std::vector<std::vector<int>> params;
    PersonType() {}
    PersonType(const int& id, const int& k) : person_id(id) {}
};
ll calc(const TaskType& task, const PersonType& person);
void update_person(PersonType& person, const TaskType& task);

int main(int argc, char** argv) {
    int n, m, k, r;
    std::cin >> n >> m >> k >> r;
    std::vector<TaskType> tasks(n);
    for (int i=0;i<n;++i) {
        TaskType tmp(i, k);
        tasks[i] = tmp;
    }
    for (int i=0;i<n;++i) for (int j=0;j<k;++j) std::cin >> tasks[i].d[j];
    std::vector<std::vector<int>> path(n);
    for (int i=0;i<r;++i) {
        int l, r;
        std::cin >> l >> r;
        path[l-1].push_back(r-1);
        ++tasks[r-1].is_available;
    }
    //for (auto i = 0;i < n;++i) {
    //    std::cout << i << '-';
    //    for (const auto& f : path[i]) {
    //        std::cout << f << ',';
    //    }
    //    std::cout << '\n';
    //}

    for (int i=0;i<n;++i) search_urgency(tasks, i, path);

    std::vector<PersonType> people(m);
    for (int i=0;i<m;++i) {
        PersonType tmp(i, k);
        people[i] = tmp;
    }

    std::set<int> person_set;
    for (int i=0;i<m;++i) person_set.insert(i);
    std::priority_queue<TaskType> task_queue;
    for (int i=0;i<n;++i) if (tasks[i].is_available==0) task_queue.push(tasks[i]);
    std::map<int, int> member_to_task_mp;

    for (int day=0;day<end_days;++day) {
        // Output
        //std::cout << "task-size = " << task_queue.size() << '\n';
        int cnt = std::min(task_queue.size(), person_set.size());
        std::vector<std::pair<int, int>> output_vec(cnt);
        for (int i=0;i<cnt;++i) {
            TaskType task_num = task_queue.top(); task_queue.pop();
            ll min_val {LLINF};
            int person_id {};
            for (auto it = person_set.begin(); it != person_set.end(); ++it) {
                if (calc(tasks[task_num.task_id], people[*it]) < min_val) {
                    min_val = calc(tasks[task_num.task_id], people[*it]);
                    person_id = *it;
                }
            }
            tasks[task_num.task_id].begin_time = day;
            output_vec[i] = std::make_pair(person_id, task_num.task_id);
            member_to_task_mp[person_id] = task_num.task_id;
            person_set.erase(person_id);
        }
        std::cout << output_vec.size();
        for (const auto& e : output_vec) {
            std::cout << ' ' << e.first+1 << ' ' << e.second+1;
        }
        std::cout << std::endl;
        // Input
        int input_vec_size;
        std::cin >> input_vec_size;
        if (input_vec_size == -1) break;
        std::vector<int> input_vec(input_vec_size);
        for (int i=0;i<input_vec_size;++i) std::cin >> input_vec[i];

        for (int i=0;i<input_vec_size;++i) {
            --input_vec[i];
            int task_num = member_to_task_mp[input_vec[i]];
            //std::cout << "input_vec[i] = " << input_vec[i] << '\n';
            //std::cout << "task         = " << task_num << "\n";
            tasks[task_num].end_time = day;
            update_person(people[input_vec[i]], tasks[task_num]);
            for (const auto& e : path[task_num]) {
                --tasks[e].is_available;
                if (tasks[e].is_available==0) task_queue.push(tasks[e]);
            }
            person_set.insert(input_vec[i]);
        }
    }
}
int search_urgency(std::vector<TaskType>& tasks, const int& idx, const std::vector<std::vector<int>>& path) {
    if (tasks[idx].urgency != -1) return tasks[idx].urgency;
    int val {tasks[idx].d[0]};
    for (int& e : tasks[idx].d) val = std::max(val, e);
    int tmp {};
    for (const int& e : path[idx]) {
        tmp = std::max(tmp, search_urgency(tasks, e, path));
    }
    tasks[idx].urgency = val + tmp;
    return val + tmp;
}
bool operator<(const TaskType& l, const TaskType& r) {
    return l.urgency < r.urgency;
}

ll calc(const TaskType& task, const PersonType& person) {
    if (person.params.size() == 0) return -1;
    std::vector<ll> param_sum(task.d.size(), 0);
    for (const auto& e : person.params) {
        for (auto i=0;i<e.size();++i) param_sum[i] += e[i];
    }
    ll ans {};
    for (int i=0;i<task.d.size();++i) {
        int p = param_sum[i] / (person.params.size()+1);
        ans += std::max(0, task.d[i] - p);
    }
    return ans;
}
void update_person(PersonType& person, const TaskType& task) {
    std::vector<int> p = task.d;
    for (int i=0;i<p.size();++i) p[i] = task.end_time - task.begin_time - p[i];
    person.params.push_back(p);
}
