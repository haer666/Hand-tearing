#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>

class ThreadPool {
public:
    ThreadPool(int numThreads) : stop(false) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                while (true) {
                    std::unique_lock<std::mutex> lock(mutex);
                    condition.wait(lock, [this] { return stop || !tasks.empty(); });
                    if (stop && tasks.empty()) {
                        return;
                    }
                    std::function<void()> task(std::move(tasks.front()));
                    tasks.pop();
                    lock.unlock();
                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(mutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& thread : threads) {
            thread.join();
        }
    }

    template<typename F, typename... Args>
    void enqueue(F&& f, Args&&... args) {
        std::function<void()> task(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(mutex);
            tasks.emplace(std::move(task));
        }
        condition.notify_one();
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex mutex;
    std::condition_variable condition;
    bool stop;
};

int index = 0;
void func() {
    std::cout << "Task " << index << " is running in thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "Task " << index << " is done" << std::endl;
    ++index;
}

void func1(int a, int b) {
    std::cout << "thread " << std::this_thread::get_id() << " is running in thread " << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "thread " << std::this_thread::get_id() << " is done" << std::endl;
    std::cout << "the result is : " << a + b << std::endl;
}

int main() {
    ThreadPool pool(4);
    // for (int i = 0; i < 8; ++i) {
    //     pool.enqueue([i] {
    //         std::cout << "Task " << i << " is running in thread " << std::this_thread::get_id() << std::endl;
    //         std::this_thread::sleep_for(std::chrono::seconds(1));
    //         std::cout << "Task " << i << " is done" << std::endl;
    //     });
    // }
    for (int i = 0; i < 8; ++i) {
        pool.enqueue(func1, i, i);
    }
    return 0;
}