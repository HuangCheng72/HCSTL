#include <iostream>


#include "random"
#include "heap.h"
#include "algorithm.h"
#include "vector.h"

//用非POD类型来测试
class myclass{
private:
    int* p;
public:
    myclass() : p(nullptr) {}
    myclass(int value) {
        p = new int(value);
    }
    myclass(const myclass& m)  {
        std::cout<<"copy constructor  value = "<< m.getter() <<std::endl;
        this->p = new int(m.getter());
    }
    myclass(myclass&& m)  {
        std::cout<<"move constructor  value = "<< m.getter() <<std::endl;
        this->p = m.p;
        m.p = nullptr;//表示所有权转移
    }
    ~myclass() {
        delete p;
    }
    myclass& operator= (const myclass& m){
        std::cout<<"copy assignment  value = "<< m.getter() <<std::endl;
        this->p = new int(m.getter());
        return *this;
    }

    myclass& operator= (myclass&& m){
        std::cout<<"move assignment  value = "<< m.getter() <<std::endl;
        this->p = m.p;
        m.p = nullptr;//表示所有权转移
        return *this;
    }

    int getter() const { return *p; }
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
//    vector<int>& vec = *(new vector<int>());

    srand(1);
    for(int i = 0; i < 10; i++){
        vec.push_back( myclass(rand()) ) ;
//        vec.push_back( rand() ) ;
    }

    std::cout<<"--------------end stage1-----------------"<<std::endl;

    struct Comp {
        bool operator() (const myclass& m1, const myclass& m2){
            return m1.getter() > m2.getter();
        }
    };
//    struct Comp {
//        bool operator() (const int& m1, const int& m2){
//            return m1 > m2;
//        }
//    };
    Comp& comp = *(new Comp());

    make_heap(vec.begin(), vec.end(), comp);

    std::cout<<"--------------end stage2-----------------"<<std::endl;

    sort_heap(vec.begin(), vec.end(), comp);

    std::cout<<"--------------end stage3-----------------"<<std::endl;

//    struct Func {
//        void operator() (int& m1){
//            std::cout<< m1 << std::endl;
//        }
//    };
//
//    Func& func = *(new Func);


    for_each(vec.begin(), vec.end(), func);

    return 0;
}
