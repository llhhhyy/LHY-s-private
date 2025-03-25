#include <iostream>

#include "board.h"
#include "queue.h"

Board::Board(const int num_disk) : /* TODO */num_disk(num_disk), rods{Rod(num_disk,0), Rod(num_disk,1), Rod(num_disk,2)} {
    // TODO
    for (int i = 2 * num_disk + 1;i >= 3;i -= 2){
        rods[0].push(Disk(0,i));
    }
}

Board::~Board() {
    // TODO
}

void Board::draw() {
    Canvas canvas {};
    canvas.reset();
    // TODO
    for (int i = 0;i < 41;i++){
        canvas.buffer[10][i] = '-';
    }
    for (int i = 0;i < 3;i++){
        rods[i].draw(canvas);
    }
    canvas.draw();
}

void Board::move(int from, int to, const bool log) {
    // TODO
    if (rods[from].empty())
        return;
    Disk tmp = rods[from].top();
    tmp.id = to;
    if (!rods[to].push(tmp)){
        return;
    }
    if (log)
        history.push(std::pair(from,to));
    rods[from].pop();
    //std::cout << rods[from].size() << rods[to].size();
}

bool Board::win() const {
    // TODO
    return (rods[0].empty() && rods[2].empty());
}

void solve(
    const int n,
    const int src,
    const int buf,
    const int dest,
    Queue<std::pair<int, int>> &solution
) {
    // TODO
    if (n == 0)
        return;
    solve(n - 1, src, dest, buf, solution);
    //std::cout << src+1 << dest+1 << std::endl;
    solution.push(std::pair(src, dest));
    solve(n - 1, buf, src, dest, solution);
}

void Board::autoplay() {
    // TODO
    while (!history.empty()){
        std::cout << "Auto moving:" << history.top().second + 1 << "->" << history.top().first + 1 << std::endl;
        move(history.top().second, history.top().first, false);
        history.pop();
        draw();
    }
    Queue<std::pair<int, int>> solution;
    solve(num_disk, 0, 2, 1, solution);
    std::cout << solution.size() << std::endl;
    while (!solution.empty()){
        std::cout << "Auto moving:" << solution.front().first + 1 << "->" << solution.front().second + 1 << std::endl;
        move(solution.front().first, solution.front().second, false);
        solution.pop();
        draw();
    }
}
