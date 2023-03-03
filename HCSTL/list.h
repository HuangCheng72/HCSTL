//
// Created by huangcheng on 2023/3/2.
//

#ifndef HCSTL_LIST_H
#define HCSTL_LIST_H

//STL的list是一个环形双向链表
#include "type_traits.h"
#include "allocator.h"
#include "constructor.h"

template <typename T>
struct __list_node {
    __list_node* prev;    //指向上一结点的指针
    __list_node* next;    //指向下一结点的指针
    T value;            //数据
};

template <typename T, typename Alloc = allocator<__list_node<T>> >
class list {
protected:
    typedef __list_node<T> list_node;
public:
    typedef list_node* link_type;

    typedef size_t size_type;   //这个不用说了，前面vector都知道了
protected:
    link_type node;     //只需一个结点即可表示整个环形双向链表
    typedef Alloc data_allocator;   //表示空间配置器
    link_type m_CreatNode(){    //创建一个结点，这个结点不带有意义的数据
        link_type res = data_allocator::allocate(1);
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
    link_type m_CreatNode(const T& value){    //创建一个结点，这个结点的数据拷贝自value
        link_type res = data_allocator::allocate(1);
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
    void m_DestroyNode(link_type& n){    //销毁一个结点
        if(!hc_type_bool<typename _type_traits<T>::has_trivial_destructor>::value){
            //non-POD类型需要调用析构器
            destroy(&(n->value));//析构数据;
        }
        destroy(n);//析构结点
        data_allocator::deallocate(n);//回收空间
    }

public:
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

    link_type begin() {
        return node->next;
    }
    link_type end() {
        return node;    //node是不保存数据的，它相当于是最末尾的节点，为了符合前开后闭[first, last)，这个node起到last的作用
    }
    bool empty() const {
        return node->next == node;
    }
    size_type size() const {
        size_type count = 0;
        for(link_type temp = node->next; temp != node; temp = temp->next, count++){}
        return count;
    }

    link_type erase(link_type position) {     //消除某位置上元素
        link_type temp = position->next;
        position->prev->next = position->next;
        position->next->prev = position->prev;//从链表中删除position这个结点
        m_DestroyNode(position);
        return temp;//现在在原来Position位置上的结点
    }

    link_type erase(link_type a, link_type b) {     //消除两个指针之间的元素，注意：a为起点，b为终点
        //析构中间的元素，然后链接即可
        link_type res = a->prev;
        res->next = b;
        b->prev = res;
        for(link_type temp = a->next; temp != b; temp = temp->next){
            //销毁结点之后temp->next的值将变化，所以应当先将指针后移，然后销毁prev，这样子就能规避这个问题
            //temp == b的时候，正好销毁完了temp->prev，也就是b->prev，恰好完成任务
            m_DestroyNode(temp->prev);
        }
        return b;//此时b在原来a的位置
    }

    void resize (size_type n){
        if(n == 0){
            //特殊情况
            clear();
        }
        //多余删除，缺少就添加，默认构造
        size_type count = size();
        link_type _end = end();
        if(n < count){
            //多余要删除
            for(size_type i = n; i > count; --i){
                link_type temp = _end->prev;
                _end->prev = temp->prev;
                temp->prev->next = _end;//从链表中删除temp这个结点
                m_DestroyNode(temp);
            }
        }else{
            //缺少要添加
            for(size_type i = count; i < n; ++i){
                link_type temp = m_CreatNode();
                temp->prev = _end->prev;
                temp->next = _end;
                temp->prev->next = temp;
                temp->next->prev = temp;
            }
        }
    }
    void resize (size_type n, const T& val){
        if(n == 0){
            //特殊情况
            clear();
        }
        //多余删除，缺少添加，拷贝构造
        size_type count = size();
        link_type _end = end();
        if(n < count){
            //多余要删除
            for(size_type i = n; i > count; --i){
                link_type temp = _end->prev;
                _end->prev = temp->prev;
                temp->prev->next = _end;//从链表中删除temp这个结点
                m_DestroyNode(temp);
            }
        }else{
            //缺少要添加
            for(size_type i = count; i < n; ++i){
                link_type temp = m_CreatNode(val);
                temp->prev = _end->prev;
                temp->next = _end;
                temp->prev->next = temp;
                temp->next->prev = temp;
            }
        }
    }

    T& front() {
        return node->next->value;
    }
    T& back() {
        return node->prev->value;
    }

    void push_back (const T& val) {
        //拷贝构造
        link_type temp = m_CreatNode(val);
        temp->prev = end()->prev;
        temp->next = end();
        temp->prev->next = temp;
        temp->next->prev = temp;
    }
    void push_front(const T& val) {
        //拷贝构造
        link_type temp = m_CreatNode(val);
        temp->next = end()->next;
        temp->next->prev = temp;
        end()->next = temp;
        temp->prev = end();
    }
    void pop_back(){
        link_type temp = end()->prev;
        end()->prev = temp->prev;
        temp->prev->next = end();//从链表中删除temp这个结点
        m_DestroyNode(temp);
    }
    void pop_front(){
        link_type temp = end()->next;//要删除temp
        end()->next = temp->next;
        temp->next->prev = end();
        m_DestroyNode(temp);
    }

    void clear(){
        for(link_type temp = begin()->next; temp != end(); temp = temp->next){
            //销毁结点之后temp->next的值将变化，所以应当先将指针后移，然后销毁prev，这样子就能规避这个问题
            //temp == end()的时候，正好销毁完了temp->prev，也就是end()->prev，恰好完成任务
            m_DestroyNode(temp->prev);
        }
        node->next = node;
        node->prev = node;
    }

    //不可能对每个容器都实现一个for_each，需要将for_each分离出来，作为一种算法，应用到任何能使用的容器中。
    template<typename Function>
    void for_each(link_type first, link_type last, Function f) {
        for(link_type temp = first; temp != last; temp = temp->next) {
            f(temp->value);
        }
    }

};

#endif //HCSTL_LIST_H
