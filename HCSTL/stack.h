//
// Created by huangcheng on 2023/3/19.
//

#ifndef HCSTL_STACK_H
#define HCSTL_STACK_H

#include "deque.h"

template <typename T, typename Container = deque<T> >
class stack{
public:

    //使用底层容器的类型，这是适配器的一个特点，依赖于原组件提供的信息
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    typedef typename Container::iterator iterator;

private:

    //底层容器对象
    Container cont;

public:
    //注意！Queue不对外提供迭代器！很多适配器并不对外提供迭代器。

    /*-------构造器与析构器相关函数--------*/

    stack() = default;

    ~stack() = default;

    /*-------构造器与析构器相关函数完--------*/

    /*-------容器容量相关函数--------*/

    bool empty() const {
        return cont.empty();
    }

    size_type size() const {
        return cont.size();
    }

    /*-------容器容量相关函数完--------*/

    /*-------容器数据相关函数--------*/

    reference top() {
        return cont.back();
    }

    void push (const T& val) {
        cont.push_back(val);
    }

    void pop(){
        cont.pop_back();
    }

    void clear(){
        cont.clear();
    }

    /*-------容器数据相关函数完--------*/

};

#endif //HCSTL_STACK_H
