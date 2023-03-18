#include <iostream>


#include "random"
#include "heap.h"
#include "algorithm.h"
#include "vector.h"

#include "deque.h"

//用非POD类型来测试
class myclass{
private:
    int* p;
public:
    myclass() : p(nullptr) {
//        std::cout<<"default constructor  value = null"<<std::endl;
    }
    myclass(int value) {
//        std::cout<<"parameter constructor  value = "<< value <<std::endl;
        p = new int(value);
    }
    myclass(const myclass& m)  {
//        std::cout<<"copy constructor  value = "<< m.getter() <<std::endl;
        this->p = new int(m.getter());
    }
    myclass(myclass&& m)  {
//        std::cout<<"move constructor  value = "<< m.getter() <<std::endl;
        this->p = m.p;
        m.p = nullptr;//表示所有权转移
    }
    ~myclass() {
//        std::cout<<"destructor" <<std::endl;
        delete p;
    }
    myclass& operator= (const myclass& m){
//        std::cout<<"copy assignment  value = "<< m.getter() <<std::endl;
        this->p = new int(m.getter());
        return *this;
    }

    myclass& operator= (myclass&& m){
//        std::cout<<"move assignment  value = "<< m.getter() <<std::endl;
        this->p = m.p;
        m.p = nullptr;//表示所有权转移
        return *this;
    }

    int getter() const { return *p; }
};

int main() {

    deque<myclass>& dq = *(new deque<myclass>());
    for(int i = 1; i < 4; i++) {
        dq.push_front(myclass(i * 100));
    }

    for(int i = 1; i < 5; i++) {
        dq.push_back(myclass(i * 1000));
    }

    std::cout<< "size = " << dq.size() <<std::endl;

    struct Func {
        void operator() (myclass& m){
            std::cout<< m.getter() <<std::endl;
        }
    };
    Func func;
    for_each(dq.begin(), dq.end(), func);

    dq.erase(dq.begin() + 7);
    std::cout<< "size = " << dq.size() <<std::endl;

    for_each(dq.begin(), dq.end(), func);

    dq.erase(dq.begin() + 1, dq.end() - 2);

    for_each(dq.begin(), dq.end(), func);

    std::cout<< "size = " << dq.size() <<std::endl;

    return 0;
}
