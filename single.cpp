#include <iostream>
#include <thread>
#include <mutex>
#include <memory>

// 懒汉模式 局部静态变量实现 
// 可以保证线程安全
class Singleton_1 {
public:
    ~Singleton_1() {
        std::cout << "Singleton_1 destructor called" << std::endl;
    }
    Singleton_1(const Singleton_1& single) = delete;
    Singleton_1& operator = (const Singleton_1& single) = delete;

    static Singleton_1& getInstance() {
        static Singleton_1 instance;
        return instance;
    }
private:
    Singleton_1() {
        std::cout << "Singleton_1 constructor called" << std::endl;
    }
};
// 调用两次，只会初始化静态局部变量一次
void test1() {
    Singleton_1& sin_1 = Singleton_1::getInstance();
    Singleton_1& sin_2 = Singleton_1::getInstance();
}

// 懒汉模式 指针实现 有缺陷的单例模式
class Singleton_2 {
public:
    ~Singleton_2() {
        std::cout << "Singleton_2 destorctor called" << std::endl;
    }
    Singleton_2(const Singleton_2& single) = delete;
    Singleton_2& operator = (const Singleton_2& single) = delete;
    
    static Singleton_2* getInstance() {
        if (ptr_instance == nullptr) {
            ptr_instance = new Singleton_2();
        }
        return ptr_instance;
    }
private:
    Singleton_2() {
        std::cout << "Singleton_2 constructor called" << std::endl;
    }
    static Singleton_2* ptr_instance;
};

Singleton_2* Singleton_2::ptr_instance = nullptr; //类私有的静态成员可以在类外被定义和初始化 但不能被访问

void test2() {
    Singleton_2* ptr_instance_1 = Singleton_2::getInstance();
    Singleton_2* ptr_instance_2 = Singleton_2::getInstance();

}


// 懒汉模式 优化的指针实现 加锁 使用共享指针
class Singleton_3 {
public:
    ~Singleton_3() {
        std::cout << "Singleton_3 destorctor called" << std::endl;
    }
    Singleton_3(const Singleton_3& single) = delete;
    Singleton_3& operator = (const Singleton_3& single) = delete;
    
    static std::shared_ptr<Singleton_3> getInstance() {
        {
            std::lock_guard<std::mutex> lock(mtx);          // 加锁
            if (s_ptr_instance == nullptr) {
                // s_ptr_instance = std::make_shared<Singleton_3>();        // error
                s_ptr_instance = std::shared_ptr<Singleton_3>(new Singleton_3);
            }
        }
        return s_ptr_instance;
    }
private:
    Singleton_3() {
        std::cout << "Singleton_3 constructor called" << std::endl;
    }
    static std::shared_ptr<Singleton_3> s_ptr_instance;
    static std::mutex mtx;
};

using s_ptr = std::shared_ptr<Singleton_3>;

s_ptr Singleton_3::s_ptr_instance = nullptr; // 类私有的静态成员可以在类外被定义和初始化 但不能被访问
std::mutex Singleton_3::mtx; // 类私有的静态成员可以在类外被定义和初始化 但不能被访问

void test3() {
    s_ptr s_ptr_instance_1 = Singleton_3::getInstance();
    s_ptr s_ptr_instance_2 = Singleton_3::getInstance();
}

// 饿汉模式 静态成员变量实现 
class Singleton_4 {
public:
    ~Singleton_4() {
        std::cout << "Singleton_4 destructor called" << std::endl;
    }
    Singleton_4(const Singleton_4& single) = delete;
    Singleton_4& operator = (const Singleton_4& single) = delete;

    static Singleton_4& getInstance() {
        return instance;
    }
private:
    Singleton_4() {
        std::cout << "Singleton_4 constructor called" << std::endl;
    }

    static Singleton_4 instance;
};
Singleton_4 Singleton_4::instance;
// 调用两次，只会初始化静态局部变量一次
void test4() {
    // int a = 0;
    // std::cin >> a;
    Singleton_4& sin_1 = Singleton_4::getInstance();
    Singleton_4& sin_2 = Singleton_4::getInstance();
}

int main()
{
    // test1();
    // test2();
    // test3();
    test4();
    return 0;
}


