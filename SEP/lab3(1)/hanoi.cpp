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
                int a,b;
                cout<<"Move a disk. Format: x y"<<endl;
                cin>>a>>b;
                if(a==0&&b==0)
                {
                    board.autoplay();
                    continue;
                }
                if(a<=0||b>3)
                {
                    board.draw();
                    continue;
                }
                board.move(a,b);
                board.draw();
            }

            if(board.win())
            {
                cout<<"Congratulations! You win!"<<endl;
                continue;
            }
        }
    }
    return 0;
}
