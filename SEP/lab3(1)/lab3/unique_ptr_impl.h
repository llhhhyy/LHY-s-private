#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

// You need to implement follow functions, signatures are provided.
// NOTE: DON'T change the function definition

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer { /* TODO */ } {

    pointer=new T;
    pointer=other.pointer;
    other= nullptr;
}

template <typename T>
UniquePtr<T>::~UniquePtr() {
    // TODO
    delete pointer;
    pointer= nullptr;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
    // TODO
    pointer=new T;
    pointer=other.pointer;
    return *this;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(std::nullptr_t) noexcept {
    // TODO
    pointer=new T;
    pointer= nullptr;
    return *this;
}

template <typename T>
void UniquePtr<T>::reset(T *ptr) noexcept {
    // TODO
    pointer= nullptr;
    pointer=ptr;
    ptr= nullptr;
}

template <typename T>
T *UniquePtr<T>::release() noexcept {
    // TODO
    T *tmp=pointer;
    pointer= nullptr;
    return tmp;
}

#endif  // UNIQUE_PTR_IMPL_H
