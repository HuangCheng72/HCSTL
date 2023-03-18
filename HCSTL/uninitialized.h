//
// Created by huangcheng on 2023/3/18.
//

#ifndef HCSTL_UNINITIALIZED_H
#define HCSTL_UNINITIALIZED_H

//初始化内存空间的工具
//uninitialized_copy，把 [first, last) 上的内容复制到以 source 为起始处的空间，返回复制结束的位置
//uninitialized_copy_n，把 [first, first + n) 上的内容复制到以 source 为起始处的空间，返回复制结束的位置
//uninitialized_fill，在 [first, last) 区间内填充元素值
//uninitialized_fill_n，从 first 位置开始，填充 n 个元素值，返回填充结束的位置
//uninitialized_move，把[first, last)上的内容移动到以 source 为起始处的空间，返回移动结束的位置，这是移动语义
//uninitialized_move_n，把[first, first + n)上的内容移动到以 source 为起始处的空间，返回移动结束的位置，移动语义

//STL规范要求这几个工具都是原子性的，也就是说，要么全部成功要么全部不成功，因为这里我们只是demo，我就不讲究这个了，但是如果大家要工业级实现的话，必须要注意这一点

#include "iterator.h"
#include "constructor.h"
#include "util.h"
#include "type_traits.h"


//以特定元素填充空间的话，对于目标空间是输入，只要能写能走一遍就行了，指向源的迭代器，不仅要能走一遍，还得又能读又能写（写是因为有可能你要改变里面的元素）
template <typename InputIterator, typename ForwardIterator>
ForwardIterator h_uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator source) {
    auto cur = source;

    for( ; first != last; first++, cur++) {
        if(hc_type_bool <typename type_traits < typename iterator_traits<ForwardIterator>::value_type >::has_trivial_copy_constructor> ::value){
            *first = *cur;
        } else {
            construct( &(*(first)) ,*cur);
        }
    }
    return cur;
}

/**
 * 拷贝从source开始的数据到未初始化的空间[ first, last )
 * @tparam InputIterator 输入迭代器类型
 * @tparam ForwardIterator 前向迭代器类型
 * @param first 迭代器指向未初始化空间头
 * @param last 迭代器指向未初始化空间尾
 * @param source 迭代器指向数据来源开始位置
 * @return 返回复制结束处位于源数据中的位置（一个迭代器）
 */
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator source) {
    return h_uninitialized_copy(first, last, source);
}


template <typename InputIterator, typename ForwardIterator, typename Size>
ForwardIterator h_uninitialized_copy_n(InputIterator first, Size n, ForwardIterator source) {
    auto cur = source;

    for( ; n > 0; first++, cur++, n--) {
        if(hc_type_bool < typename type_traits < typename iterator_traits<ForwardIterator>::value_type >::has_trivial_copy_constructor> ::value){
            *first = *cur;
        } else {
            construct( &(*(first)) ,*cur);
        }
    }
    return cur;
}

/**
 * 拷贝从source开始的数据到未初始化的空间[ first, first + n )
 * @tparam InputIterator 输入迭代器类型
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam Size 数量类型，一般为size_t或者int
 * @param first 迭代器指向未初始化空间头
 * @param n 复制数量
 * @param source 迭代器指向数据来源开始位置
 * @return 返回复制结束处位于源数据中的位置（一个迭代器）
 */
template <typename InputIterator, typename ForwardIterator, typename Size>
ForwardIterator uninitialized_copy_n(InputIterator first, Size n, ForwardIterator source) {
    return h_uninitialized_copy_n(first, n, source);
}


template <typename ForwardIterator, typename T>
void h_uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
    for( ; first != last ; first++) {
        if(hc_type_bool <typename type_traits< T >::has_trivial_assignment_operator> ::value){
            *first = value;
        } else {
            construct( &(*(first)) , value);
        }
    }
}

/**
 * 在 [first, last) 区间内填充指定的元素值
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam T 元素类型
 * @param first 要填充的开始位置
 * @param last 要填充的结束位置
 * @param value 填充的元素
 */
template <typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& value) {
    h_uninitialized_fill(first, last, value);
}

template <typename ForwardIterator, typename T, typename Size>
void h_uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
    for( ; n > 0; first++, n--) {
        if(hc_type_bool <typename type_traits < T >::has_trivial_assignment_operator> ::value){
            *first = value;
        } else {
            construct( &(*(first)) , value);
        }
    }
}

/**
 * 在 [first, first + n) 区间内填充指定的元素值
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam T 元素类型
 * @tparam Size 数量类型
 * @param first 要填充的开始位置
 * @param n 填充数量
 * @param value 填充的元素
 */
template <typename ForwardIterator, typename T, typename Size>
void uninitialized_fill_n(ForwardIterator first, Size n, const T& value) {
    h_uninitialized_fill_n(first, n, value);
}


template <typename InputIterator, typename ForwardIterator>
ForwardIterator h_uninitialized_move(InputIterator first, InputIterator last, ForwardIterator source) {
    auto cur = source;

    for( ; first != last; first++, cur++) {
        if(hc_type_bool <typename type_traits < typename iterator_traits<ForwardIterator>::value_type >::has_trivial_copy_constructor> ::value){
            *first = *cur;
        } else {
            construct( &(*(first)) , move(*cur) );
        }
    }
    return cur;
}

/**
 * 移动从source开始的数据到未初始化的空间[ first, last )，用的是移动语义
 * @tparam InputIterator 输入迭代器类型
 * @tparam ForwardIterator 前向迭代器类型
 * @param first 迭代器指向未初始化空间头
 * @param last 迭代器指向未初始化空间尾
 * @param source 迭代器指向数据来源开始位置
 * @return 返回复制结束处位于源数据中的位置（一个迭代器）
 */
template <typename InputIterator, typename ForwardIterator>
ForwardIterator uninitialized_move(InputIterator first, InputIterator last, ForwardIterator source) {
    return h_uninitialized_copy(first, last, source);
}


template <typename InputIterator, typename ForwardIterator, typename Size>
ForwardIterator h_uninitialized_move_n(InputIterator first, Size n, ForwardIterator source) {
    auto cur = source;

    for( ; n > 0; first++, cur++, n--) {
        if(hc_type_bool <typename type_traits < typename iterator_traits<ForwardIterator>::value_type >::has_trivial_copy_constructor> ::value){
            *first = *cur;
        } else {
            construct( &(*(first)) , move(*cur) );
        }
    }
    return cur;
}

/**
 * 移动从source开始的数据到未初始化的空间[ first, first + n )，用的是移动语义
 * @tparam InputIterator 输入迭代器类型
 * @tparam ForwardIterator 前向迭代器类型
 * @tparam Size 数量类型，一般为size_t或者int
 * @param first 迭代器指向未初始化空间头
 * @param n 复制数量
 * @param source 迭代器指向数据来源开始位置
 * @return 返回复制结束处位于源数据中的位置（一个迭代器）
 */
template <typename InputIterator, typename ForwardIterator, typename Size>
ForwardIterator uninitialized_move_n(InputIterator first, Size n, ForwardIterator source) {
    return h_uninitialized_move_n(first, n, source);
}

#endif //HCSTL_UNINITIALIZED_H
