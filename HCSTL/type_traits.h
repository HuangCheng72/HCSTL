//
// Created by huangcheng on 2023/3/2.
//

#ifndef HCSTL_TYPE_TRAITS_H
#define HCSTL_TYPE_TRAITS_H

//类型萃取
//POD类型（C++的内建数据类型，还有原生指针和C风格的结构体）和复杂数据类型最大的区别是：
//构造（默认构造器）、析构、赋值、复制（拷贝构造器）
//即default_constructor，destructor，assignment_operator，copy_constructor
//复杂数据类型先天缺少这四样（编译器给它加上的先不算），而POD类型先天具备这四样（POD的这四样是编译器完成的，你别管）
//类型萃取这部分使用结构体模板实现，最大的好处C风格结构体也是POD，这样就不用管构造和析构的事情了。

//搜索得到：
//POD（Plain Old Data）数据类型是指能用C的memcpy()等函数进行操作的类、结构体或联合体。POD类型有以下特征：
//没有用户自定义的构造函数、析构函数、拷贝赋值运算符
//没有虚函数和虚基类
//所有非静态成员都是public
//所有非静态成员都是POD类型
//没有继承或只继承了POD类型

//用空结构体而不用bool类型，也是出于节省内存的角度考虑的，因为空结构体不占用内存空间，bool变量在结构体中还占用1字节空间
struct true_type {};
struct false_type {};


template <typename T>
struct hc_type_bool;

//以下两个hc_type_bool的特化，将两个空结构体变为bool变量，可以在if else语句中使用
template<>
struct hc_type_bool<true_type> {//使用空结构体的一大好处就是空结构体可以作为模板参数传递
    static constexpr bool value= true;
};
template<>
struct hc_type_bool<false_type> {
    static constexpr bool value= false;
};


template <typename Tp>
struct type_traits {   //前面加上_（这个符号叫专名号，但是好像很多人叫它下划线）的原因是因为文件名字就叫type_traits，以示区分。
    //has trivial的字面意思是，是否有不重要的，我个人倾向于理解成，是否无需考虑的
    typedef false_type    has_trivial_default_constructor;    //是否无需考虑默认构造器，非POD自然需要考虑，因此false
    typedef false_type    has_trivial_copy_constructor;       //是否无需考虑拷贝构造器，非POD自然需要考虑，因此false
    typedef false_type    has_trivial_assignment_operator;    //是否无需考虑赋值操作符，非POD自然需要考虑，因此false
    typedef false_type    has_trivial_destructor;             //是否无需考虑析构器，非POD自然需要考虑，因此false
    typedef false_type    is_POD_type;                        //不是POD，自然false
};
//似乎只需要一个is_POD_type就可以了，但是STL的规范就是这样，先按照规范来吧
template<>
struct type_traits<char> {
    typedef true_type    has_trivial_default_constructor;      //是否无需考虑默认构造器，POD自然无需考虑，因此true，下同
    typedef true_type    has_trivial_copy_constructor;         //是否无需考虑拷贝构造器，POD自然无需考虑，因此true，下同
    typedef true_type    has_trivial_assignment_operator;      //是否无需考虑赋值操作符，POD自然无需考虑，因此true，下同
    typedef true_type    has_trivial_destructor;               //是否无需考虑析构器，POD自然无需考虑，因此true，下同
    typedef true_type    is_POD_type;                          //POD，自然true，下同
};

template<>
struct type_traits<signed char> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<unsigned char> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<short> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<unsigned short> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<int> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<unsigned int> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<long> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<unsigned long> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<float> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<double> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

template<>
struct type_traits<long double> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

//原生指针也是POD
template<typename T>
struct type_traits<T*> {
    typedef true_type    has_trivial_default_constructor;
    typedef true_type    has_trivial_copy_constructor;
    typedef true_type    has_trivial_assignment_operator;
    typedef true_type    has_trivial_destructor;
    typedef true_type    is_POD_type;
};

#endif //HCSTL_TYPE_TRAITS_H
