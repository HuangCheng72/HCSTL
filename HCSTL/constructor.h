//
// Created by huangcheng on 2023/3/2.
//

#ifndef HCSTL_CONSTRUCTOR_H
#define HCSTL_CONSTRUCTOR_H

#include "util.h"

//这里负责对象的构造和析构操作，主要是应用placement new

template <typename T>
void construct(T *p) {
    //默认构造器，在传入指针p指向的内存空间中构造对象
    new (p) T();
}

template <typename T, typename... Value_Type>
void construct(T *p, const Value_Type&... value) {
    //带参构造器，参数为value
    new (p) T(value...);
}

template <typename T, typename... Value_Type>
static void construct(T* p, Value_Type&&... value){
    //为了保证一定调用移动构造器，要move处理一遍t，防止t是左值引用。
    new(p) T(move(value)...);
}

template <typename T>
void destroy(T* p) {
    //析构器
    p->~T();
}

#endif //HCSTL_CONSTRUCTOR_H
