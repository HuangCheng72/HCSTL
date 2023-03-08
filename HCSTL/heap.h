//
// Created by huangcheng on 2023/3/7.
//

#ifndef HCSTL_HEAP_H
#define HCSTL_HEAP_H

//这里是建堆，入堆，出堆，堆排序这些堆的相关算法

//根据C++的编码规范，内部实现的函数应该在前面加上双下划线（专名号），以示区分
//辅助实现的函数，应当加上_aux后缀
//例如，要实现一个push_heap：
//push_heap是对外暴露的函数，也是给用户使用的函数，它内部应该直接调用__push_heap_aux。
//__push_heap_aux，是辅助实现的函数，它的作用就是调用__push_heap。
//__push_heap，才是真正实现的函数。

//这一方式的好处，例如：
//你在优先队列里面很多地方调用了push_heap，但是你现在想替换底层实现了，比如有个更高效的建堆方法，你要替换。
//那么这时候你只需要替换__push_heap就行了，如果你直接在push_heap实现，到时候可能会出现很多地方都要改代码的情况。
//__push_heap_aux主要起到一个分发的作用，一个__push_heap_aux可能会根据情况调用多个__push_heap的实现。

//模板参数名字前面都应该加上单下划线（专名号）_ ，这是为了防止和用户自定义的类型重名出现命名冲突。
//在参数前面加上双下划线同理。
//在C++17规范中，已经明确禁止了在函数前加下划线前缀，因为这是给STL的实现专用的。
//我们做的虽然是一个Demo或者说玩具程度的STL，但是也应当适用这一规范。

#include "iterator.h"


template <typename _RandomAccessIterator, typename _Distance, typename _T>
void __push_heap(_RandomAccessIterator __first, _Distance __topIndex, _Distance __targetIndex, _T __value) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮，这是一个更高效的上浮法，有点像下沉。
    //它的做法是一路往上比较，如果发现parent位置元素比value小，就把parent扔到child位置。
    //最终停下来的时候，child位置就是value位置。

    _Distance parent = (__targetIndex - 1) / 2;
    //若已经到达堆顶，不可能接着上浮
    while(__targetIndex > __topIndex) {

        //根据先前数学推理：i的孩子结点是，i * 2 + 1和i * 2 + 2
        //反推得i的父节点为 (i + 1) / 2 - 1，其实用（i - 1） / 2就行了，结果是一样的

        //STL 默认的是最大堆，所以这里应该是发现如果其比父节点小，说明位置正确，停止上浮
        if( __value < *(__first + parent)  ) {
            break;
        }
        //把元素往下扔
        *(__first + __targetIndex) = *(__first + parent);
        //更新下标
        __targetIndex = parent;
        parent = (__targetIndex - 1) / 2;
    }

    //这里就是value的位置
    *(__first + __targetIndex) = __value;

}


template <typename _RandomAccessIterator, typename _Distance, typename _T>
void __push_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Distance*, _T*) {
    //传入_Distance*和_T*的指针是为了模板参数的完成自动推导
    __push_heap(__first, _Distance(0) , _Distance((__last - __first) - 1) , _T(*(__last - 1)));
}


/**
 * 一个元素已经放到 heap 容器底部，将这个元素插入到 heap 中正确的位置（上浮），使用 < 进行比较
 * @tparam _RandomAccessIterator 对任何随机访问迭代器适用
 * @param __first 容器头
 * @param __last 容器尾
 */
template <typename _RandomAccessIterator>
void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __push_heap_aux(__first, __last, __distance_type(__first), __value_type(__first));
}




template <typename _RandomAccessIterator, typename _Distance, typename _T, typename _Compare>
void __push_heap(_RandomAccessIterator __first, _Distance __topIndex, _Distance __targetIndex, _T __value, _Compare __comp) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮

    _Distance parent = (__targetIndex - 1) / 2;
    //若已经到达堆顶，不可能接着上浮
    while(__targetIndex > __topIndex) {

        //根据先前数学推理：i的孩子结点是，i * 2 + 1和i * 2 + 2
        //反推得i的父节点为 (i + 1) / 2 - 1，其实用（i - 1） / 2就行了，结果是一样的

        //STL 默认的是最大堆，所以这里应该是发现如果其比父节点小，说明位置正确，停止上浮
        if( __comp(__value, *(__first + parent)) ) {
            break;
        }
        //把元素往下扔
        *(__first + __targetIndex) = *(__first + parent);
        //更新下标
        __targetIndex = parent;
        parent = (__targetIndex - 1) / 2;
    }

    //这里就是value的位置
    *(__first + __targetIndex) = __value;
}

template <typename _RandomAccessIterator, typename _Distance, typename _T, typename _Compare>
void __push_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Distance*, _T*, _Compare __comp) {
    //传入_Distance*和_T*的指针是为了模板参数的完成自动推导
    __push_heap(__first, _Distance(0) , _Distance((__last - __first) - 1) , _T(*(__last - 1)), __comp);
}


/**
 * 一个元素已经放到 heap 容器底部，将这个元素插入到 heap 中正确的位置（上浮），使用给定比较器进行比较
 * @tparam _RandomAccessIterator 对任何随机访问迭代器适用
 * @tparam _Compare 比较器类型
 * @param __first 容器头
 * @param __last 容器尾
 * @param __comp 给定的比较器对象
 */
