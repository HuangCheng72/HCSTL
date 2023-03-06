//
// Created by huangcheng on 2023/3/2.
//

#ifndef HCSTL_LIST_H
#define HCSTL_LIST_H

//STL的list是一个环形双向链表
#include "type_traits.h"
#include "allocator.h"
#include "constructor.h"
#include "iterator.h"

template <typename T>
struct __list_node {
    __list_node* prev;    //指向上一结点的指针
    __list_node* next;    //指向下一结点的指针
    T value;            //数据
};

template<typename T>
struct list_iterator : public iterator<bidirectional_iterator_tag, T>{  //继承迭代器原型，这样就能通过iterator_traits来萃取特性了。

    //迭代器需要用到的类型起别名，这是为了代码的阅读方便，和之前为了迭代器能够特性萃取的不是一回事。
    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef __list_node<T> list_node;
    typedef list_node* ListNode;
    typedef list_iterator<T> self;

    //存储当前所处的结点
    ListNode cur;


    //构造器
    list_iterator() : cur(nullptr) {}

    list_iterator(const ListNode& p) : cur(p) {}

    list_iterator(const list_iterator& li) : cur(li.cur) {}

    //重载指针的运算符，让这个迭代器能够起到部分指针的作用
    //这是一个双向迭代器，能够实现：
    //*取值
    //->，通过指针访问成员（变量或者函数）的地址
    //++，自增
    //--，自减
    //==，判断是否等于
    // !=，判断是否不等于

    reference operator*()  const {
        return cur->value;
    }
    pointer operator->() const {
        return &(cur->value);
    }
    self& operator++() {    //++it
        cur = cur->next;
        return *this;
    }
    self operator++(int) {   //it++
        self temp = *this;
        cur = cur->next;
        //其实可以直接++(*this); ， 这样子代码复用性更好，如果迭代器++操作很复杂的话，就不要在这里再实现一次了，直接复用
        return temp;
    }
    self& operator--() {
        cur = cur->prev;
        return *this;
    }
    self operator--(int) {
        self temp = *this;
        cur = cur->prev;
        return temp;
    }
    bool operator==(const self& li) const {
        return cur == li.cur;
    }
    bool operator!=(const self& li) const {
        return cur != li.cur;
    }

};

template <typename T>
class list {
public:
    /*-------嵌套类型别名--------*/
    //在这里是为了代码的可读性
    typedef size_t size_type;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;

    typedef list_iterator<T> iterator;  //确定list_iterator是这个容器的迭代器类型

    /*-------嵌套类型别名完--------*/
protected:
    //这里起别名也是为了代码可读性
    typedef __list_node<T> list_node;
    typedef list_node* ListNode;

    ListNode node;     //只需一个结点即可表示整个环形双向链表

    typedef allocator<__list_node<T>> data_allocator;   //对结点的空间配置器

    ListNode m_CreatNode(){    //创建一个结点，这个结点不带有意义的数据
        ListNode res = data_allocator::allocate(1);
        if(hc_type_bool<typename _type_traits<T>::has_trivial_default_constructor>::value){
            //是POD类型无需构造，直接填充数据0
            res->value = 0;
        } else {
            //含有non-POD类型的结构体，也是non-POD类型，应当调用默认构造器构造
            construct(res);
            //non-POD类型需要调用默认构造器
            construct(&(res->value));
        }
        return res;
    }
    ListNode m_CreatNode(const_reference value){    //创建一个结点，这个结点的数据拷贝自value
        ListNode res = data_allocator::allocate(1);
        if(hc_type_bool<typename _type_traits<T>::has_trivial_copy_constructor>::value){
            //是POD类型无需构造，直接填充数据
            res->value = value;
        } else {
            //含有non-POD类型的结构体，也是non-POD类型，应当调用默认构造器构造
            construct(res);
            //non-POD类型需要调用拷贝构造器
            construct(&(res->value),value);
        }
        return res;
    }
    void m_DestroyNode(ListNode& n){    //销毁一个结点
        if(!hc_type_bool<typename _type_traits<T>::has_trivial_destructor>::value){
            //non-POD类型需要调用析构器
            destroy(&(n->value));//析构数据;
        }
        destroy(n);//析构结点
        data_allocator::deallocate(n);//回收空间
    }
public:

    //提示，内部实现如果不好操作的话，不用迭代器是没问题的，本来迭代器就不是必须的，可以直接微操指针

    /*-------迭代器相关函数--------*/

    iterator begin() {
        return iterator(node->next);
    }

