#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>

template <typename T>
void Stack<T>::push(T t) {
    // TODO
    UniquePtr<Node<T>> tmp = MakeUnique<Node<T>>(t);
    if(sz==0)
    {
        head.reset(tmp.release());
        sz++;
    }
    else
    {
        tmp->next.reset(head.release());
        head.reset(tmp.release());
        sz++;
    }
}

template <typename T>
void Stack<T>::pop() {
    // TODO
    if(sz==0)return;
    UniquePtr<Node<T>> del;
    del.reset(head.release());
    head.reset(del->next.release());
    sz--;
}

template <typename T>
T &Stack<T>::top() {
    // TODO
    return head->val;

}

template <typename T>
bool Stack<T>::empty() const {
    // TODO
    if(sz == 0){return true;}
    else {return false;}
}

template <typename T>
size_t Stack<T>::size() const {
    // TODO
    return sz;
}

#endif  // STACK_IMPL_H
