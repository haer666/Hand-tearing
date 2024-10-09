#include <iostream>
#include <memory>
#include <mutex>
using namespace std;
template <typename T>
class m_unique_ptr {
public:
    ~m_unique_ptr()
    {
        cout << "destruct" << endl;
        release();
    }
    
    // 默认构造
    m_unique_ptr()
    {
        cout << "default constuct" << endl;
    }

    // 构造函数
    m_unique_ptr(T* ptr)
            : _ptr(ptr)
    {
        cout << "constuct" << endl;
    }

    // 拷贝构造函数
    m_unique_ptr(const m_unique_ptr& up)
    {
        // 深度拷贝
        _ptr = new T();
        *_ptr = *up._ptr;
        cout << "copy constuct" << endl;
    }

    // 赋值重载
    m_unique_ptr<T>& operator = (const m_unique_ptr<T>& up)
    {   
        // 防止自我赋值
        if (_ptr == up._ptr) 
            return *this;
        // 深度拷贝
        _ptr = new T();
        *_ptr = *up._ptr;
        cout << "= constuct" << endl;
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

    void release() 
    {
        if (_ptr == nullptr) 
            return;
        delete _ptr;
    }
    
private:
    T* _ptr;
};

int main() {
    {
        m_unique_ptr<int> p(new int(10));
        m_unique_ptr<int> p2(p);
        m_unique_ptr<int> p3;
        p3 = p2;
        cout << *p3 << endl;
    }
    return 0;
}