    iterator end() {
        return iterator(node);
    }

    /*-------迭代器相关函数完--------*/

    /*-------构造器与析构器相关函数--------*/

    list() {
        //创建一个空链表
        node = m_CreatNode();
        node->next = node;
        node->prev = node;
    }

    ~list(){
        clear();
        m_DestroyNode(node);
    }

    /*-------构造器与析构器相关函数完--------*/

    /*-------容器容量相关函数--------*/

    bool empty() const {
        return node->next == node;
    }

    size_type size() const {
        size_type count = 0;
        //内部实现的话，可以不用迭代器
        for(ListNode temp = node->next; temp != node; temp = temp->next, count++){}
        return count;
    }

    void resize (size_type n){
        if(n == 0){
            //特殊情况
            clear();
        }
        //多余删除，缺少就添加，默认构造
        size_type count = size();
        ListNode _end = node;
        if(n < count){
            //多余要删除
            for(size_type i = n; i > count; --i){
                ListNode temp = _end->prev;
                _end->prev = temp->prev;
                temp->prev->next = _end;//从链表中删除temp这个结点
                m_DestroyNode(temp);
            }
        }else{
            //缺少要添加
            for(size_type i = count; i < n; ++i){
                ListNode temp = m_CreatNode();
                temp->prev = _end->prev;
                temp->next = _end;
                temp->prev->next = temp;
                temp->next->prev = temp;
            }
        }
    }

    void resize (size_type n, const_reference val){
        if(n == 0){
            //特殊情况
            clear();
        }
        //多余删除，缺少添加，拷贝构造
        size_type count = size();
        ListNode _end = node;
        if(n < count){
            //多余要删除
            for(size_type i = n; i > count; --i){
                ListNode temp = _end->prev;
                _end->prev = temp->prev;
                temp->prev->next = _end;//从链表中删除temp这个结点
                m_DestroyNode(temp);
            }
        }else{
            //缺少要添加
            for(size_type i = count; i < n; ++i){
                ListNode temp = m_CreatNode(val);
                temp->prev = _end->prev;
                temp->next = _end;
                temp->prev->next = temp;
                temp->next->prev = temp;
            }
        }
    }

    /*-------容器容量相关函数完--------*/

    /*-------容器数据相关函数--------*/

    reference front() const{
        return node->next->value;
    }

    reference back() {
        return node->prev->value;
    }

    void push_back (const_reference val) {
        //拷贝构造
        ListNode temp = m_CreatNode(val);
        temp->prev = node->prev;
        temp->next = node;
        temp->prev->next = temp;
        temp->next->prev = temp;
    }

    void push_front(const_reference val) {
        //拷贝构造
        ListNode temp = m_CreatNode(val);
        temp->next = node->next;
        temp->next->prev = temp;
        node->next = temp;
        temp->prev = node;
    }

    void pop_back(){
        ListNode temp = node->prev;
        node->prev = temp->prev;
        temp->prev->next = node;//从链表中删除temp这个结点
        m_DestroyNode(temp);
    }

    void pop_front(){
        ListNode temp = node->next;//要删除temp
        node->next = temp->next;
        temp->next->prev = node;
        m_DestroyNode(temp);
    }

    iterator erase(iterator position) {
        ListNode temp = position.cur->next;
        position.cur->prev->next = position.cur->next;
        position.cur->next->prev = position.cur->prev;//从链表中删除position这个结点
        m_DestroyNode(*(position.cur));
        return iterator(temp);//现在在原来Position位置上的结点的迭代器
    }

    iterator erase(iterator first, iterator last) {
        //直接把first->prev和last相连接，析构中间的元素
        ListNode res = first.cur->prev;
        res->next = last.cur;
        last.cur->prev = res;
        //迭代器是包装了一个ListNode类型的指针cur的结构体，而且没有重载直接与ListNode比较和赋值的操作，所以，必须要拿cur出来比较和赋值。
        for(ListNode temp = first.cur->next; temp != last.cur; temp = temp->next){
            //销毁结点之后temp->next的值将变化，所以应当先将指针后移，然后销毁prev，这样子就能规避这个问题
            //temp == last的时候，正好销毁完了temp->prev，也就是last->prev，恰好完成任务
            m_DestroyNode(temp->prev);
        }
        return last;//此时last在原来first的位置
    }

    void clear(){
        erase(begin(),end());
    }

    /*-------容器数据相关函数完--------*/

};

#endif //HCSTL_LIST_H
