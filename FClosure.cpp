#include <iostream>
#include <string.h>
#include <vector>
#include <set>
using namespace std;

set<string> generateSubsets(string str, int i) {
  set <string> res;
  if(i == -1) {
    return {};
  }
  set<string> left = generateSubsets(str, i-1);
  set<string> right = generateSubsets(str, i-1);
  string temp = "";
  temp += str[i];
  res = left;
  if(right.size() == 0) {
    res.insert(temp);
  } else {
    for(auto x : right) {
      res.insert(x + temp);
    }
    res.insert(temp);
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
  set <pair<string, string>> fds = {{"A", "B"}, {"B", "C"}, {"A", "C"}};
  set <string> schema = {"A", "B", "C", "D"};
  set <pair<string, string>> fClosure = fds;
  set <pair<string, string>> prevRes = {};
  string combinedAttributes = "";
  for(auto i : schema) {
    combinedAttributes += i;
  }
  set <string> attributeSubset = generateSubsets(combinedAttributes, combinedAttributes.length()-1);
  while(fClosure != prevRes) {
    prevRes = fClosure;
    for(auto fd : fClosure) {
      set<string> subsets = generateSubsets(fd.first, fd.first.length()-1);
      // Applying reflexive rule to generate trivial FDs.
      for(auto subset : subsets) {
        pair<string, string> temp;
        temp.first = fd.first;
        temp.second = subset;
        // prevRes = fClosure;
        fClosure.insert(temp);
      }
    }
    for(auto i : schema) {
      fClosure.insert({i, i});
    }

    // Applying Augumentation using given schema attribute set
    set <pair<string, string>> augumentedFds;
    for(auto fd : fClosure) {
      pair <string, string> temp;
      for(auto aug : attributeSubset) {
        temp.first = unionStrings(aug, fd.first);
        temp.second = unionStrings(aug, fd.second);
        augumentedFds.insert(temp);
      }
    }
    for(auto i : augumentedFds){
      // prevRes = fClosure;
      fClosure.insert(i);
    }
    // Applying transitivity rule to FDs.
    for(auto fd1 : fClosure) {
      for(auto fd2 : fClosure) {
        if(fd1.second == fd2.first) {
          pair<string, string> temp;
          temp.first = fd1.first;
          temp.second = fd2.second;
          // prevRes = fClosure;
          fClosure.insert(temp);
        }
      }
    }
  }
  cout << fClosure.size() << "\n";
  for(auto i : fClosure) {
    cout << i.first << "->" << i.second << "\n";
  }
  
}
