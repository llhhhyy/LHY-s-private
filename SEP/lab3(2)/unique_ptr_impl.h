#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

// You need to implement follow functions, signatures are provided.
// NOTE: DON'T change the function definition

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer { other.release() } {}

template <typename T>
UniquePtr<T>::~UniquePtr() {
    // TODO
    if(pointer)
    {
        delete pointer;
        pointer= nullptr;
    }
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
    // TODO

    reset(other.pointer);
    return *this;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(std::nullptr_t) noexcept {
    // TODO
    pointer= nullptr;
    return *this;
}

template <typename T>
void UniquePtr<T>::reset(T *ptr) noexcept {
    // TODO
    pointer=ptr;
    ptr= nullptr;
    delete ptr;
}

template <typename T>
T *UniquePtr<T>::release() noexcept {
    // TODO
    T *tmp=pointer;
    pointer= nullptr;
    return tmp;
}

#endif  // UNIQUE_PTR_IMPL_H
