//
// Created by huangcheng on 2023/3/1.
//

#ifndef HCSTL_VECTOR_H
#define HCSTL_VECTOR_H

//仿照SGI-STL的范式，实现一个只支持double型数据的vector

class vector {
protected:
    double* start;             //表示当前使用空间的头
    double* finish;            //表示当前使用空间的尾
    double* end_of_storage;    //表示当前可用空间的尾

    void allocate_and_copy(int newcapacity) {        //这个函数的主要作用就是扩容，newcapacity 应当大于 size()
        //可用空间为0，则需要重新申请更大可用空间，并把元素都复制过去
        double* temp = new double[newcapacity];
        for(int i = 0; i < size(); i++){
            temp[i] = start[i];
        }
        end_of_storage = temp + (size() * 2);
        finish = temp + size();

        delete start;   //回收内存
        start = temp;
    }

public:
    double* begin() const {
        return start;
    }

    double* end() const {
        return finish;
    }

    //加上const是说明这个函数不能改变任何成员属性和函数
    int size() const {
        return int(finish - start);
    }

    int capacity() const {
        return int(end_of_storage - start);
    }

    bool empty() const {
        return begin() == end();
    }

    double& operator[](int n) {
        return *(begin() + n);
    }

    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}

    vector(int n) {
        start = new double[n];
        finish = start;
        end_of_storage = start + n;
        for(int i = 0; i < n; i++){
            start[i] = 0.0;
        }
    }

    vector(int n, const double& value) {
        start = new double[n];
        finish = start + n;
        end_of_storage = finish;
        for(int i = 0; i < n; i++){
            start[i] = value;
        }
    }

    ~vector() {
        delete start;
    }

    double& front() {
        return *begin();
    }

    double& back() {
        return *(end() - 1);
    }

    void push_back(const double& x) {
        if(finish == end_of_storage) {
            allocate_and_copy(capacity() * 2);  //这里采用扩容系数为2
        }
        //直接添加到末尾即可
        *finish = x;    //在新的finish位置上插入元素
        ++finish;       //finish自增，指针移动
    }

    void pop_back() {
        //尾端元素删除，指针移动即可
        --finish;
    }

    double* erase(double* position) {     //消除某位置上元素
        //将后面元素往前移动即可
        for(double* i = position; i != finish; i++) {
            *i = *(i + 1);
        }
        --finish;   //指针向前移动
        return position;
    }

    double* erase(double* a, double* b) {     //消除两个指针之间的元素，注意：a为起点，b为终点
        //将后面元素往前移动即可，把b到finish这一段，复制到a开始的空间这里
        int diff = int(b - a);
        int count = 0; //计数器
        for(double* i = b; i != finish; i++, count++) {
            *(a + count) = *(a + count + diff);
        }
        finish = finish - diff;   //指针向前移动
        return a;
    }

    void resize(int new_size, const double& x) {
        if(new_size < size()){
            //小于直接删除就行了
            erase(begin() + new_size, end());
        } else {
            //这里不考虑等于的情况，只考虑大于，因而在start + size到start + new_size这一段，插入常数x
            if(new_size > capacity()) {
                //需要扩容才行
                allocate_and_copy(capacity() * 2);  //这里采用扩容系数为2
            }
            //填充参数
            for(int i = 0; i < new_size - size(); i++) {
                *(finish + i) = x;
            }
            //移动指针
            finish += new_size - size();
        }
    }

    void resize(int new_size) {
        resize(new_size, 0);
    }

    void clear() {
        erase(begin(), end());
    }
};

#endif //HCSTL_VECTOR_H
