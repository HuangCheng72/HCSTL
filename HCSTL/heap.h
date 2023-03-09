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

//算了，还是别搞那么多幺蛾子了，参数名直接用原名，类型名也是直接用原型，只有实在改不掉的双下划线前缀的函数名，改为h_前缀。
//以前这样做怕重复是因为没有命名空间。
//现在都有命名空间了，用namespace hcstl一包起来，根本不怕重名问题。

#include "iterator.h"


template <typename RandomAccessIterator, typename Distance, typename T>
void h_push_heap(RandomAccessIterator first, Distance topIndex, Distance targetIndex, T value) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮，这是一个更高效的上浮法，有点像下沉。
    //它的做法是一路往上比较，如果发现parent位置元素比value小，就把parent扔到child位置。
    //最终停下来的时候，child位置就是value位置。

    Distance parent = (targetIndex - 1) / 2;
    //若已经到达堆顶，不可能接着上浮
    while(targetIndex > topIndex) {

        //根据先前数学推理：i的孩子结点是，i * 2 + 1和i * 2 + 2
        //反推得i的父节点为 (i + 1) / 2 - 1，其实用（i - 1） / 2就行了，结果是一样的

        //STL 默认的是最大堆，所以这里应该是发现如果其比父节点小，说明位置正确，停止上浮
        if( value < *(first + parent)  ) {
            break;
        }
        //把元素往下扔
        *(first + targetIndex) = *(first + parent);
        //更新下标
        targetIndex = parent;
        parent = (targetIndex - 1) / 2;
    }

    //这里就是value的位置
    *(first + targetIndex) = value;

}


template <typename RandomAccessIterator, typename Distance, typename T>
void h_push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_push_heap(first, Distance(0) , Distance((last - first) - 1) , T(*(last - 1)));
}


/**
 * 一个元素已经放到 heap 容器底部，将这个元素插入到 heap 中正确的位置（上浮），使用 < 进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @param first 容器头
 * @param last 容器尾
 */
template <typename RandomAccessIterator>
void push_heap(RandomAccessIterator first, RandomAccessIterator last) {
    h_push_heap_aux(first, last, distance_type(first), value_type(first));
}




template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_push_heap(RandomAccessIterator first, Distance topIndex, Distance targetIndex, T value, Compare comp) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮

    Distance parent = (targetIndex - 1) / 2;
    //若已经到达堆顶，不可能接着上浮
    while(targetIndex > topIndex) {

        //根据先前数学推理：i的孩子结点是，i * 2 + 1和i * 2 + 2
        //反推得i的父节点为 (i + 1) / 2 - 1，其实用（i - 1） / 2就行了，结果是一样的

        //STL 默认的是最大堆，所以这里应该是发现如果其比父节点小，说明位置正确，停止上浮
        if( comp(value, *(first + parent)) ) {
            break;
        }
        //把元素往下扔
        *(first + targetIndex) = *(first + parent);
        //更新下标
        targetIndex = parent;
        parent = (targetIndex - 1) / 2;
    }

    //这里就是value的位置
    *(first + targetIndex) = value;
}

template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_push_heap(first, Distance(0) , Distance((last - first) - 1) , T(*(last - 1)), comp);
}


/**
 * 一个元素已经放到 heap 容器底部，将这个元素插入到 heap 中正确的位置（上浮），使用给定比较器进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @tparam Compare 比较器类型
 * @param first 容器头
 * @param last 容器尾
 * @param comp 给定的比较器对象
 */
template <typename RandomAccessIterator, typename Compare>
void push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    h_push_heap_aux(first, last, distance_type(first), value_type(first), comp);
}


