//
// Created by huangcheng on 2023/3/17.
//

#ifndef HCSTL_DEQUE_H
#define HCSTL_DEQUE_H

#include "allocator.h"
#include "constructor.h"
#include "iterator.h"
#include "type_traits.h"
#include "uninitialized.h"

//官方实现的STL deque，要可以自行指定缓冲区的长度
//这里只是做一个Demo，暂时只管实现就行了。
//我在这里默认每个buffer长度为4（现实中一般是512）
//这本来应该是一个计算的函数，我这里简化一下，直接变成一个常数了
#define buffer_size 2

//内部实现的迭代器，对外暴露的迭代器要再次包装才可以
template<typename T>
struct deque_iterator{

    //没有继承iterator结构体，所以要自行把这些嵌套定义写下来

    typedef random_access_iterator_tag iterator_category;
    typedef T value_type;
    typedef ptrdiff_t difference_type;
    typedef T* pointer;
    typedef T& reference;

    //与deque保持一致
    typedef pointer* map_pointer;
    typedef deque_iterator<T> self;

    //保存四个信息
    //当前缓冲区的头、尾、本迭代器指向的当前元素对象，中控map中的位置（直接指向这个位置，解引用之后得到当前缓冲区）
    T* first;
    T* last;
    T* cur;
    map_pointer node;

    //重设迭代器所在的缓冲区
    void set_node(map_pointer new_node) {
        node = new_node;
        first = *new_node;
        last = first + difference_type(buffer_size);
    }

    //重载各种运算符
    reference operator*() const {
        return *cur;
    }
    pointer operator->() const {
        return &(operator*());
    }
    difference_type operator-(const self& x) const {
        //指针减法，特别注意
        //因为是分段的，所以计算方式大概如下
        //模型：
        //0 -----x----
        //1 ----------
        //2 ----------
        //3 ---i------
        //所以，计算x到i的距离
        //dis = (i所在的缓冲区指针 - x所在的缓冲区指针) * buffer_size - x到first0的距离 + i到first3距离

        return difference_type(buffer_size) * (node - x.node) + (cur - first) - (x.cur - x.first);

    }
    self& operator++() {
        ++cur;
        if( cur == last ) {
            set_node(node + 1);
            cur = first;
        }
        return *this;
    }
    self& operator++(int) {
        self temp = *this;
        ++(*this);//直接调用++i
        return temp;
    }
    self& operator--() {
        if( first == cur ) {
            set_node(node - 1);
            cur = last;
        }
        --cur;
        return *this;
    }
    self& operator--(int) {
        self temp = *this;
        --(*this);//直接调用--i
        return temp;
    }
    self& operator+= (difference_type n) {
        //实现随机存取，直接跳跃n个距离

        //第一步，首先确定自身当前位置
        difference_type position = cur - first;
        //第二步，目的地距离当前缓冲区的first多少（偏移量），以当前缓冲区的first为起点移动
        difference_type target_offset = position + n;
        //第三步，target_offset 为负数 or 为正数
        if(target_offset >= 0) {
            if( target_offset < difference_type(buffer_size) ) {
                //就在本缓冲区内向后移动，因而直接相加即可
                cur += n;
            } else {
                //出缓冲区了
                //要横跨多少个缓冲区
                difference_type node_offset = target_offset / difference_type(buffer_size);
                //跨出
                set_node(node + node_offset);
                //target_offset减去中间经过的缓冲区的偏移量
                target_offset -= (node_offset) * difference_type(buffer_size);
                //剩下的偏移量，直接从目的地缓冲区的first起算即可到达
                cur = first + target_offset;
            }
        } else {
            //target_offset为负数，说明必须出缓冲区了
            //这里的算法不太一样

            //0 f-----x----l
            //1 f----------l
            //2 f---i------l

            //f2 到 x 为 target_offset
            //上一个的l理论上应该是这一个的f（实际上node不同）
            //即l1 到 x 为 target_offset
            //故 node_offset = (-target_offset) / buffer_size
            //l1到x仅仅需要移动一个缓冲区，因为l1所在的缓冲区是1，但是本缓冲区是2，需要移动2个，所以：
            //set_node(node - node_offset - 1);（这是因为实际上node不同）

            //但是有一种特殊情况，尾 - （元素数 - 1）直接到头，如:
            //abcd
            //efgh
            //hijk
            //k - 11，应到a
            //如果按照以上逻辑
            //n = -11，position = 3，target_offset = -11 + 3 = -8
            //node_offset = 8 / 4 = 2
            //set_node不可能再-1
            //所以可以node_offset = (-target_offset - 1) / buffer_size
            //这样可以规避这种情况，同时也不会有什么影响，哪怕缓冲区长度是1也一样


            //我们的整体思路和之前两迭代器相减计算距离的时候差不多
            //从f2到f0，已经移动了(node_offset + 1) * buffer_size
            //应当从f0起移动到目标所需的偏移量就是 ( (node_offset + 1) * buffer_size ) + target_offset（这是个负数，所以直接相加就行了）
            //cur = f0 + ( node_offset + 1 * buffer_size ) + target_offset

            difference_type node_offset = (-target_offset - 1) / difference_type(buffer_size);
            set_node(node - node_offset - 1);
            target_offset += (node_offset + 1) * difference_type(buffer_size);
            cur = first + target_offset;

        }
        return *this;
    }
    self& operator-=(difference_type n) {
        return *this += -n;
    }
    self operator+(difference_type n) const {
        self temp = *this;
        return temp += n;
    }
    self operator-(difference_type n) const {
        self temp = *this;
        return temp -= n;
    }
    reference operator[](difference_type n) {
        return *( (*this) + n );
    }
    bool operator==(const self& x) {
        return cur == x.cur;
    }
    bool operator!=(const self& x) {
        //直接取反
        return !(*(this) == x);
    }
    bool operator< (const self& x) {
        return (node == x.node) ? (cur < x.cur) : (node < x.node);
    }
    bool operator> (const self& x) {
        return (node == x.node) ? (cur > x.cur) : (node > x.node);
    }


};

