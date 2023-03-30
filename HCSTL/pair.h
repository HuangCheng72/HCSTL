//
// Created by huangcheng on 2023/3/30.
//

#ifndef HCSTL_PAIR_H
#define HCSTL_PAIR_H

//主要是pair（二元组）这种数据结构的定义以及运算，并且基于pair实现键值对（Key-Value）

#include "util.h"

//要求传入的两种类型都是POD数据类型或者重载了==，<，>三个操作符的复杂数据类型，才能使用pair的所有操作符
template <typename Ty1, typename Ty2>
struct pair {
    typedef Ty1	first_type;
    typedef Ty2	second_type;

    first_type first;
    second_type second;

    //默认构造器
    pair() :first(first_type()), second(second_type()) {}
    //带参构造器
    pair(const first_type& a, const second_type& b) :first(a), second(b) {}
    //拷贝构造器
    template<typename T1, typename T2>
    pair(const pair<T1, T2>& p) : first(p.first), second(p.second) {}
    //移动构造器
    template<typename T1, typename T2>
    pair(pair<T1, T2>&& p) : first(move(p.first)), second(move(p.second)) {}

    //重载pair类型的各个操作符
    //包括：
    //1.相等 ==
    //2.不等 !=
    //3.小于 <
    //4.大于 >
    //5.小于等于 <=
    //6.大于等于 >=
    bool operator== (const pair<first_type, second_type>& p) {
        //first和second都要相同才可以
        return first == p.first && second == p.second;
    }

    bool operator!= (const pair<first_type, second_type>& p) {
        //直接判断是否相等，取反就行
        return !(*this == p);
    }
    
    bool operator< (const pair<first_type, second_type>& p) {
        //如果this的first小于p的first，直接返回true
        //如果this的first大于等于p的first，再比较second
        return first < p.first || (!(p.first < first) && second < p.second);
    }
    
    bool operator> (const pair<first_type, second_type>& p) {
        //对调this和p的位置即可
        return p.first < first || (!(first < p.first) && p.second < second);
    }
    
    bool operator<= (const pair<first_type, second_type>& p) {
        //如果p < this为true，说明this >= p
        return !(p < *this);
    }
    
    bool operator>= (const pair<first_type, second_type>& p) {
        //同理如果this < p为true，说明p >= this
        return !(*this < p);
    }

};

/**
 * 将数据a和b组合为一个pair并返回，其中a为first，b为second
 * @tparam T1 a的类型
 * @tparam T2 b的类型
 * @param a 数据
 * @param b 数据
 * @return 返回一个first为a，second为b的pair
 */
template <class T1, class T2>
pair<T1, T2> make_pair(const T1& a, const T2& b) {
    return pair<T1, T2>(a, b);
}

#endif //HCSTL_PAIR_H
