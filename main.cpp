#include <iostream>


#include "random"
#include "heap.h"
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
    ~myclass() {
        myvalue = 0;
    }
    int getter() const { return myvalue; }
};

int main() {

    //以下是测试内容和应有结果

    myclass array[10];

    srand(1);
    for(int i = 0; i < 10; i++){
        array[i] = *(new myclass(rand()));
    }

    struct Comp {
        bool operator() (myclass& m1, myclass& m2){
            return m1.getter() < m2.getter();
        }
    };
    Comp& comp = *(new Comp());
    make_heap(array, array + 10, comp);
    sort_heap(array, array + 10, comp);

    struct Func {
        void operator() (myclass& m1){
            std::cout<< m1.getter() << std::endl;
        }
    };

    Func& func = *(new Func);

    for_each(array, array + 10, func);

    return 0;
}
