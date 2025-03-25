#include <climits>
#include <iostream>
#include <string>

#include "board.h"

using namespace std;

int main() {
    while (true) {
        cout << "How many disks do you want? (1 ~ 5)" << endl;
        string input;
        getline(cin, input);
        if (input == "Q") {
            break;
        }
        // TODO
        else if(input<"1"||input>"5")
        {
            continue;
        }
        else
        {
            int num=input[0]-'0';
            Board board(num);
            board.draw();

            while(!board.win())
            {
                cout<<"Move a disk. Format: x y"<<endl;
                getline(cin, input);
                if (input.length() == 3 && input[1] == ' '){
                    if ((input[0] - '0' > 0 && input[0] - '0' < 4) && (input[2] - '0' > 0 && input[2] - '0' < 4)){
                        board.move(input[0] - '0', input[2] - '0');
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
                if(board.win())
                {
                    cout<<"Congratulations! You win!"<<endl;
                }
            }
        }
    }
    return 0;
}
