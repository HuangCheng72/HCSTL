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
    myclass() : p(nullptr) {
        std::cout<<"default constructor  value = null"<<std::endl;
    }
    myclass(int value) {
        std::cout<<"parameter constructor  value = "<< value <<std::endl;
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
        std::cout<<"destructor" <<std::endl;
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

    vector<myclass>& vec1 = *(new vector<myclass>(4));
    vector<myclass>& vec2 = *(new vector<myclass>(4));

    std::cout<< "-----------stage1-split--------------" << std::endl;

    for(int i = 1; i < 4; i++) {
        myclass m_temp = *(new myclass(i * 100));
        vec1.push_back(m_temp);
    }

    std::cout<< "-----------stage2-split--------------" << std::endl;

    for(int i = 1; i < 4; i++) {
        vec2.push_back(myclass(i * 100 + 1000));
    }

    vector<myclass> vec3(vec2);
    vector<myclass> vec4(move(vec2));
    vector<myclass> vec5 = vec3;
    vector<myclass> vec6 = move(vec4);

    std::cout<< vec3.size() <<std::endl;
    std::cout<< vec4.size() <<std::endl;
    std::cout<< vec5.size() <<std::endl;
    std::cout<< vec6.size() <<std::endl;

    std::cout<< "-----------stage3-split--------------" << std::endl;

    return 0;
}
