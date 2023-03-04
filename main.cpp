#include <iostream>

#include "list.h"
#include "vector.h"
#include "algorithm.h"

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

    list<myclass>& l = *(new list<myclass>());
    for(int i = 1; i < 10; i++){
        myclass& temp = *(new myclass(i*100));
        l.push_back(temp);
    }

    struct Func{
        void operator() (myclass& m){
            std::cout << m.getter() << std::endl;
        }
    };

    Func& f = *(new Func());

    for_each(l.begin(), l.end(), f);               //应当输出100到900

    //直接通过迭代器取元素
    list<myclass>::iterator it = l.begin();
    for(int i = 0; i < 5; i++, it++){}
    //i = 5时退出循环，此时it应当指向myvalue为600的对象
    auto a = *it;
    std::cout << a.getter() << std::endl;

    return 0;
}
