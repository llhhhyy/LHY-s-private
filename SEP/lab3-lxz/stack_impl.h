#ifndef STACK_IMPL_H
#define STACK_IMPL_H
#include <cassert>

template <typename T>
void Stack<T>::push(T t) {
    // TODO
    //std::cout << "Stack push\n";
    UniquePtr<Node<T>> tmp = MakeUnique<Node<T>>(t);
    if (sz == 0){
        head.reset(tmp.release());
        sz++;
    }
    else{
        tmp->next = std::move(head);
        head = std::move(tmp);
        sz++;
    }
}

template <typename T>
void Stack<T>::pop() {
    // TODO
    if (sz == 0)
        return;
    UniquePtr<Node<T>> tmp;
    tmp = std::move(head);
    head = std::move(tmp->next);
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
    return sz == 0;
}

template <typename T>
size_t Stack<T>::size() const {
    // TODO
    return sz;
}

#endif  // STACK_IMPL_H
