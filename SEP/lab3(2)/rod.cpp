#include "rod.h"
#include <cstddef>

Rod::Rod(const int capacity, const int id) :capacity(capacity),id(id) /* TODO */ { }

bool Rod::push(const Disk d) {
    // TODO
    if(full())
    {
        return false;
    }
    if(!empty())
    {
        if(d.val>stack.top().val)
        {
            return false;
        }
        else
        {
            stack.push(d);
            return true;
        }
    }
    if(empty())
    {
        stack.push(d);
        return true;
    }
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
    if(stack.size()==capacity)
        return true;
    else
        return false;
}
void Rod::draw(Canvas &canvas) {
    // TODO
    int x=5;
    for(int i=0;i<41;i++)
    {
        canvas.buffer[10][i]='-';
    }
    for(int y=0;y<11;y++)
    {
        canvas.buffer[y][x+id*15]='|';
    }
    for(int i=5;i<41;i+=15)
    {
        canvas.buffer[10][i]='|';
    }
    Stack<const Disk> tmp;
    while (!empty()){
        stack.top().draw(canvas,int(size()-1),id);
        tmp.push(stack.top());
        stack.pop();
    }
    while (!tmp.empty()){
        stack.push(tmp.top());
        tmp.pop();
    }
}
Rod::~Rod() {
    while(!stack.empty())
    {
        stack.pop();
    }
}
