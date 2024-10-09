#include <iostream>
#include <memory>
#include <mutex>
using namespace std;


template <typename T>
class m_shared_ptr {
public:
    // 析构函数
    ~m_shared_ptr()
    {
        release();
    }

    // 构造函数
    m_shared_ptr(T* ptr)
            : _ptr(ptr)
            , _count(new int(1))
            , _mtx(new mutex())
    {}

    // 拷贝构造函数
    m_shared_ptr(const m_shared_ptr& sp)
            : _ptr(sp._ptr)
            , _count(sp._count)
            , _mtx(sp._mtx)
    {
        addcount();
    }

    // 赋值重载
    m_shared_ptr<T>& operator = (const m_shared_ptr<T>& sp)
    {   
        // 防止自我赋值
        if (_ptr == sp._ptr) 
            return *this;
        // 维护原来的共享指针
        release();
        // 维护新指向的共享指针
        _ptr = sp._ptr;
        _count = sp._count;
        _mtx = sp._mtx;
        addcount();
        return *this;
    }

    T& operator * () 
    {
        return *_ptr;
    }

    T* operator -> ()
    {
        return _ptr;
    }
    
    void addcount()
    {
        _mtx->lock();
        ++(*_count);
        _mtx->unlock();
    }

    int count()
    {
        return *_count;
    }

    // 释放共享指针
    void release() 
    {
        if (_ptr == nullptr) 
            return;
        _mtx->lock();
        if (--(*_count) == 0) {
            delete _ptr;
        }
        _mtx->unlock();
        if (*_count == 0) {
            delete _count;
            delete _mtx;
        }
    }
private:
    T* _ptr;
    // 思考为什么使用指针变量
    int* _count;    
    mutex* _mtx;
};

int main() {
    m_shared_ptr<int> p(new int(10));
    m_shared_ptr<int> p2(p);
    cout << *p << endl;
    cout << p.count() << endl;
    return 0;
}