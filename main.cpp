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

    vector<myclass>& vec = *(new vector<myclass>(4));
    for(int i = 1; i < 10; i++){
        myclass& temp = *(new myclass(i*100));
        vec.push_back(temp);
    }

    struct Func{
        void operator() (myclass& m){
            std::cout << m.getter() << std::endl;
        }
    };

    Func& f = *(new Func());

    vec.for_each(vec.begin() + 2, vec.end() - 3, f);               //应当输出300到600

    return 0;
}
