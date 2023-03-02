//
// Created by huangcheng on 2023/3/1.
//

#ifndef HCSTL_VECTOR_H
#define HCSTL_VECTOR_H

//引入模板，将vector的可用范围扩大到全部类型。
//但是这引入了一个问题，POD类型和用户自行建立的复杂数据类型（就是你自己建的类）该怎么处理？
//POD类型（指C++的内建数据类型，还有原生指针和C风格的结构体）不需要我们手动去构造和销毁，而复杂数据类型需要构造和销毁。
//因此需要区分POD类型和复杂数据类型，复杂数据类型使用vector的时候，需要涉及到构造和销毁操作。
//可以模板偏特化所有的POD类型，但是那样代码量太大了，而且不可能以后碰到这种问题全都这样大力出奇迹解决。
//因此最好的做法就是分离一个单独的类型判断模块，提高代码的复用性。
//这就是STL当中类型萃取（type_traits）的意义，萃取各种类型的特点，这里提到的是否需要构造和销毁，就是很重要的类型特点。

#include "type_traits.h"
#include "allocator.h"
#include "constructor.h"

template<typename T, typename Alloc = allocator<T>>         //缺省默认空间配置器为allocator
class vector {
protected:
    T* start;             //表示当前使用空间的头
    T* finish;            //表示当前使用空间的尾
    T* end_of_storage;    //表示当前可用空间的尾

    typedef allocator<T> data_allocator;

    void allocate_and_copy(int newcapacity) {        //这个函数的主要作用就是扩容，newcapacity 应当大于 size()
        //可用空间为0，则需要重新申请更大可用空间，并把元素都复制过去
        T* temp = data_allocator::allocate(newcapacity);
        for(int i = 0; i < size(); i++){
            if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
                //我这里就是用的字面意思，需要用到拷贝构造器我就用has_trivial_copy_constructor，其余同
                temp[i] = start[i];
            } else {
                //在指定空间上构造对象
                construct(temp + i, start[i]);
            }
        }
        end_of_storage = temp + (size() * 2);
        finish = temp + size();

        if(!hc_type_bool<typename _type_traits<T>::is_POD_type>::value){
            //POD类型直接回收空间，如果是复杂数据类型，就先逐个析构完毕，然后再回收空间
            for(int i = 0; i < size(); i++){
                destroy(start + i);
            }
        }
        //通过空间配置器回收空间
        data_allocator::deallocate(start);

        start = temp;
    }

public:
    T* begin() const {
        return start;
    }

    T* end() const {
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

    T& operator[](int n) {
        return *(begin() + n);
    }

    vector() : start(nullptr), finish(nullptr), end_of_storage(nullptr) {}

    vector(int n) {
        start = data_allocator::allocate(n);
        finish = start;
        end_of_storage = start + n;
        for(int i = 0; i < n; i++){
            if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
                start[i] = 0;
            }else{
                construct(start + i);
            }
        }
    }

    vector(int n, const T& value) {
        start = data_allocator::allocate(n);
        finish = start + n;
        end_of_storage = finish;
        for(int i = 0; i < n; i++){
            if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
                start[i] = value;
            }else{
                construct(start + i, value);
            }
        }
    }

    ~vector() {
        delete start;
    }

    T& front() {
        return *begin();
    }

    T& back() {
        return *(end() - 1);
    }

    void push_back(const T& x) {
        if(finish == end_of_storage) {
            allocate_and_copy(capacity() * 2);  //这里采用扩容系数为2
        }
        //直接添加到末尾即可
        if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
            *finish = x;    //在新的finish位置上插入元素
        } else {
            //在新的finish位置上构造新元素
            construct(finish, x);
        }

        ++finish;       //finish自增，指针移动
    }

    void pop_back() {
        //尾端元素删除，指针移动即可
        if(!hc_type_bool<typename _type_traits<T>::has_trivial_destructor>::value){
            //如果需要析构就必须析构
            destroy(finish);
        }
        --finish;
    }

    T* erase(T* position) {     //消除某位置上元素
        //将后面元素往前移动即可
        for(T* i = position; i != finish; i++) {
            if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
                *i = *(i + 1);
            } else {
                //需要先析构，再构造
                destroy(i);
                construct(i, *(i + 1));
            }

        }
        --finish;   //指针向前移动
        return position;
    }

    T* erase(T* a, T* b) {     //消除两个指针之间的元素，注意：a为起点，b为终点
        //将后面元素往前移动即可，把b到finish这一段，复制到a开始的空间这里
        int diff = int(b - a);
        int count = 0; //计数器
        for(T* i = b; i != finish; i++, count++) {
            if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
                *(a + count) = *(a + count + diff);
            } else {
                destroy(a + count);
                construct(a + count, *(a + count + diff));
                destroy(a + count + diff);
            }
        }
        if(!hc_type_bool<typename _type_traits<T>::has_trivial_destructor>::value){
            //需要析构a + count到b这一段
            for(T* i = a + count; i != b; i++) {
                destroy(i);
            }
        }
        finish = finish - diff;   //指针向前移动
        return a;
    }

    void resize(int new_size, const T& x) {
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
                if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
                    *(finish + i) = x;
                } else {
                    construct(finish + i, x);
                }
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
