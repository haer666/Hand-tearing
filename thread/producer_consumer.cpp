#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <queue>
using namespace std;

std::queue<int> str_que;
std::mutex mtx;
std::condition_variable con;

void producer() {
    int i = 0;
    while (true) {
        {
            std:::unique_lock<std::mutex> u_lock(mtx);
            str_que.push(i);
        }
        con.notify_one();
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "producer add " << i << endl;
    }
}

void cosumer() {
    int i = -1;
    while (true) {
        {
            std:::unique_lock<std::mutex> u_lock(mtx);
            con.wait(u_lock, [] { return !str_que.empty(); } );
            i = str_que.front();
            str_que.pop();
        }
        std::this_thread::sleep_for(std::chrono::seconds(2));
        cout << "consumer delete " << i << endl;
    }
}

int main() {
    std::thread th1(producer);
    std::thread th2(consumer);
    th1.join();
    th2.join();

    return 0;
}