#include "rod.h"
#include <cstddef>

Rod::Rod(const int capacity, const int id) : /* TODO */ capacity(capacity), id(id){ }

Rod::~Rod(){}

bool Rod::push(const Disk d) {
    // TODO
    //std::cout << "Rod push\n";
    if (!empty() && d.val > stack.top().val){
        //std::cout << "Wrong\n";
        return false;
    }
    stack.push(d);
    return true;
}

const Disk &Rod::top() {
    // TODO
    return stack.top();
}

void Rod::pop() {
    // TODO
    stack.pop();
}

size_t Rod::size() const {
    // TODO
    return stack.size();
}

bool Rod::empty() const {
    // TODO
    return stack.empty();
}
bool Rod::full() const {
    // TODO
    return size() == capacity;
}
void Rod::draw(Canvas &canvas) {
    // TODO
    for (int i = 0;i < 11;i++){
        canvas.buffer[i][15 * id + 5] = '|';
    }
    Stack<const Disk> tmp;
    while (!empty()){
        stack.top().draw(canvas, int(size()-1), id);
        tmp.push(stack.top());
        stack.pop();
    }
    while (!tmp.empty()){
        stack.push(tmp.top());
        tmp.pop();
    }
}
