//
// Created by huangcheng on 2023/3/7.
//

#ifndef HCSTL_FUNCTIONAL_H
#define HCSTL_FUNCTIONAL_H

//这是STL中的常见函数对象

//大于
template <class T>
struct greater{
    bool operator() (const T& x, const T& y) const {
        return x > y;
    }
};

//小于
template <class T>
struct less{
    bool operator() (const T& x, const T& y) const {
        return x < y;
    }
};

#endif //HCSTL_FUNCTIONAL_H
