//
// Created by huangcheng on 2023/3/4.
//

#ifndef HCSTL_ALGORITHM_H
#define HCSTL_ALGORITHM_H


//将通用的算法（计算逻辑）独立出来，提高代码的复用性

template<typename Input_Iterator, typename Function>
void for_each(Input_Iterator first, Input_Iterator last, Function f) {
    for(Input_Iterator temp = first; temp != last; temp++) {
        f(*temp);
    }
}


#endif //HCSTL_ALGORITHM_H
