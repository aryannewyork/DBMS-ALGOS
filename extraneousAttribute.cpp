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

string removeAttribute(string attribute, char attrToRemove) {
  string res = "";
  for(auto i : attribute) {
    if(i != attrToRemove) {
      res += i;
    }
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

set <pair<string, string>> removeExtraneousAttributes(set<pair<string, string>> fds) {
  set <pair<string, string>> fdsReduced = fds;
  set <pair<string, string>> prevRes = {};
  while(prevRes != fdsReduced) {
    prevRes = fdsReduced;
    set <pair<string, string>>::iterator fd;
    for(fd = fdsReduced.begin(); fd != fdsReduced.end(); fd++) {
      string a = (*fd).first;
      for(auto i : a) {
        string y = removeAttribute(a, i);
        string yClosure = attributeClosure(fdsReduced, y);
        string b = (*fd).second;
        if(isSubset(b, yClosure)) {
          cout << i << " is Extraneous in " << (*fd).first << "->" << (*fd).second << "\n";
          pair<string, string> temp2;
          temp2.first = y;
          temp2.second = b;
          fdsReduced.insert(temp2);
          fd = fdsReduced.erase(fd);
          break;
        }
      }
    }

    // Check the extraneous-ity of all the attributes on RHS of FDs.
    for(fd = fdsReduced.begin(); fd != fdsReduced.end(); fd++) {
      string b = (*fd).second;
      if(b.length() == 1) {
        continue;
      }
      for(auto &i : b) {
        pair<string, string> tempFd;
        tempFd.first = (*fd).first;
        tempFd.second = removeAttribute(b, i);
        fdsReduced.insert(tempFd);
        fd = fdsReduced.erase(fd);
        string x = (*fd).first;
        string aClosure = attributeClosure(fdsReduced, x);
        string istr(1, i);
        if(isSubset(istr, aClosure)) {
          cout << i << " is Extraneous in " << (*fd).first << "->" << b << "\n";
          break;
        } else {
          pair<string, string> temp2;
          temp2.first = (*fd).first;
          temp2.second = b;
          fd = fdsReduced.erase(fd);
        }
      }
    }
  }
  return fdsReduced;
}

int main() {
  set <string> schema = {"A", "B", "C", "D", "E", "F"};
  set <pair<string, string>> fds = {{"AB", "C"}, {"ACD", "B"}, {"BC", "D"}, {"BE", "C"}, {"C", "A"}, {"CE", "A"}, {"CE", "F"}, {"CF", "B"}, {"CF", "D"}, {"CF", "D"}, {"D", "E"}, {"D", "F"}};
  // Check the extraneous-ity of all the attributes on LHS of FDs.
  
  set<pair<string, string>> fdsReduced = removeExtraneousAttributes(fds);
  cout << "Previous FD set: ";
  for(auto x : fds) { 
    cout << x.first << "->" << x.second << ", ";
  }
  // using non-destructive backspace to replace last ", " with "  ";
  cout << "\b\b \n";
  cout << "Reduced FD set: ";
  for(auto x : fdsReduced) { 
    cout << x.first << "->" << x.second << ", ";
  }
  cout << "\b\b \n";

}


// Schema: A, B, C, D, E
// FDs: AB->CD, A->E, E->C
// to check if C is extraneous

// Example 1
// {"A", "B", "C", "D", "E"}
// {{"AB", "CD"}, {"A", "E"}, {"E", "C"}}
// C is Extraneous in AB->CD


// Example 2
// {"A", "B", "C"}
// {{"A", "C"}, {"AB", "C"}}
// B is Extraneous in AB->C

// Example 3
// {"A", "B", "C", "D", "E", "F"}
// {{"AB", "C"}, {"ACD", "B"}, {"BC", "D"}, {"BE", "C"}, {"C", "A"}, {"CE", "A"}, {"CE", "F"}, {"CF", "B"}, {"CF", "D"}, {"CF", "D"}, {"D", "E"}, {"D", "F"}}