template <typename _RandomAccessIterator, typename _Compare>
void push_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    __push_heap_aux(__first, __last, __distance_type(__first), __value_type(__first), __comp);
}


template <typename _RandomAccessIterator, typename _Distance, typename _T>
void __pop_heap(_RandomAccessIterator __first, _Distance __targetIndex, _Distance __bottomIndex, _T __value) {
    //四项参数分别是，容器开头迭代器，目标（堆顶）元素所在下标，堆底元素所在下标，将被交换到堆顶的堆底元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）

    //首先，将堆顶元素放到堆底，即堆顶元素出堆
    *(__first + __bottomIndex) = *(__first + __targetIndex);

    //然后往下下沉，找到__value的应有位置

    _Distance max_child;

    //如果已经到达新的堆底（之前堆顶元素已经出堆了，所以不能等于__bottomIndex），则不能再下沉
    while(__targetIndex * 2 + 1 < __bottomIndex) {

        //找出最大的孩子结点，因为左孩子无论如何都存在，所以默认为左孩子
        max_child = __targetIndex * 2 + 1;
        //如果有右孩子就要比较右孩子
        if(__targetIndex * 2 + 2 < __bottomIndex && *(__first + max_child) < *(__first + __targetIndex * 2 + 2) ) {
            max_child = __targetIndex * 2 + 2;
        }

        //如果最大的孩子都比__value小，说明这里就是__value的位置
        if( *(__first + max_child) < __value) {
            break;
        }
        //不是__value的位置，就换最大的孩子到当前target位置上来
        *(__first + __targetIndex) = *(__first + max_child);
        //更新位置到原最大孩子处
        __targetIndex = max_child;
    }
    //出循环的时候，__targetIndex就是__value应在的位置
    *(__first + __targetIndex) = __value;

}


template <typename _RandomAccessIterator, typename _Distance, typename _T>
void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Distance*, _T*) {
    //传入_Distance*和_T*的指针是为了模板参数的完成自动推导
    __pop_heap(__first, _Distance(0) , _Distance((__last - __first) - 1) , _T(*(__last - 1)));
}


/**
 * 将堆顶元素出堆，放到堆底，并对其余元素重新成堆（下沉），使用 < 运算符进行比较
 * @tparam _RandomAccessIterator 对任何随机访问迭代器适用
 * @param __first 容器头
 * @param __last 容器尾
 */
template <typename _RandomAccessIterator>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last) {
    __pop_heap_aux(__first, __last, __distance_type(__first), __value_type(__first));
}




template <typename _RandomAccessIterator, typename _Distance, typename _T, typename _Compare>
void __pop_heap(_RandomAccessIterator __first, _Distance __targetIndex, _Distance __bottomIndex, _T __value, _Compare __comp) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮

    //首先，将堆顶元素放到堆底，即堆顶元素出堆
    *(__first + __bottomIndex) = *(__first + __targetIndex);

    //然后往下下沉，找到__value的应有位置

    _Distance max_child;

    //如果已经到达新的堆底（之前堆顶元素已经出堆了，所以不能等于__bottomIndex），则不能再下沉
    while(__targetIndex * 2 + 1 < __bottomIndex) {

        //找出最大的孩子结点，因为左孩子无论如何都存在，所以默认为左孩子
        max_child = __targetIndex * 2 + 1;
        //如果有右孩子就要比较右孩子
        if(__targetIndex * 2 + 2 < __bottomIndex && *(__first + max_child) < *(__first + __targetIndex * 2 + 2) ) {
            max_child = __targetIndex * 2 + 2;
        }

        //如果最大的孩子都比__value小，说明这里就是__value的位置
        if( __comp(*(__first + max_child) , __value) ) {
            break;
        }
        //不是__value的位置，就换最大的孩子到当前target位置上来
        *(__first + __targetIndex) = *(__first + max_child);
        //更新位置到原最大孩子处
        __targetIndex = max_child;
    }
    //出循环的时候，__targetIndex就是__value应在的位置
    *(__first + __targetIndex) = __value;
}

template <typename _RandomAccessIterator, typename _Distance, typename _T, typename _Compare>
void __pop_heap_aux(_RandomAccessIterator __first, _RandomAccessIterator __last, _Distance*, _T*, _Compare __comp) {
    //传入_Distance*和_T*的指针是为了模板参数的完成自动推导
    __pop_heap(__first, _Distance(0) , _Distance((__last - __first) - 1) , _T(*(__last - 1)), __comp);
}


/**
 * 将堆顶元素出堆，放到堆底，并对其余元素重新成堆（下沉），使用给定比较器进行比较
 * @tparam _RandomAccessIterator 对任何随机访问迭代器适用
 * @tparam _Compare 比较器类型
 * @param __first 容器头
 * @param __last 容器尾
 * @param __comp 给定的比较器对象
 */
template <typename _RandomAccessIterator, typename _Compare>
void pop_heap(_RandomAccessIterator __first, _RandomAccessIterator __last, _Compare __comp) {
    __pop_heap_aux(__first, __last, __distance_type(__first), __value_type(__first), __comp);
}

#endif //HCSTL_HEAP_H
