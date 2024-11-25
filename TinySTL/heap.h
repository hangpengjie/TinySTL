#ifndef TINYSTL_HEAP_H
#define TINYSTL_HEAP_H

#include <type_traits>
#include <iterator>


namespace hstl{

template <class RandomAccessIterator, class Distance, class T>
void __push_heap(RandomAccessIterator first, Distance holeIndex, Distance topIndex, T value){
    Distance parent = (holeIndex - 1) / 2;
    while(holeIndex > topIndex && *(first + parent) < value){
        *(first + holeIndex) = *(first + parent);
        holeIndex = parent;
        parent = (holeIndex - 1) / 2;
    }
    *(first + holeIndex) = value;
}

template <class RandomAccessIterator, class Distance, class T>
void __push_heap_aux(RandomAccessIterator first, RandomAccessIterator last, Distance*, T*){
    __push_heap(first, Distance((last - first) - 1), Distance(0), T(*(last - 1)));
}

// the new value is already at the end of the heap
template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first, RandomAccessIterator last){
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    __push_heap_aux(first, last, static_cast<difference_type*>(0), static_cast<value_type*>(0));
}


template <class RandomAccessIterator, class Distance, class T>
void __adjust_heap(RandomAccessIterator first, Distance holeIndex, Distance len, T value){
    Distance topIndex = holeIndex;
    Distance secondChild = 2 * holeIndex + 2;
    while(secondChild < len){
        if(*(first + secondChild) < *(first + (secondChild - 1))){
            secondChild--;
        }
        *(first + holeIndex) = *(first + secondChild);
        holeIndex = secondChild;
        secondChild = 2 * (secondChild + 1);
    }
    if(secondChild == len){
        *(first + holeIndex) = *(first + (secondChild - 1));
        holeIndex = secondChild - 1;
    }
    __push_heap(first, holeIndex, topIndex, value);
}

template <class RandomAccessIterator, class Distance, class T>
void __pop_heap(RandomAccessIterator first, RandomAccessIterator last, RandomAccessIterator result, T value, Distance*){
    *result = *first;
    __adjust_heap(first, Distance(0), Distance(last - first), value);
}

template <class RandomAccessIterator, class T>
void __pop_heap_aux(RandomAccessIterator first, RandomAccessIterator last, T*){
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    __pop_heap(first, last - 1, last - 1, T(*(last - 1)), static_cast<difference_type*>(0));
}

// move the top element of the heap to the end of the heap
template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first, RandomAccessIterator last){
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    __pop_heap_aux(first, last, static_cast<value_type*>(0));
}

// sort the heap
template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first, RandomAccessIterator last){
    while(last - first > 1){
        hstl::pop_heap(first, last--);
    }
}

// make a heap
template <class RandomAccessIterator, class Distance, class T>
void __make_heap(RandomAccessIterator first, RandomAccessIterator last, T*, Distance*){
    if(last - first < 2){
        return;
    }
    Distance len = last - first;
    Distance parent = (len - 2) / 2;
    while(true){
        __adjust_heap(first, parent, len, T(*(first + parent)));
        if(parent == 0){
            return;
        }
        parent--;
    }
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first, RandomAccessIterator last){
    typedef typename std::iterator_traits<RandomAccessIterator>::value_type value_type;
    typedef typename std::iterator_traits<RandomAccessIterator>::difference_type difference_type;
    __make_heap(first, last, static_cast<value_type*>(0), static_cast<difference_type*>(0));
}


}


#endif