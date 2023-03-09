//
// Created by huangcheng on 2023/3/4.
//

#ifndef HCSTL_ITERATOR_H
#define HCSTL_ITERATOR_H

//迭代器的存在，就是为算法操作容器数据提供一个统一的接口。
//让算法不需要面对各种千奇百怪的结构或者操作方法，只需要操作迭代器，就可以读写容器。
//迭代器分为五种，
//输入迭代器（单向移动，只能读一次）（例如istream_iterator）
//输出迭代器（单向移动，只能写一次）（例如ostream_iterator）
//正向迭代器（单向移动，读写多次）（例如单链表的迭代器）（在输入输出两个迭代器基础上扩展）
//双向迭代器（双向移动，读写多次）（例如双向链表的迭代器）（在正向迭代器基础上扩展）
//随机访问迭代器（可以随便移动到哪里，读写多次）（最经典就是指针）（在双向迭代器基础上扩展）
//每个容器均可以通过以上其中一或多种迭代器访问其中数据
//我们希望迭代器能像指针一样，可以通过*来读写数据，可以++，--，或者+ 3 - 4 来移动，还可以通过两个迭代器相减计算两个迭代器之间的距离（最好同样用指针相减的结果ptrdiff_t类型表示，当然你非得用其他也行）等等。
//这就需要重载这些运算符
//但是算法只面对迭代器的时候，算法并不知道迭代器所指向的数据是什么类型的，也不知道这个迭代器本身到底是五种迭代器的哪种（不能让正向迭代器反向移动）。
//因此需要在迭代器中保存这些信息，让算法判断，从而确定这个数据类型能不能执行，这个迭代器能否按照算法的要求移动，等等。

#include "type_traits.h"

// 五种迭代器
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<typename Category, typename T, typename Distance = ptrdiff_t, typename Pointer = T*, typename Reference = T&>
struct iterator {                       //所有迭代器都要保存这些信息，这个结构体相当于迭代器设计规范（原型）
    typedef Category iterator_category; //迭代器的类型，这是为了让使用者（算法）知道能对迭代器用什么操作，比如正向迭代器你就不能让它后退，输入迭代器你就不能让它写数据。
    typedef T value_type;               //迭代器所指向元素的类型，因为C++强类型语言，参数和返回值都需要确定类型，读写数据的时候都要用到这个确定迭代器接受的数据类型和返回的数据类型。
    typedef Distance difference_type;   //迭代器距离的类型，这个确定是为了尽可能像指针，两个迭代器相减确定距离就像指针相减一样。
    typedef Pointer pointer;            //迭代器所指向元素的指针类型，这么做其实就是为了增强可读性
    typedef Reference reference;        //迭代器所指向元素的引用类型，这么做其实就是为了增强可读性
};

//我们需要知道得到了迭代器，需要知道迭代器里面包含的这些信息，该怎么做？
//这时候我们就想起来了类型萃取（type_traits）了，类型萃取是萃取类型的信息，这里我们也是在萃取迭代器的信息。
//只要是符合上面标准的迭代器，都可以被萃取出这些信息。
//其实可以让算法干这件事，但是我们希望算法只干本职工作就是计算逻辑，只要拿到迭代器就能读写，不要管迭代器的内部状态（相当于一个ATM，我只管存钱取钱，不关心你ATM内部运作或者你怎么判断假币）。
template<typename Iterator>
struct iterator_traits{
    typedef typename Iterator::iterator_category iterator_category; //迭代器的类型
    typedef typename Iterator::value_type value_type;               //迭代器所指向元素的类型
    typedef typename Iterator::difference_type difference_type;     //迭代器距离的类型
    typedef typename Iterator::pointer pointer;                     //迭代器所指向元素的指针类型，这么做其实就是为了增强可读性
    typedef typename Iterator::reference reference;                 //迭代器所指向元素的引用类型，这么做其实就是为了增强可读性
};

//针对原生指针这种迭代器的特化版本
template<typename T>
struct iterator_traits<T*>{
    typedef random_access_iterator_tag iterator_category;       //迭代器的类型
    typedef T value_type;                                       //迭代器所指向元素的类型
    typedef ptrdiff_t difference_type;                          //迭代器距离的类型
    typedef T* pointer;                                         //迭代器所指向元素的指针类型，这么做其实就是为了增强可读性
    typedef T& reference;                                       //迭代器所指向元素的引用类型，这么做其实就是为了增强可读性
};
//同样是特化版本，目标是原生指针常量，这个指针指向的内容可变，指针不可变
template<typename T>
struct iterator_traits<const T*>{
    typedef random_access_iterator_tag iterator_category;       //迭代器的类型
    typedef T value_type;                                       //迭代器所指向元素的类型
    typedef ptrdiff_t difference_type;                          //迭代器距离的类型
    typedef T* pointer;                                         //迭代器所指向元素的指针类型，这么做其实就是为了增强可读性
    typedef T& reference;                                       //迭代器所指向元素的引用类型，这么做其实就是为了增强可读性
};

//通过iterator_traits获取各项信息的函数（不要直接用struct，信息太多，尽量少暴露信息，算法用函数，不要直接调结构体）
template<typename Iterator>
typename iterator_traits<Iterator>::iterator_category iterator_category(const Iterator&) {
    //获取迭代器类型
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();//构造一个空结构体传回去，又可以作为参数传入
}

template<typename Iterator>
typename iterator_traits<Iterator>::value_type* value_type(const Iterator&) {
    //获取迭代器指向元素类型
    return (typename iterator_traits<Iterator>::value_type*)0;//传一个该类型的指针，给判断类型用
}

template<typename Iterator>
typename iterator_traits<Iterator>::difference_type * distance_type(const Iterator&) {
    //获取距离类型
    return (typename iterator_traits<Iterator>::difference_type*)(0);//传一个该类型的指针，给判断类型用
}

#endif //HCSTL_ITERATOR_H
