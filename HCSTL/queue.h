//
// Created by huangcheng on 2023/3/5.
//

#ifndef HCSTL_QUEUE_H
#define HCSTL_QUEUE_H

//以vector为基础实现最小优先队列。
//利用已有的组件提供的能力，拓展出新功能新特性，或者大大加强组件某一方面的能力形成新组件，这种新组件就是所谓的适配器（配接器，adapter）。
//适配器不是独立的组件，它依赖于已有的组件。
//比如优先队列基于vector实现，它就是一种容器适配器，同样的还有基于deque实现的queue和stack。
//比如迭代器中有一种反向迭代器，它就是正向迭代器的适配器。

//这里先求基于vector实现最小优先队列，基于其他的容器和其他的比较规则的实现之后再说。
//注意，在这里你是接触不到容器内的指针，你只能接触到迭代器。
//优先队列并不提供迭代器，它只能访问堆最顶端的值。

#include "deque.h"
#include "functional.h"
#include "heap.h"

//函数无法作为模板参数，但是结构体名可以，所以比较器这个模板参数默认值应当采用函数对象，在之前我们在算法for_each中提供的是函数对象也是一样的道理，所以要将比较函数变成函数对象
template <typename T, typename Compare = less<T>, typename Container = deque<T> >
class priority_queue{
public:

    typedef Compare value_compare;

    //使用底层容器的类型，这是适配器的一个特点，依赖于原组件提供的信息
    typedef typename Container::value_type value_type;
    typedef typename Container::size_type size_type;
    typedef typename Container::reference reference;
    typedef typename Container::const_reference const_reference;

    typedef typename Container::iterator iterator;

private:

    //底层容器对象
    Container cont;
    //指针距离计算结果类型
    typedef typename iterator_traits<iterator>::difference_type Distance;
    //比较器
    value_compare comp;

public:
    //注意！Priority_Queue不对外提供迭代器！很多适配器并不对外提供迭代器。

    /*-------构造器与析构器相关函数--------*/

    priority_queue() = default;

    ~priority_queue() = default;

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
        return cont.front();
    }

    void push (const T& val) {
        cont.push_back(val);
        push_heap(cont.begin(),cont.end(), comp);
    }

    void pop(){
        pop_heap(cont.begin(),cont.end(), comp);
        cont.pop_back();
    }

    void clear(){
        cont.clear();
    }

    /*-------容器数据相关函数完--------*/

};

template <typename T, typename Container = deque<T> >
class queue{
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

    queue() = default;

    ~queue() = default;

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

    reference front() {
        return cont.front();
    }
    reference back() {
        return cont.back();
    }

    void push (const T& val) {
        cont.push_back(val);
    }

    void pop(){
        cont.pop_front();
    }

    void clear(){
        cont.clear();
    }

    /*-------容器数据相关函数完--------*/

};

#endif //HCSTL_QUEUE_H
