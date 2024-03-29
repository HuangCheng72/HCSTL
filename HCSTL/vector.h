//
// Created by huangcheng on 2023/3/1.
//

#ifndef HCSTL_VECTOR_H
#define HCSTL_VECTOR_H

#include "type_traits.h"
#include "allocator.h"
#include "constructor.h"
#include "iterator.h"

template<typename T>
class vector {
public:
    /*-------嵌套类型别名--------*/

    typedef size_t size_type;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;

    typedef pointer iterator;  //vector的iterator用原生指针就可以了，根本不需要封装，直接指定原生指针为vector的iterator

    /*-------嵌套类型别名完--------*/
protected:

    iterator start;             //表示当前使用空间的头
    iterator finish;            //表示当前使用空间的尾
    iterator end_of_storage;    //表示当前可用空间的尾
    typedef allocator<T> data_allocator;   //表示空间配置器

    void allocate_and_copy(size_type newcapacity) {        //这个函数的主要作用就是扩容，newcapacity 应当大于 size()
        //可用空间为0，则需要重新申请更大可用空间，并把元素都复制过去
        iterator temp = data_allocator::allocate(newcapacity);
        for(size_type i = 0; i < size(); i++){
            if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                //我这里就是用的字面意思，需要用到拷贝构造器我就用has_trivial_copy_constructor，其余同
                temp[i] = start[i];
            } else {
                //在指定空间上构造对象
                construct(temp + i, start[i]);
                //析构start + i上的对象
                destroy(start + i);
            }
        }

        //通过空间配置器回收空间
        data_allocator::deallocate(start);

        end_of_storage = temp + (size() * 2);
        finish = temp + size();
        start = temp;
    }

