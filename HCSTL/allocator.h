//
// Created by huangcheng on 2023/3/2.
//

#ifndef HCSTL_ALLOCATOR_H
#define HCSTL_ALLOCATOR_H

//先前在vector的allocate_and_copy函数中，POD类型可以delete关键字直接回收数组空间，但是复杂类型的时候不行，造成浪费。
//建立空间配置器的目的，最终是为了解决内存分配的低效和浪费问题，目前没那么远，先把回收空间的问题解决了。
//一般的new关键字，创建一个对象的时候是进行了两步操作，一是申请这个对象所需要大小（用sizeof）的内存空间，二是在这个内存空间上构造对象，然后返回这个内存空间的指针。
//空间配置器就是要把这两个部分分开，分别操作，申请和释放就只管空间的事情，类似于C语言的malloc和free，不管空间上面有什么。

//这里先实现一个最简单的能解决当前回收空间问题的空间配置器就行了，这里就参考HP-STL的空间配置器实现。

#include <new>
#include <iostream>

//修改size的类型为size_t，能够管理（目前只有分配和回收）更大的内存，int类型相对来说太小了。


template<typename T>
T* allocate(size_t size, T*) {         //这么定参数列表的原因，size自然是指定大小，给T*作为参数，是为了预防需要void*这种类型的出现
    std::set_new_handler(nullptr);  //这个命令的意思本来是，设置当申请失败时的处理函数，这里设置为nullptr就是让直接抛异常
    //::operator new，全局new，是对C语言malloc的封装，用法一样，返回的也是void*
    //::operator delete，全局delete，是对C语言free的封装，用法一样
    //在前面加上::，又不加上命名空间，说明作用域是全局
    T* temp = (T*)(::operator new( size_t(size * sizeof(T)) ));
    if(temp == 0){
        //申请失败的时候直接报错
        std::cerr<<"Failed to allocate memory."<<std::endl;
        exit(1);
    }
    return temp;
}

template<typename T>
void deallocate(T* buffer){
    ::operator delete(buffer);
}


template<typename T>
class allocator{
public:

    typedef size_t size_type;

    static T* allocate(size_type n) {
        //在前面加上::，又不加上命名空间，说明作用域是全局
        //删了::就调用不了前面的allocate了
        return ::allocate(n, (T*)0);
    }

    static void deallocate(T* p) {
        //在前面加上::，又不加上命名空间，说明作用域是全局
        //删了::就调用不了前面的deallocate了
        return ::deallocate(p);
    }
};

#endif //HCSTL_ALLOCATOR_H
