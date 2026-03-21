#include "src.hpp"
#include <iostream>

void test_linked_hashmap_advanced() {
    sjtu::linked_hashmap<int, int> map;
    
    // 测试1：空链表操作
    std::cout << "测试空链表:" << std::endl;
    try {
        map.begin()--;
        std::cout << "FAIL: begin()-- 应该抛异常" << std::endl;
    } catch(...) {
        std::cout << "PASS: begin()-- 抛异常" << std::endl;
    }
    
    try {
        map.end()++;
        std::cout << "FAIL: end()++ 应该抛异常" << std::endl;
    } catch(...) {
        std::cout << "PASS: end()++ 抛异常" << std::endl;
    }
    
    // 测试2：插入后遍历
    std::cout << "\n测试插入和遍历:" << std::endl;
    for(int i = 1; i <= 5; i++) {
        map.insert({i, i * 100});
    }
    
    std::cout << "顺序: ";
    for(auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << std::endl;
    
    // 测试3：重复插入不改变顺序
    map.insert({3, 350});
    std::cout << "重复插入3后顺序: ";
    for(auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << std::endl;
    
    // 测试4：删除中间元素
    auto it = map.find(3);
    if(it != map.end()) {
        map.remove(it);
    }
    std::cout << "删除3后顺序: ";
    for(auto it = map.begin(); it != map.end(); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << std::endl;
    
    // 测试5：end()-- 操作
    std::cout << "\n测试 end()--:" << std::endl;
    auto last = map.end();
    --last;
    std::cout << "最后一个元素: " << last->first << std::endl;
    
    // 测试6：const 迭代器
    std::cout << "\n测试 const 迭代器:" << std::endl;
    const auto& cmap = map;
    for(auto it = cmap.cbegin(); it != cmap.cend(); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << std::endl;
    
    // 测试7：拷贝构造
    sjtu::linked_hashmap<int, int> map2 = map;
    std::cout << "\n拷贝后顺序: ";
    for(auto it = map2.begin(); it != map2.end(); ++it) {
        std::cout << it->first << " ";
    }
    std::cout << std::endl;
    
    std::cout << "\n所有测试完成!" << std::endl;
}

int main() {
    test_linked_hashmap_advanced();
    return 0;
}
