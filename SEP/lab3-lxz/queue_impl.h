#ifndef QUEUE_IMPL_H
#define QUEUE_IMPL_H

template <typename T>
void Queue<T>::push(T t) {
    // TODO
    UniquePtr<Node<T>> tmp = MakeUnique<Node<T>>(t);
    if (sz == 0){
        tail = tmp.get();
        head.reset(tmp.release());
        sz++;
    }
    else{
        tail->next = std::move(tmp);
        tail = tail->next.get();
        sz++;
    }
}

template <typename T>
void Queue<T>::pop() {
    // TODO
    if (sz == 0)
        return;
    UniquePtr<Node<T>> tmp;
    tmp = std::move(head);
    head = std::move(tmp->next);
    sz--;
}

template <typename T>
T &Queue<T>::front() {
    // TODO
    return head->val;
}

template <typename T>
bool Queue<T>::empty() const {
    // TODO
    return sz == 0;
}

template <typename T>
size_t Queue<T>::size() const {
    // TODO
    return sz;
}
#endif  // QUEUE_IMPL_H
