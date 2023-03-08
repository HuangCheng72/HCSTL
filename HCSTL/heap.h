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
//加上这些单双下划线的，都是所谓的保留名，是编译器自带的STL实现专用的，不允许给用户使用
//所以我们只能借鉴和参考这种命名方法，实际使用中还是不要这么做，以免和官方STL冲突。
//在C++17规范中，已经不允许在函数前加下划线前缀，因为这是给STL的内部实现专用的。
//怎么借鉴呢，我自己的方法是，模板参数名不加下划线用原名方便阅读，函数参数名用双下划线的，我改为 h_ ，这个h正好是代表了我们这个项目的名字hcstl

#include "iterator.h"


template <typename RandomAccessIterator, typename Distance, typename T>
void h_push_heap(RandomAccessIterator h_first, Distance hTopIndex, Distance hTargetIndex, T h_value) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮，这是一个更高效的上浮法，有点像下沉。
    //它的做法是一路往上比较，如果发现parent位置元素比value小，就把parent扔到child位置。
    //最终停下来的时候，child位置就是value位置。

    Distance parent = (hTargetIndex - 1) / 2;
    //若已经到达堆顶，不可能接着上浮
    while(hTargetIndex > hTopIndex) {

        //根据先前数学推理：i的孩子结点是，i * 2 + 1和i * 2 + 2
        //反推得i的父节点为 (i + 1) / 2 - 1，其实用（i - 1） / 2就行了，结果是一样的

        //STL 默认的是最大堆，所以这里应该是发现如果其比父节点小，说明位置正确，停止上浮
        if( h_value < *(h_first + parent)  ) {
            break;
        }
        //把元素往下扔
        *(h_first + hTargetIndex) = *(h_first + parent);
        //更新下标
        hTargetIndex = parent;
        parent = (hTargetIndex - 1) / 2;
    }

    //这里就是value的位置
    *(h_first + hTargetIndex) = h_value;

}


template <typename RandomAccessIterator, typename Distance, typename T>
void h_push_heap_aux(RandomAccessIterator h_first, RandomAccessIterator h_last, Distance*, T*) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_push_heap(h_first, Distance(0) , Distance((h_last - h_first) - 1) , T(*(h_last - 1)));
}


/**
 * 一个元素已经放到 heap 容器底部，将这个元素插入到 heap 中正确的位置（上浮），使用 < 进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @param h_first 容器头
 * @param h_last 容器尾
 */
template <typename RandomAccessIterator>
void push_heap(RandomAccessIterator h_first, RandomAccessIterator h_last) {
    h_push_heap_aux(h_first, h_last, hDistanceType(h_first), h_valueType(h_first));
}




template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_push_heap(RandomAccessIterator h_first, Distance hTopIndex, Distance hTargetIndex, T h_value, Compare h_comp) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮

    Distance parent = (hTargetIndex - 1) / 2;
    //若已经到达堆顶，不可能接着上浮
    while(hTargetIndex > hTopIndex) {

        //根据先前数学推理：i的孩子结点是，i * 2 + 1和i * 2 + 2
        //反推得i的父节点为 (i + 1) / 2 - 1，其实用（i - 1） / 2就行了，结果是一样的

        //STL 默认的是最大堆，所以这里应该是发现如果其比父节点小，说明位置正确，停止上浮
        if( h_comp(h_value, *(h_first + parent)) ) {
            break;
        }
        //把元素往下扔
        *(h_first + hTargetIndex) = *(h_first + parent);
        //更新下标
        hTargetIndex = parent;
        parent = (hTargetIndex - 1) / 2;
    }

    //这里就是value的位置
    *(h_first + hTargetIndex) = h_value;
}

template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_push_heap_aux(RandomAccessIterator h_first, RandomAccessIterator h_last, Distance*, T*, Compare h_comp) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_push_heap(h_first, Distance(0) , Distance((h_last - h_first) - 1) , T(*(h_last - 1)), h_comp);
}


/**
 * 一个元素已经放到 heap 容器底部，将这个元素插入到 heap 中正确的位置（上浮），使用给定比较器进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @tparam Compare 比较器类型
 * @param h_first 容器头
 * @param h_last 容器尾
 * @param h_comp 给定的比较器对象
 */
template <typename RandomAccessIterator, typename Compare>
void push_heap(RandomAccessIterator h_first, RandomAccessIterator h_last, Compare h_comp) {
    h_push_heap_aux(h_first, h_last, hDistanceType(h_first), h_valueType(h_first), h_comp);
}


