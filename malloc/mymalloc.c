#include<unistd.h>

#define BLOCK_SIZE 24

typedef struct m_block
{
    size_t size;        // 数据区大小   
    struct m_block* next;      // 指向下一个块的指针
    int free;           // 是否时空闲块
    int padding;        // 填充4字节，保证块的大小为8的倍数
    char data[1];       // 虚拟字段，表示数据块的第一个字节
}m_block;

// 全局维护的第一个block
m_block* first_block = NULL;
// 没有找到合适的block时，last指向最后一个block，用于扩展block链表
m_block* last_block = NULL;

// size对8进行对齐
size_t align8(size_t size) {
    if (size & 0x7 == 0)
        return size;
    return ((size >> 3) + 1) << 3;
}
// 找到第一个满足大于size的block 没找到时last_block指向最后一个block
m_block* find_block(size_t size);
// 扩展堆
m_block* extend_heap(size_t size);
// 切割当前的块
// m_block* split_block(m_block* block, size_t size);

void* malloc(size_t size)
{
    m_block* b;
    size_t s = align8(size);        // 对齐地址
    if (first_block) {
       last_block = first_block;
       b = find_block(s);
       if (b) {
            b->free = 1;            // 找到后修改标志位
       } else {
            b = extend_heap(s);
            if (!b) return NULL;    // 拓展失败
       }
    } else {
        b = extend_heap(s);
        if (!b) return NULL;
        first_block = b;
    }
    return b->data;
}

int main() {

    return 0;
}

m_block* find_block(size_t size)
{
    m_block* b = first_block;
    while (b) {
        if (b->free && b->size >= size) {
            return b;
        } else {
            last_block = b;
            b = b->next;
        }
    }
    return b;
}

m_block* extend_heap(size_t size)
{
    m_block* b;
    b = sbrk(0);    // 获取当前break指针
    if (sbrk(BLOCK_SIZE + size) == (void*)-1) 
        return NULL;
    b->size = size;
    b->next = NULL;
    b->free = 0;
    if (last_block) 
        last_block->next = b;
    return b;
}
