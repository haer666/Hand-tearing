#include <iostream>
#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
using namespace std;

class ReadWriteLock{
private:
    mutex _mtx;
    condition_variable _r_cv;
    condition_variable _w_cv;
    int _reader_num;
    bool _writing;
public:
    ReadWriteLock() : _reader_num(0), _writing(false) {}
    ~ReadWriteLock() {}
    // 加读锁
    void lockRead() {
        unique_lock<mutex> ulock(_mtx);
        // 若当前正在写，则阻塞
        _r_cv.wait(ulock, [this] { return !_writing; });
        ++_reader_num;
    }

    // 释放读锁
    void unlockRead() {
        unique_lock<mutex> ulock(_mtx);
        --_reader_num;
        // 读者数量为0，唤醒写者
        if (_reader_num == 0) {
            _w_cv.notify_one();
        }
    }

    void lockWrite() {
        unique_lock<mutex> ulock(_mtx);
        // 若当前正在写或者正在读，则阻塞
        _w_cv.wait(ulock, [this] { return !_writing && _reader_num == 0; });
        _writing = true;
    }

    void unlockWrite() {
        unique_lock<mutex> ulock(_mtx);
        _writing = false;
        _r_cv.notify_one(); // 唤醒读者
        _w_cv.notify_one(); // 唤醒写者
    }
};

ReadWriteLock rwLock;

void readOp(int id) {
    while (true) {
        rwLock.lockRead();
        cout << "Reader " << id << " is reading" << endl;
        rwLock.unlockRead();
        this_thread::sleep_for(chrono::seconds(1));
    }
}

void writeOp(int id) {
    while (true) {
        rwLock.lockWrite();
        cout << "Writer " << id << " is writing" << endl;
        rwLock.unlockWrite();
        this_thread::sleep_for(chrono::seconds(1));
    }
}

int main() {
    vector<thread> readThreads;
    vector<thread> writeThreads;
    // 4个读者    
    for (int i = 1; i < 5; ++i) {
        readThreads.push_back(thread(readOp, i));
    }

    // 2个写者
    for (int i = 1; i < 3; ++i) {
        readThreads.push_back(thread(readOp, i));
    }

    for (int i = 1; i < 5; ++i) {
        readThreads[i].join();
    }
    for (int i = 1; i < 3; ++i) {
        writeThreads[i].join();
    }
    return 0;
}