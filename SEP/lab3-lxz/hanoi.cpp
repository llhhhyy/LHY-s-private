#include <climits>
#include <iostream>
#include <string>

#include "board.h"

using namespace std;

int main() {
    Stack<int> stack;
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }

        // TODO
        if (input == "1" || input == "2" || input == "3" || input == "4" || input == "5"){
            Board board(input[0]-'0');
            //cout << "Success Build\n";
            board.draw();
            while (true){
                cout << "Move a disk. Format: x y" << endl;
                getline(cin, input);
                if (input.length() == 3 && input[1] == ' '){
                    if ((input[0] - '0' >= 1 && input[0] - '0' <= 3) && (input[2] - '0' >= 1 && input[2] - '0' <= 3)){
                        board.move(input[0] - '1', input[2] - '1', true);
                        board.draw();
                    }
                    else if (input[0] == '0' && input[2] == '0'){
                        board.autoplay();
                    }
                    else
                        board.draw();
                }
                else
                    board.draw();
                if (board.win()){
                    cout << "Congratulations! You win!" << endl;
                    break;
                }
            }
        }
    }
    return 0;
}
/*
2
0 0
Q

2
1 2
2 3
1 2
3 2
Q
*/