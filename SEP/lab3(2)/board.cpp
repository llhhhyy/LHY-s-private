#include <iostream>

#include "board.h"
#include "queue.h"

Board::Board(const int num_disk) :num_disk(num_disk),rods{Rod(num_disk,0), Rod(num_disk,1), Rod(num_disk,2)}{
    for(int i=0;i<num_disk;)
    {
        if(rods[0].push(Disk(i,2*(num_disk-i)+1)))
        {
            i++;
        }
    }
}

Board::~Board() {
    // TODO
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    // TODO
    for(int i=0;i<3;i++)
    {
        rods[i].draw(canvas);
    }
    canvas.draw();
}

void Board::move(int from, int to, const bool log) {
    // TODO
    if(from==0&&to==0)
    {
        autoplay();
        return;
    }
    if(from<0||to<0||to>3)
    {
        return;
    }
    if(rods[from-1].empty()||rods[to-1].full())
    {
        return;
    }
    Disk tmp=rods[from-1].stack.top();
    if(!rods[to-1].push(tmp))
    {
        return;
    }
    else
    {
        std::pair<int,int> current(from,to);
        if(log){
            history.push(current);
        }
        rods[to-1].pop();
        if(rods[to-1].empty())
        {
            tmp.id=0;
            rods[to-1].stack.push(tmp);
        }
        else
        {
            Disk temp=rods[to-1].top();
            tmp.id=temp.id+1;
            rods[to-1].stack.push(tmp);
        }
        rods[from-1].stack.pop();
    }
}

bool Board::win() const {
    // TODO
    if(rods[0].empty()&&rods[2].empty())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void solve(
    const int n,
    const int src,
    const int buf,
    const int dest,
    Queue<std::pair<int, int>> &solution
) {
    // TODO
    std::pair<int,int> s;
    if(n==0)return;

    else if(n==1)
    {
        s.first=src;
        s.second=dest;
        solution.push(s);
        return;
    }
    else{
        solve(n-1,src,dest,buf,solution);
        s.first=src;
        s.second=dest;
        solution.push(s);
        solve(n-1,buf,src,dest,solution);
    }
}

void Board::autoplay() {
    // TODO
    while(!history.empty())
    {
        std::pair<int,int> his=history.top();
        move(his.second,his.first, false);
        std::cout<<"Auto moving: "<<his.second<<"->"<<his.first<<std::endl;
        draw();
        history.pop();
    }
    Queue<std::pair<int,int>> solution;
    solve(num_disk,1,3,2,solution);
    while(!solution.empty())
    {
        std::pair<int,int> solu=solution.front();
        move(solu.first,solu.second);
        std::cout<<"Auto moving:"<<solu.first<<"->"<<solu.second<<std::endl;
        draw();
        solution.pop();
    }
}
