#include <iostream>


#include "random"
#include "heap.h"
#include "algorithm.h"
#include "vector.h"
#include "queue.h"

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

    priority_queue<int>& pq = *(new priority_queue<int>());

    srand(1);
    for(int i = 0; i < 10; i++) {
        int temp = rand();
        pq.push(temp);
    }

    while(!pq.empty()){
        std::cout << pq.top() << std::endl;
        pq.pop();
    }

    delete &pq;

    return 0;
}
