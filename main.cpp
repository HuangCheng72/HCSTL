#include <iostream>


#include "random"
#include "heap.h"
#include "algorithm.h"
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
    ~myclass() {
        myvalue = 0;
    }
    int getter() const { return myvalue; }
};

int main() {

    //以下是测试内容和应有结果

    struct Func {
        void operator() (myclass& m1){
            std::cout<< m1.getter() << std::endl;
        }
    };

    Func& func = *(new Func);

    vector<myclass>& vec = *(new vector<myclass>());

    srand(1);
    for(int i = 0; i < 10; i++){
        vec.push_back(*(new myclass(rand()))) ;
    }

    struct Comp {
        bool operator() (myclass& m1, myclass& m2){
            return m1.getter() > m2.getter();
        }
    };
    Comp& comp = *(new Comp());

    make_heap(vec.begin(), vec.end(), comp);
    sort_heap(vec.begin(), vec.end(), comp);



    for_each(vec.begin(), vec.end(), func);

    return 0;
}