public:

    /*-------迭代器相关函数--------*/

    iterator begin() const {
        return start;
    }

    iterator end() const {
        return finish;
    }

    /*-------迭代器相关函数完--------*/

    /*-------构造器与析构器相关函数--------*/

    vector() : vector(16) {} //默认构造器调用带参构造器必须使用委托构造语法，默认初始容量为16

    vector(size_type n) {
        start = data_allocator::allocate(n);
        finish = start;
        end_of_storage = start + n;
        for(size_type i = 0; i < n; i++){
            if(hc_type_bool<typename type_traits<T>::has_trivial_default_constructor>::value){
                start[i] = 0;
            }else{
                construct(start + i);
            }
        }
    }

    vector(size_type n, const_reference value) {
        start = data_allocator::allocate(n);
        finish = start + n;
        end_of_storage = finish;
        for(size_type i = 0; i < n; i++){
            if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                start[i] = value;
            }else{
                construct(start + i, value);
            }
        }
    }

    vector(const vector<T>& vec) {
        //先把之前的空间删掉
        for(size_type i = 0; i < size(); i++){
            if(!hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                //如果需要就析构start + i上的对象
                destroy(start + i);
            }
        }
        //通过空间配置器回收空间
        data_allocator::deallocate(start);

        //再申请空间
        start = data_allocator::allocate(vec.capacity());
        finish = start + vec.size();
        end_of_storage = start + vec.capacity();
        for(size_type i = 0; i < vec.size(); i++){
            if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                //这里不能用vec[i]的原因是operator[]我没给它加上const，而这里的vec是一个常量引用，只能调动在参数列表后面加了const的保证不会修改内部的函数
                start[i] = *(vec.begin() + i);
            } else {
                //在指定空间上构造对象
                //不能用vec[i]的理由同上
                construct(start + i, *(vec.begin() + i));
            }
        }
    }

    vector(vector<T>&& vec) {
        //先把之前的空间删掉
        for(size_type i = 0; i < size(); i++){
            if(!hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                //如果需要就析构start + i上的对象
                destroy(start + i);
            }
        }
        //通过空间配置器回收空间
        data_allocator::deallocate(start);

        //然后直接搬运
        start = vec.start;
        finish = vec.finish;
        end_of_storage = vec.end_of_storage;
        vec.start = nullptr;
        vec.finish = nullptr;
        vec.end_of_storage = nullptr;
    }

    vector& operator= (const vector<T>& vec){
        //先把之前的空间删掉
        for(size_type i = 0; i < size(); i++){
            if(!hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                //如果需要就析构start + i上的对象
                destroy(start + i);
            }
        }
        //通过空间配置器回收空间
        data_allocator::deallocate(start);

        //再申请空间
        start = data_allocator::allocate(vec.capacity());
        finish = start + vec.size();
        end_of_storage = start + vec.capacity();
        for(size_type i = 0; i < vec.size(); i++){
            if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                //这里不能用vec[i]的原因是operator[]我没给它加上const，而这里的vec是一个常量引用，只能调动在参数列表后面加了const的保证不会修改内部的函数
                start[i] = *(vec.begin() + i);
            } else {
                //在指定空间上构造对象
                //不能用vec[i]的理由同上
                construct(start + i, *(vec.begin() + i));
            }
        }
    }

    vector& operator= (vector<T>&& vec) {
        //先把之前的空间删掉
        for(size_type i = 0; i < size(); i++){
            if(!hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                //如果需要就析构start + i上的对象
                destroy(start + i);
            }
        }
        //通过空间配置器回收空间
        data_allocator::deallocate(start);

        //然后直接搬运
        start = vec.start;
        finish = vec.finish;
        end_of_storage = vec.end_of_storage;
        vec.start = nullptr;
        vec.finish = nullptr;
        vec.end_of_storage = nullptr;
    }

    ~vector() {
        clear();
        data_allocator::deallocate(start);
    }

    /*-------构造器与析构器相关函数完--------*/

    /*-------容器容量相关函数--------*/

    size_type size() const {
        return size_type(finish - start);
    }

    size_type capacity() const {
        return size_type(end_of_storage - start);
    }

    bool empty() const {
        return begin() == end();
    }

    /*-------容器容量相关函数完--------*/

    /*-------容器数据相关函数--------*/

    reference front() {
        return *begin();
    }

    reference back() {
        return *(end() - 1);
    }

    reference operator[](size_type n) {
        return *(begin() + n);
    }

    void push_back(const_reference x) {
        if(finish == end_of_storage) {
            allocate_and_copy(capacity() * 2);  //这里采用扩容系数为2
        }
        //直接添加到末尾即可
        if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
            *finish = x;    //在新的finish位置上插入元素
        } else {
            //在新的finish位置上构造新元素
            construct(finish, x);
        }

        ++finish;       //finish自增，指针移动
    }

    void push_back(value_type&& x) {
        if(finish == end_of_storage) {
            allocate_and_copy(capacity() * 2);  //这里采用扩容系数为2
        }
        //直接添加到末尾即可
        if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
            *finish = x;    //在新的finish位置上插入元素
        } else {
            //在新的finish位置上构造新元素
            construct(finish, move(x));
        }
        ++finish;       //finish自增，指针移动
    }

    void pop_back() {
        //尾端元素删除，指针移动即可
        if(!hc_type_bool<typename type_traits<T>::has_trivial_destructor>::value){
            //如果需要析构就必须析构
            destroy(finish);
        }
        --finish;
    }

    iterator erase(iterator position) {     //消除某位置上元素
        //将后面元素往前移动即可
        for(iterator i = position; i != finish; i++) {
            if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
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

    iterator erase(iterator first, iterator last) {
        //将后面元素往前移动即可，把last到finish这一段，复制到first开始的空间这里
        ptrdiff_t diff = last - first;
        size_type count = 0; //计数器
        for(iterator i = last; i != finish; i++, count++) {
            if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                *(first + count) = *(first + count + diff);
            } else {
                destroy(first + count);
                construct(first + count, *(first + count + diff));
                destroy(first + count + diff);
            }
        }
        if(!hc_type_bool<typename type_traits<T>::has_trivial_destructor>::value){
            //需要析构first + count到last这一段
            for(iterator i = first + count; i != last; i++) {
                destroy(i);
            }
        }
        finish = finish - diff;   //指针向前移动
        return first;
    }

    void resize(size_type new_size, const_reference x) {
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
            for(size_type i = 0; i < new_size - size(); i++) {
                if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                    *(finish + i) = x;
                } else {
                    construct(finish + i, x);
                }
            }
            //移动指针
            finish += new_size - size();
        }
    }

    void resize(size_type new_size) {
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
            for(size_type i = 0; i < new_size - size(); i++) {
                if(hc_type_bool<typename type_traits<T>::has_trivial_copy_constructor>::value){
                    *(finish + i) = 0;
                } else {
                    construct(finish + i);
                }
            }
            //移动指针
            finish += new_size - size();
        }
    }

    void clear() {
        erase(begin(), end());
    }

    /*-------容器数据相关函数完--------*/

};

#endif //HCSTL_VECTOR_H
