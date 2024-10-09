#include <iostream>
#include <unordered_map>
#include <string>
using namespace std;

/*
*   实现LRU算法 原题：力扣146
*   注意：get()和put()的平均时间复杂度要求为O(1)，使用双向链表和哈希表来实现。
*/

// 双向链表
// 注意使用struct和使用class有什么区别
struct Linkedlist {
    // 思考为什么要设置key这个字段
    int key;
    int value;
    Linkedlist* prev;
    Linkedlist* next;

    Linkedlist(int key, int val)
        : key(key)
        , value(val)
        , prev(nullptr)
        , next(nullptr)
    { }
};

class LRUCache {
private:
    int cache_size;
    int count;
    // 头部表示最旧的访问的数据，尾部表示最新的数据
    Linkedlist* head;
    Linkedlist* tail;
    // <a, node>a表示key, node->val表示value
    // 使用hash表可以在O(1)的时间复杂度内定位到节点
    unordered_map<int, Linkedlist*> record;
public:
    // 构造函数
    LRUCache(int capacity)
        : cache_size(capacity)
        , count(0)
        , head(new Linkedlist(-1, -1))
        , tail(new Linkedlist(-1, -1))
    {
        head->next = tail;
        tail->prev = head;
    }
    
    // 析构函数
    ~LRUCache() {
        while (head->next != tail) {
            Linkedlist* node = head->next;
            head->next = node->next;
            node->next->prev = head;
            delete node;
        }
        delete head;
        delete tail;
    }

    // 根据key来获取value，若不存在则返回-1
    int get(int key) {
        if (record.find(key) == record.end())
            return -1;
        // 通过hash快速定位节点
        int val = record[key]->value;
        // 访问后需要更新当前key-value在缓存中的位置
        update(key, val);
        return val;
    }

    // 增加key-value
    // key存在：更新key-value的value和key-target在缓存中的位置
    // key不存在：增加新的key-value
    void put(int key, int value) {
        // key存在
        if (record.find(key) != record.end()) {
            update(key, value);
            record[key]->value = value;
            return;
        }
        Linkedlist* newNode = new Linkedlist(key, value);
        record.emplace(key, newNode);
        // 缓存已满则替换最旧的key-value
        if (count == cache_size) {
            record.erase(front());
            pop_front();
            count--;
        }
        push_back(newNode);
        count++;
    }

private:
    // 返回头部第一个(最旧)的key-value的key
    int front() {
        return head->next->key;
    }

    void pop_front() {
        if (count == 0) return;
        Linkedlist* tmp = head->next;
        head->next = tmp->next;
        tmp->next->prev = head;
        delete tmp;
    }

    void push_back(Linkedlist* node) {
        Linkedlist* tmp = tail->prev;
        tail->prev = node;
        node->next = tail;
        tmp->next = node;
        node->prev = tmp;
    }

    // 更新key-target在缓存中的位置
    void update(int key, int target) {
        // if (record.find(key) == record.end()) return;
        Linkedlist* node = record[key];
        node->value = target;
        // key-target是最新的缓存就直接返回
        if (node->next == tail) return;
        // 删除原位置的key-target
        Linkedlist* tmp = node->prev;
        tmp->next = node->next;
        node->next->prev = tmp;
        // 将key-target更新到尾部(最新的缓存)
        push_back(node);
    }
};