template <typename RandomAccessIterator, typename Distance, typename T>
void h_pop_heap(RandomAccessIterator first, Distance targetIndex, Distance bottomIndex, T value) {
    //四项参数分别是，容器开头迭代器，目标（堆顶）元素所在下标，堆底元素所在下标，将被交换到堆顶的堆底元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）

    //首先，将堆顶元素放到堆底，即堆顶元素出堆
    *(first + bottomIndex) = *(first + targetIndex);

    //然后往下下沉，找到value的应有位置

    Distance max_child;

    //如果已经到达新的堆底（之前堆顶元素已经出堆了，所以不能等于bottomIndex），则不能再下沉
    while(targetIndex * 2 + 1 < bottomIndex) {

        //找出最大的孩子结点，因为左孩子无论如何都存在，所以默认为左孩子
        max_child = targetIndex * 2 + 1;
        //如果有右孩子就要比较右孩子
        if(targetIndex * 2 + 2 < bottomIndex && *(first + max_child) < *(first + targetIndex * 2 + 2) ) {
            max_child = targetIndex * 2 + 2;
        }

        //如果最大的孩子都比value小，说明这里就是value的位置
        if( *(first + max_child) < value) {
            break;
        }
        //不是value的位置，就换最大的孩子到当前target位置上来
        *(first + targetIndex) = *(first + max_child);
        //更新位置到原最大孩子处
        targetIndex = max_child;
    }
    //出循环的时候，targetIndex就是value应在的位置
    *(first + targetIndex) = value;

}


template <typename RandomAccessIterator, typename Distance, typename T>
void h_pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_pop_heap(first, Distance(0) , Distance((last - first) - 1) , T(*(last - 1)));
}


/**
 * 将堆顶元素出堆，放到堆底，并对其余元素重新成堆（下沉），使用 < 运算符进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @param first 容器头
 * @param last 容器尾
 */
template <typename RandomAccessIterator>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last) {
    h_pop_heap_aux(first, last, distance_type(first), value_type(first));
}




template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_pop_heap(RandomAccessIterator first, Distance targetIndex, Distance bottomIndex, T value, Compare comp) {
    //四项参数分别是，容器开头迭代器，堆顶元素所在下标，目标元素所在下标，要插入的元素（比较的要素齐全，不需要创建一个变量临时存储了，节约内存提高效率）
    //实现元素上浮

    //首先，将堆顶元素放到堆底，即堆顶元素出堆
    *(first + bottomIndex) = *(first + targetIndex);

    //然后往下下沉，找到value的应有位置

    Distance max_child;

    //如果已经到达新的堆底（之前堆顶元素已经出堆了，所以不能等于bottomIndex），则不能再下沉
    while(targetIndex * 2 + 1 < bottomIndex) {

        //找出最大的孩子结点，因为左孩子无论如何都存在，所以默认为左孩子
        max_child = targetIndex * 2 + 1;
        //如果有右孩子就要比较右孩子
        if(targetIndex * 2 + 2 < bottomIndex && *(first + max_child) < *(first + targetIndex * 2 + 2) ) {
            max_child = targetIndex * 2 + 2;
        }

        //如果最大的孩子都比value小，说明这里就是value的位置
        if( comp(*(first + max_child) , value) ) {
            break;
        }
        //不是value的位置，就换最大的孩子到当前target位置上来
        *(first + targetIndex) = *(first + max_child);
        //更新位置到原最大孩子处
        targetIndex = max_child;
    }
    //出循环的时候，targetIndex就是value应在的位置
    *(first + targetIndex) = value;
}

template <typename RandomAccessIterator, typename Distance, typename T, typename Compare>
void h_pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*, Compare comp) {
    //传入Distance*和T*的指针是为了模板参数的完成自动推导
    h_pop_heap(first, Distance(0) , Distance((last - first) - 1) , T(*(last - 1)), comp);
}


/**
 * 将堆顶元素出堆，放到堆底，并对其余元素重新成堆（下沉），使用给定比较器进行比较
 * @tparam RandomAccessIterator 对任何随机访问迭代器适用
 * @tparam Compare 比较器类型
 * @param first 容器头
 * @param last 容器尾
 * @param comp 给定的比较器对象
 */
template <typename RandomAccessIterator, typename Compare>
void pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp) {
    h_pop_heap_aux(first, last, distance_type(first), value_type(first), comp);
}

#endif //HCSTL_HEAP_H
