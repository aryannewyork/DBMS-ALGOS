#include <iostream>
#include <string.h>
#include <vector>
#include <Set>
using namespace std;

string unionStrings(string x, string y){
  set<char> s;
  for(auto i : x) {
    s.insert(i);
  }
  for(auto i : y) {
    s.insert(i);
  }
  string res = "";
  for(auto i : s){
    res += i;
  }
  return res;
}

bool isSubset(string x, string y) {
  int lenx = x.length();
  int leny = y.length();
  int count = 0;
  for(int i = 0; i < lenx; i++){
    bool has = false;
    for(int j = 0; j < leny; j++){
      if(x[i] == y[j]){
        has = true;
        break;
      }
    }
    if(has){
      count++;
    } else {
      return false;
    }
  }
  return true;
}

string attributeClosure(set <pair<string, string>> fds, string attribute) {
  pair<string, string> res = {attribute, attribute};
  string currResult = attribute;
  string prevResult = "";
  while(currResult != prevResult) {
    bool flag = false;
    for(auto i : fds) {
      if(isSubset(i.first, currResult)) {
        prevResult = currResult;
        if(!isSubset(i.second, currResult)) {
          currResult = unionStrings(currResult, i.second);
        }
        flag = true;
      }
    }
    if(!flag){
      return currResult;
    }
  }
  return currResult;
}


int main() {
  set <pair<string, string>> fds =  {{"AC","G"}, {"D", "EG"}, {"BC", "D"}, {"CG", "BD"}, {"ACD", "B"}, {"CE", "AG"}};
  string attribute = "CD";
  string result = attributeClosure(fds, attribute);
  cout << result << "\n";

}