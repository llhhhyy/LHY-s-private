#ifndef UNIQUE_PTR_IMPL_H
#define UNIQUE_PTR_IMPL_H

// You need to implement follow functions, signatures are provided.
// NOTE: DON'T change the function definition

template <typename T>
UniquePtr<T>::UniquePtr(UniquePtr &&other) noexcept : pointer {/*TODO*/ other.release()  } { }

template <typename T>
UniquePtr<T>::~UniquePtr() {
    // TODO
    delete pointer;
    pointer= nullptr;
    //save a question
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(UniquePtr &&other) noexcept {
    // TODO
    if(&other != this)
        reset(other.release());
    return *this;
}

template <typename T>
UniquePtr<T> &UniquePtr<T>::operator=(std::nullptr_t) noexcept {
    // TODO
    pointer=nullptr;
    return *this;
}

template <typename T>
void UniquePtr<T>::reset(T *ptr) noexcept {
    // TODO
//    if(ptr!= nullptr)
    delete pointer;
    pointer=ptr;
//    else pointer= nullptr;
}

template <typename T>
T *UniquePtr<T>::release() noexcept {
    // TODO
    T *tmp=get();
    pointer= nullptr;
    return  tmp;
}

#endif  // UNIQUE_PTR_IMPL_H
