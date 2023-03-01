#include <iostream>

#include "vector.h"

//用非POD类型来测试
class myclass{
private:
    int myvalue;
public:
    myclass() : myvalue(0) {}
    myclass(int value) : myvalue(value) {}
    myclass(const myclass& m)  {
        this->myvalue = m.getter();
    }
    ~myclass() {}
    int getter() const { return myvalue; }
};

int main() {
    //以下是测试内容和应有结果

    vector<myclass>& vec = *(new vector<myclass>(4));

    for(int i = 1; i < 10; i++){
        myclass& temp = *(new myclass(i*100));
        vec.push_back(temp);
    }

    std::cout << vec.size() << std::endl;                               //9
    std::cout << vec.capacity() << std::endl;                           //16
    std::cout << vec.front().getter() << std::endl;                     //100
    std::cout << vec.back().getter() << std::endl;                      //900

    vec.erase(vec.begin() + 2);                                 //删去的值是300

    std::cout << vec[2].getter() << std::endl;                          //400
    std::cout << vec.size() << std::endl;                               //8

    vec.erase(vec.begin() + 4, vec.end() - 2);                    //删去的值是600，700两个

    std::cout << vec.size() << std::endl;                               //6
    std::cout << vec[4].getter() << std::endl;                          //800

    return 0;
}
