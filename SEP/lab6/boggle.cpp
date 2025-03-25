#include "lexicon.h"
#include <vector>
#include <iostream>
#include <utility>
#include <set>
#include <algorithm>
#include <cctype>

using namespace std;

vector<std::pair<int, int>> dir = {
        {1, 0}, {-1, 0}, {0, 1}, {0, -1},//right,left,down,up
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}//right and down,right and up,left and down,left and up
};

void dfs(const vector<vector<char>>& map, int x, int y, string& currentWord,
         vector<vector<bool>>& visited, Lexicon& foundWords,Lexicon &lex) {
    if (currentWord.length() >= 4&&lex.contains(currentWord)) {
        foundWords.add(currentWord);
    }

    for (const auto& d : dir) {
        int nx = x + d.first;
        int ny = y + d.second;

        if (nx >= 0 && nx < map.size() && ny >= 0 && ny < map[0].size() && !visited[nx][ny]) {
            visited[nx][ny] = true;
            currentWord.push_back(map[nx][ny]);

            if(lex.containsPrefix(currentWord))
               dfs(map, nx, ny, currentWord, visited, foundWords,lex);

            currentWord.pop_back();
            visited[nx][ny] = false;
        }
    }
}

Lexicon findWords(const vector<vector<char>>& map,Lexicon &lex) {
    Lexicon foundWords;
    int n = map.size();
    if (n == 0) return foundWords;

    vector<vector<bool>> visited(n, vector<bool>(n, false));
    string currentWord;

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            currentWord = "";
            currentWord.push_back(map[i][j]);
            visited[i][j] = true;

            dfs(map, i, j, currentWord, visited, foundWords,lex);

            visited[i][j] = false;
        }
    }

    return foundWords;
}

int main() {
  // TODO
  Lexicon lex("EnglishWords.txt");
  Lexicon allWords;
  Lexicon FoundWordsOfP1, FoundWordsOfP2;
  vector<vector<char>> map;

  int n;
  cin>>n;
  map.resize(n, std::vector<char>(n));
  for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
          cin>>map[i][j];
      }
  }

  allWords = findWords(map,lex);
  string p1="",p2="";
  int scoreOfP1=0,scoreOfP2=0;
  while(p1!="???"){
      cout<<"Player 1 Score: "<<scoreOfP1<<endl;
      cin>>p1;
      if(p1=="???")break;
      if(p1.size()<4){
          cout<<p1<<" is too short."<<endl;
          continue;
      }
      if(!lex.contains(p1)){
          cout<<p1<<" is not a word."<<endl;
          continue;
      }
      if(FoundWordsOfP1.contains(p1)){
          cout<<p1<<" is already found."<<endl;
          continue;
      }
      if(allWords.contains(p1)){
          cout<<"Correct."<<endl;
          FoundWordsOfP1.add(p1);
          scoreOfP1+=p1.size()-3;
      }else{
          cout<<p1<<" is not on board."<<endl;
      }
  }
    while(p2!="???"){
        cout<<"Player 2 Score: "<<scoreOfP2<<endl;
        cin>>p2;
        if(p2=="???")break;
        if(p2.size()<4){
            cout<<p2<<" is too short."<<endl;
            continue;
        }
        if(!lex.contains(p2)){
            cout<<p2<<" is not a word."<<endl;
            continue;
        }
        if(FoundWordsOfP2.contains(p2)){
            cout<<p2<<" is already found."<<endl;
            continue;
        }
        if(allWords.contains(p2)){
            cout<<"Correct."<<endl;
            FoundWordsOfP2.add(p2);
            scoreOfP2+=p2.size()-3;
        }else{
            cout<<p2<<" is not on board."<<endl;
        }
    }
    if(p1=="???"&&p2=="???"){
        cout<<"Player 1 Score: "<<scoreOfP1<<endl;
        cout<<"Player 2 Score: "<<scoreOfP2<<endl;
        if(scoreOfP1>scoreOfP2){
            cout<<"Player 1 wins!"<<endl;
        }else if(scoreOfP2>scoreOfP1){
            cout<<"Player 2 wins!"<<endl;
        }else{
            cout<<"It's a tie!"<<endl;
        }
    }
    set<string> all = allWords.toStlSet();
    cout<<"All Possible Words: ";
    for(string ele:all){
        transform(ele.begin(), ele.end(), ele.begin(), ::toupper);
        cout<<ele<<" ";
    }
  return 0;
}
