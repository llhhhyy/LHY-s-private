#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

template <typename T>
void Queue<T>::push(T t) {
    // TODO
    auto* a=new Node<T>(t);
    UniquePtr<Node<T>> tmp(a);
    if(sz==0){
        head=std::move(tmp);
        tail=head.get();
    }
    else{
        tail->next=std::move(tmp);
        tail=tail->next.get();
    }
    sz++;
}

template <typename T>
void Queue<T>::pop() {
    // TODO
    head=std::move(head->next);
//    head.reset(std::move(head->next.get()));
    sz--;
}

template <typename T>
T &Queue<T>::front() {
    // TODO
    Node<T>* tmp=head.get();
    return tmp->val;
}

template <typename T>
bool Queue<T>::empty() const {
    // TODO
    if(sz==0)   return true;
    return false;
}

template <typename T>
size_t Queue<T>::size() const {
    // TODO
    return sz;
}
#endif  // QUEUE_IMPL_H
