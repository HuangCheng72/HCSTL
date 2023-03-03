#include <iostream>

#include "list.h"
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

    /*
    list<myclass>& m_list = *(new list<myclass>());

    for(int i = 1; i < 10; i++){
        myclass& temp = *(new myclass(i*100));
        m_list.push_back(temp);
    }

    std::cout << m_list.size() << std::endl;                               //9
    std::cout << m_list.front().getter() << std::endl;                     //100
    std::cout << m_list.back().getter() << std::endl;                      //900

    m_list.resize(11,*(new myclass(111)));
    std::cout << m_list.size() << std::endl;                               //11
    std::cout << m_list.back().getter() << std::endl;                      //111
    m_list.pop_back();
    m_list.pop_front();
    std::cout << m_list.front().getter() << std::endl;                     //200
    std::cout << m_list.back().getter() << std::endl;                      //111

    m_list.erase(m_list.begin(),m_list.end());
    std::cout << m_list.size() << std::endl;                               //0
    */

    vector<double>& vec = *(new vector<double>(4));
    for(int i = 1; i < 10; i++){
        vec.push_back((double)i / 100);
    }

    vec.for_each(vec.begin() + 2, vec.end() - 3);               //应当输出0.03到0.06

    return 0;
}
