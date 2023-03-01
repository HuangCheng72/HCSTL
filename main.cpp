#include <iostream>

#include "vector.h"

int main() {
    //以下是测试内容和应有结果

    vector& vec = *(new vector(4));
    for(int i = 1; i < 10; i++){
        vec.push_back((double)i / 100);
    }

    std::cout << vec.size() << std::endl;                   //9
    std::cout << vec.capacity() << std::endl;               //16
    std::cout << vec.front() << std::endl;                  //0.01
    std::cout << vec.back() << std::endl;                   //0.09

    vec.erase(vec.begin() + 2);                     //删去的值是0.03

    std::cout << vec[2] << std::endl;                       //0.04
    std::cout << vec.size() << std::endl;                   //8

    vec.erase(vec.begin() + 4, vec.end() - 2);        //删去的值是0.06，0.07两个

    std::cout << vec.size() << std::endl;                   //6
    std::cout << vec[4] << std::endl;                       //0.08

    return 0;
}
