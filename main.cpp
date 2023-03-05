#include <iostream>

#include "queue.h"
#include "random"

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

    priority_queue<int>& pq = *(new priority_queue<int>());

    //直接上手堆排序

    srand(1);
    for(int i = 10; i >= 1; i--){
        pq.push(rand());
    }

    while(!pq.empty()){
        std::cout<< pq.top() << std::endl;
        pq.pop();
    }

    //随机数种子固定，输出结果如下：
    //41
    //6334
    //11478
    //15724
    //18467
    //19169
    //24464
    //26500
    //26962
    //29358

    return 0;
}
