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
  set <pair<string, string>> fds = {{"AC","G"}, {"D", "EG"}, {"BC", "D"}, {"CG", "BD"}, {"ACD", "B"}, {"CE", "AG"}};
  set <string> schema = {"A", "B", "C", "D", "E", "F", "G", "H"};
  set <pair<string, string>> fc = fds;
  set <pair<string, string>> prevRes = {};
  while(prevRes != fc) {
    prevRes = fc;
    set <pair<string, string>>::iterator fd1;
    set <pair<string, string>>::iterator fd2;
    for(fd1 = fc.begin(); fd1 != fc.end(); fd1++) {
      bool flag = false;
      for(fd2 = fc.begin(); fd2 != fc.end(); fd2++) {
        cout << "out: " << (*fd1).first << " " << (*fd2).first << "\n";
        if((*fd1 != *fd2 && (*fd1).first == (*fd2).first)){
          cout << "in: " << (*fd1).first << " " << (*fd2).first << "\n";
          pair<string, string> temp;
          temp.first = (*fd1).first;
          temp.second = unionStrings((*fd1).second, (*fd2).second);
          fc.insert(temp);
          fd1 = fc.erase(fd1);
          fd2 = fc.erase(fd2);
          flag = true;
          break;
        }
      }
      if(flag){
          break;
      }
    }
    for(auto p : fc) {
      cout << p.first << "->" << p.second << ", ";
    }
    cout << "\b\b \n";
    fc = removeExtraneousAttributes(fc);
    cout << "DONE..\n";
    for(auto p : fc) {
      cout << p.first << "->" << p.second << ", ";
    }
    cout << "\b\b \n";

  }
  cout << fc.size() << "\n";
  for(auto x : fc) {
    cout << x.first << "->" << x.second << "\n";
  }
}



// A, B, C, D, E, F, G, H
// FD: AC->G, D->EG, BC->D, CG->BD, ACD->B, CE->AG
// FC: AC->B, D->EG, BC->D, CG->D, CE->A