template <typename RandomAccessIterator, typename Distance, typename T>
void h_pop_heap(RandomAccessIterator h_first, Distance hTargetIndex, Distance h_bottomIndex, T h_value) {
    //四项参数分别是，容器开头迭代器，目标（堆顶）元素所在下标，堆底元素所在下标，将被交换到堆顶的堆底元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）

    //首先，将堆顶元素放到堆底，即堆顶元素出堆
    *(h_first + h_bottomIndex) = *(h_first + hTargetIndex);

    //然后往下下沉，找到h_value的应有位置

    Distance max_child;

    //如果已经到达新的堆底（之前堆顶元素已经出堆了，所以不能等于h_bottomIndex），则不能再下沉
    while(hTargetIndex * 2 + 1 < h_bottomIndex) {

        //找出最大的孩子结点，因为左孩子无论如何都存在，所以默认为左孩子
        max_child = hTargetIndex * 2 + 1;
        //如果有右孩子就要比较右孩子
        if(hTargetIndex * 2 + 2 < h_bottomIndex && *(h_first + max_child) < *(h_first + hTargetIndex * 2 + 2) ) {
            max_child = hTargetIndex * 2 + 2;
        }

        //如果最大的孩子都比h_value小，说明这里就是h_value的位置
        if( *(h_first + max_child) < h_value) {
            break;
        }
        //不是h_value的位置，就换最大的孩子到当前target位置上来
        *(h_first + hTargetIndex) = *(h_first + max_child);
        //更新位置到原最大孩子处
        hTargetIndex = max_child;
    }
    //出循环的时候，hTargetIndex就是h_value应在的位置
    *(h_first + hTargetIndex) = h_value;

}


template <typename RandomAccessIterator, typename Distance, typename T>
void h_pop_heap_aux(RandomAccessIterator h_first, RandomAccessIterator h_last, Distance*, T*) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_pop_heap(h_first, Distance(0) , Distance((h_last - h_first) - 1) , T(*(h_last - 1)));
}


/**
 * 将堆顶元素出堆，放到堆底，并对其余元素重新成堆（下沉），使用 < 运算符进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @param h_first 容器头
 * @param h_last 容器尾
 */
template <typename RandomAccessIterator>
void pop_heap(RandomAccessIterator h_first, RandomAccessIterator h_last) {
    h_pop_heap_aux(h_first, h_last, hDistanceType(h_first), h_valueType(h_first));
}




template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_pop_heap(RandomAccessIterator h_first, Distance hTargetIndex, Distance h_bottomIndex, T h_value, Compare h_comp) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮

    //首先，将堆顶元素放到堆底，即堆顶元素出堆
    *(h_first + h_bottomIndex) = *(h_first + hTargetIndex);

    //然后往下下沉，找到h_value的应有位置

    Distance max_child;

    //如果已经到达新的堆底（之前堆顶元素已经出堆了，所以不能等于h_bottomIndex），则不能再下沉
    while(hTargetIndex * 2 + 1 < h_bottomIndex) {

        //找出最大的孩子结点，因为左孩子无论如何都存在，所以默认为左孩子
        max_child = hTargetIndex * 2 + 1;
        //如果有右孩子就要比较右孩子
        if(hTargetIndex * 2 + 2 < h_bottomIndex && *(h_first + max_child) < *(h_first + hTargetIndex * 2 + 2) ) {
            max_child = hTargetIndex * 2 + 2;
        }

        //如果最大的孩子都比h_value小，说明这里就是h_value的位置
        if( h_comp(*(h_first + max_child) , h_value) ) {
            break;
        }
        //不是h_value的位置，就换最大的孩子到当前target位置上来
        *(h_first + hTargetIndex) = *(h_first + max_child);
        //更新位置到原最大孩子处
        hTargetIndex = max_child;
    }
    //出循环的时候，hTargetIndex就是h_value应在的位置
    *(h_first + hTargetIndex) = h_value;
}

template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_pop_heap_aux(RandomAccessIterator h_first, RandomAccessIterator h_last, Distance*, T*, Compare h_comp) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_pop_heap(h_first, Distance(0) , Distance((h_last - h_first) - 1) , T(*(h_last - 1)), h_comp);
}


/**
 * 将堆顶元素出堆，放到堆底，并对其余元素重新成堆（下沉），使用给定比较器进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @tparam Compare 比较器类型
 * @param h_first 容器头
 * @param h_last 容器尾
 * @param h_comp 给定的比较器对象
 */
template <typename RandomAccessIterator, typename Compare>
void pop_heap(RandomAccessIterator h_first, RandomAccessIterator h_last, Compare h_comp) {
    h_pop_heap_aux(h_first, h_last, hDistanceType(h_first), h_valueType(h_first), h_comp);
}

#endif //HCSTL_HEAP_H
