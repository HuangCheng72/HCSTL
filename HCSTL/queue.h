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

#include "vector.h"

template <typename T>
class priority_queue{
public:

    typedef vector<T> container_type;

    //使用底层容器的类型，这是适配器的一个特点，依赖于原组件提供的信息
    typedef typename vector<T>::value_type value_type;
    typedef typename vector<T>::size_type size_type;
    typedef typename vector<T>::reference reference;
    typedef typename vector<T>::const_reference const_reference;

    typedef typename vector<T>::iterator iterator;

private:

    //底层容器对象
    container_type cont;
    //指针距离计算结果类型
    typedef typename iterator_traits<iterator>::difference_type Distance;

    //建立堆所需要用到的操作工具和算法。
    //回顾我们建立堆的方法，在数组中，i从1起算时，i的孩子结点是i * 2和i * 2 + 1。
    //类似的，如果i从0起算，那么根据数学推理有，(i + 1) 的孩子结点是 (i + 1) * 2 和 (i + 1) * 2 + 1。
    //所以i的孩子结点是，i * 2 + 1和i * 2 + 2
    //我们使用的是vector，返回top的时候返回的是vector.front()，而且我们不知道输入的数据是什么，没办法在i = 0处放置哨兵结点。
    //因此我们采用i从0起算的方法。

    //交换两个迭代器所指向的元素
    void swap(Distance a, Distance b) {
        //提供迭代器交换这两个迭代器所对应的元素
        value_type temp = *(cont.begin() + a);
        *(cont.begin() + a) = *(cont.begin() + b);
        *(cont.begin() + b) = temp;
    }

    //判断a是否小于b
    bool less(Distance a, Distance b) {
        return *(cont.begin() + a) < *(cont.begin() + b);
    }

    //元素上浮
    void swim(Distance a) {

        Distance index = a;

        //若已经到达堆顶，不可能接着上浮
        while(index != 0) {

            //根据先前数学推理：i的孩子结点是，i * 2 + 1和i * 2 + 2
            //反推得i的父节点为 (i + 1) / 2 - 1

            Distance parent = (index + 1) / 2 - 1;

            //如果其不比父节点小，说明位置正确，停止上浮
            if(!less(index , parent)) {
                break;
            }
            //交换元素
            swap(index, parent);
            index = parent;

        }
    }

    //元素下沉
    void sink(Distance a) {

        Distance index = a;

        //若已经到达堆底，不可能接着下沉
        while(index * 2 + 1 < cont.size()) {    //至少有一个孩子可以比较的情况才行（也就是最坏情况最多允许左孩子(i *2 + 1)正好到cont.size() - 1，所以最多i * 2 + 2 == cont.size()）

            Distance min_child = index * 2 + 1; //左孩子无论如何都存在
            if(index * 2 + 2 < cont.size()) {
                //还有右孩子就比较右孩子
                if(less(index * 2 + 2, min_child)){
                    min_child = index * 2 + 2;
                }
            }
            //求出左右两个哪个孩子更小以后，与最小的孩子比较

            if(!less(min_child, index)){
                //如果最小的孩子都不比自身小，说明已经下沉到正确位置
                break;
            }
            swap(min_child, index);
            index = min_child;

        }
    }

public:

    /*-------构造器与析构器相关函数--------*/

    priority_queue(){
        //如果不构造这个，vector的默认构造器直接把三个指针全部设置为nullptr，出问题就在这里。
        cont = *(new container_type(4));
    }

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
        swim(Distance (cont.size() - 1));
    }

    void pop(){
        //交换堆顶和堆底，然后把堆的范围缩小
        swap(0, cont.size() - 1);
        //删除堆底
        cont.erase(cont.end() - 1);
        //堆顶元素下沉
        sink(0);
    }

    void clear(){
        cont.clear();
    }

    /*-------容器数据相关函数完--------*/

};

#endif //HCSTL_QUEUE_H
