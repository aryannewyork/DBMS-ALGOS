#include <iostream>
#include <string.h>
#include <vector>
#include <set>
using namespace std;

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
          currResult += i.second;
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

string setIntersection(string x, set <string> y) {
  string res = "";
  for(auto i : x) {
    string istr(1, i);
    for(auto j : y) {
      if(istr == j) {
        res += istr;
      }
    }
  }
  return res;
}

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

int main() {
  set <pair<string, string>> fds = {{"A", "B"}, {"C", "DE"}, {"AC", "F"}};
  set <string> r1 = {"A", "B"};
  set <string> r2 = {"C", "D", "E"};
  set <string> r3 = {"A", "C", "F"};
  set <set<string>> decomposition = {r1, r2, r3};
  bool isPreserved = true;
  for(auto fd : fds) {
    string result = fd.first;
    string prevRes = "";
    while(result != prevRes) {
      for(auto ri : decomposition) {
        string temp = setIntersection(result, ri);
        string attrClosure = attributeClosure(fds, temp);
        prevRes = result;
        temp = setIntersection(attrClosure, ri);
        result = unionStrings(result, temp);
      }
    }
    if(!isSubset(fd.second, result)) {
      isPreserved = false;
    }
  }
  if(isPreserved) {
    cout << "The given decomposition PRESERVES FDs\n";
  } else {
    cout << "The given decomposition does NOT PRESERVES FDs\n";
  }
  
}



// Schema: A, B, C
// FDs: A->B, B->C
// R1: (A, C)
// R2: (B, C)
// NOT PRESERVING

// Schema: A, B, C, D, E, F
// FDs: A->B, C->DE, AC->F
// R1: (A, B)
// R2: (C, D, E)
// R3: (A, C, F)
// PRESERVING