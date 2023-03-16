//
// Created by huangcheng on 2023/3/10.
//

#ifndef HCSTL_UTIL_H
#define HCSTL_UTIL_H

//概念梳理
//右值，简单来说就是没办法用&运算符取地址的值，只能在等号右边。右值只能用于给左值赋值。
//我们经常接触到的右值就是两种，一种是字面量，一种是匿名对象，其他不作详解
//在C++11以前，左值的引用是不能直接绑定右值的，必须将右值赋值给左值，然后左值引用绑定右值。例如
//T* temp = new T();
//T& t = *temp;
//C++11提出了右值引用的概念，可以用右值引用（右值引用本身是个左值，注意）直接绑定右值，例如
//T&& t = T();
//对于T这个类来说，如果T这个类有移动构造器，就会直接调用移动构造器直接完成资源转移，如果没有移动构造器，才会调用拷贝构造器进行拷贝构造。
//对于POD类型，就是直接copy了。
//T& temp = *(new T()); 这句代码之所以能顺利运行的原因是因为我采用了*对匿名对象解引用操作之后，它就变成了一个左值，因为它可以用&运算符取地址了。
//比如T* p =  &( *(new T()) );
//而原本的new T()的结果，是没办法取地址的，因而是右值。

//一般来说，左值引用不能绑定到右值，但有以下几种例外：
//1. 常量左值引用是一个万能的引用类型，它可以绑定到非常量左值、常量左值和右值。
//2. 右值引用可以赋值给左值引用，该左值引用绑定到右值引用指向的对象。
//3. 使用std::move可以将左值转换为将亡值（一种特殊的右值），从而绑定到右值引用。

//引用折叠原则：
//&& && 为 &&
//& && 为 &
//& & 为 &
//&& & 为 &

//移动构造器（move_constructor）是移动构造器是一种特殊的构造函数，它可以从一个临时对象（右值）中转移资源。
//移动构造的好处在于减少了资源的申请和释放，提高了效率，直接把匿名对象资源拿过来就可以使用了。
//移动构造器原型（参数是一个通用引用）：T(T&& t);
//在移动构造器中，可以将通用引用t所绑定的匿名对象的所有资源全部转移到当前对象。

//通用引用也是T&&，它作为参数既可以接受左值引用又可以接受右值引用，当T为具体类型的时候就是右值引用。

//move是为了把所有的引用都转换为右值引用。使用移动构造器的配套。

//引用移除是为了获取原类型，才能转换为右值引用。

//引用移除，使用结构体模板实现
template<typename T>
struct remove_reference{
    typedef T type;
};
//模板特化两种情况
template<typename T>
struct remove_reference<T&>{
    typedef T type;
};
template<typename T>
struct remove_reference<T&&>{
    typedef T type;
};

//move是无条件强制将任何参数转换为右值引用返回。
//为什么参数是T&&，因为这是一个通用引用，它作为参数既可以接受左值引用又可以接受右值引用，也就是全部接受
//T&&可以表示为T&的引用，也就是一个左值引用的引用，同时它又是一个右值引用的意义。

template<typename T>
typename remove_reference<T>::type&& move(T&& t) {
    return ( typename remove_reference<T>::type&& )(t);
}

template<typename T>
void swap(T& a, T& b) {
    //调用移动构造器完成内部资源的交换。
    T temp = move(a);
    a = move(b);
    b = move(temp);
}

template<typename T>
T&& forward( typename remove_reference<T>::type& ref){
    //ref为左值引用的时候，根据引用折叠必定转发为左值引用
    return (T&&)ref;
}

template<typename T>
T&& forward( typename remove_reference<T>::type&& ref){
    //ref为右值引用的时候，根据引用折叠必定转发为右值引用
    return (T&&)ref;
}

#endif //HCSTL_UTIL_H