template<typename T>
class deque {
public:
    /*-------嵌套类型别名--------*/

    typedef size_t size_type;

    typedef T value_type;
    typedef T* pointer;
    typedef T& reference;
    typedef const T& const_reference;

    typedef deque_iterator<T> iterator;
    typedef ptrdiff_t difference_type;

    typedef allocator<value_type> data_allocator;
    typedef allocator<pointer> map_allocator;


    /*-------嵌套类型别名完--------*/
protected:
    typedef pointer* map_pointer;

    map_pointer map;    //中控map
    size_type map_size; //map可容纳的指针数

    //这就是一个自己建立的二维数组

    //两个迭代器
    iterator start;
    iterator finish;

    //辅助函数，三倍扩容
    void Dilatation_Map_And_Nodes_aux() {
        //此处有必要设立缓冲初值
        //因而要初始化工具
        map_pointer new_map = map_allocator::allocate(map_size * 3);
        //原来的那部分要在中间
        //打个比方，原本的map_size为2，扩容之后是6
        //那么两个缓冲区就要在中间
        //0 ---------空的--------
        //1 ---------空的--------
        //2 ----原来的缓冲区数据----
        //3 ----原来的缓冲区数据----
        //4 ---------空的--------
        //5 ---------空的--------

        //原来缓冲区对应扩容后缓冲区的下标（我都是三倍扩容的，这个只适用于三倍）
        //new_i_first = map_size
        //然后new_i_first++

        for(int i = 0; i < map_size * 3; i++) {
            //申请空间
            new_map[i] = data_allocator::allocate(buffer_size);
        }
        for(int i = 0; i < map_size; i++) {
            //在这个过程顺便更新迭代器
            if( start.node == map + i ){
                difference_type temp = start.cur - start.first;
                start.node = new_map + i + map_size;
                start.first = *(start.node);
                start.last = start.first + buffer_size;
                start.cur = start.first + temp;
            }
            if( finish.node == map + i ){
                difference_type temp = finish.cur - finish.first;
                finish.node = new_map + i + map_size;
                finish.first = *(finish.node);
                finish.last = finish.first + buffer_size;
                finish.cur = finish.first + temp;
            }
            uninitialized_move_n(new_map[i + map_size], buffer_size, map[i]);//扩展的时候直接把原来空间全部的东西都给移动过来

            //移动之后原来空间也就剩个空壳了，直接回收就行了
            data_allocator::deallocate(map[i]);
        }
        //回收map中控
        map_allocator::deallocate(map);
        //复制新的map中控
        map = new_map;
        //更新map_size
        map_size *= 3;
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

    deque() {
        //初始化
        map_size = 3;
        //此处有必要设立缓冲初值
        //因而要初始化工具
        map = map_allocator::allocate(map_size);
        for(int i = 0; i < map_size; i++) {
            map[i] = data_allocator::allocate(buffer_size);
        }
        start.node = map + (map_size / 3);
        start.first = *(start.node);
        start.last = start.first + buffer_size;
        start.cur = start.first;
        finish = start;
    }

    ~deque() {
        clear();
        for(int i = 0; i < map_size; i++) {
            //移动之后原来空间也就剩个空壳了，直接回收就行了
            data_allocator::deallocate(map[i]);
        }
        //回收中控map
        map_allocator::deallocate(map);
    }

    /*-------构造器与析构器相关函数完--------*/

    /*-------容器容量相关函数--------*/

    size_type size() const {
        return size_type(finish - start);
    }

    bool empty() const {
        return size() == 0;
    }

    /*-------容器容量相关函数完--------*/

    /*-------容器数据相关函数--------*/

    reference front() const {
        return *begin();
    }

    reference back() const {
        return *end();
    }

    void push_front(const_reference x) {
        //[start , finish)，这里也同样遵循
        //默认该位置上有元素，所以要先判断是否需要扩容
        if(start.node == map && start.cur == start.first) {
            //需要扩容了
            std::cout<<"Dilatation. Front."<<std::endl;
            Dilatation_Map_And_Nodes_aux();
        }
        //无需扩容的话，直接在新位置构造（该位置上有元素，所以要先移动迭代器）
        --start;
        construct(start.cur, x);
    }

    void push_front(value_type&& x) {
        //[start , finish)，这里也同样遵循
        //默认该位置上有元素，所以要先判断是否需要扩容
        if(start.node == map && start.cur == start.first) {
            //需要扩容了
            std::cout<<"Dilatation. Front."<<std::endl;
            Dilatation_Map_And_Nodes_aux();
        }
        //无需扩容的话，直接在新位置构造（该位置上有元素，所以要先移动迭代器）
        --start;
        construct(start.cur, move(x));
    }

    void push_back(const_reference x) {
        //[start , finish)，这里也同样遵循
        //默认该位置上没有元素，可以直接构造
        construct(finish.cur, x);
        //构造完成之后，应当判断是否还有空位，如果没有空位了，就应该扩容
        if(finish.node == map + map_size - 1 && finish.cur == finish.last - 1) {
            std::cout<<"Dilatation. Back."<<std::endl;
            Dilatation_Map_And_Nodes_aux();
        }
        //不管是否扩容，都要移动迭代器
        ++finish;
    }

    void push_back(value_type&& x) {
        //[start , finish)，这里也同样遵循
        //默认该位置上没有元素，可以直接构造
        construct(finish.cur, move(x));
        //构造完成之后，应当判断是否还有空位，如果没有空位了，就应该扩容
        if(finish.node == map + map_size - 1 && finish.cur == finish.last - 1) {
            std::cout<<"Dilatation. Back."<<std::endl;
            Dilatation_Map_And_Nodes_aux();
        }
        //不管是否扩容，都要移动迭代器
        ++finish;
    }

    void pop_front() {
        destroy( &(*(start)) );
        start++;
    }
    void pop_back() {
        destroy( &(*(finish - 1)) );
        finish--;
    }

    reference operator[] (size_type n) {
        return start[difference_type(n)];
    }

    iterator erase(iterator position) {     //消除某位置上元素
        //将后面元素往前移动即可
        uninitialized_move(position, finish, position + 1);
        --finish;   //指针向前移动
        //因为是移动语义的，无需析构（资源都搬空了）
        return position;
    }

    iterator erase(iterator first, iterator last) {
        //仿照vector的
        //将后面元素往前移动即可，把last到finish这一段，复制到first开始的空间这里
        difference_type diff = finish - last;
        uninitialized_move_n(first, diff, last);
        //因为是移动语义的，last到finish这一段无需析构（资源都搬空了）
        //但是要考虑到，first 到 last中间，可能还有一部分没有被搬空，这部分必须析构
        for(; last != first + diff ; --last) {
            destroy( &(*(last)) );
        }
        finish = first + diff;   //指针向前移动
        return finish;
    }

    void clear() {
        erase(begin(), end());
    }

    /*-------容器数据相关函数完--------*/

};

#endif //HCSTL_DEQUE_H
