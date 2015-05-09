#include "../Defines.h"

/* START SOLUTION */

// formula for the number of combinations of k items
// with item i having multiplicity n_i:
// (n_1 + 1)(n_2 + 1)...(n_k + 1) 

// fills cur with the next combination of size n, sorted asc
// set:   vector with all elements in set, sorted asc
// cur:   current combination (empty vector to start with)
// n:     size of combination (ignored if cur.size() != 0)
// returns: false when all combs of n elms have been
//      returned, true otherwise

// usage:
// vector<ll> set = ... // entire set of numbers
// vector<ll> cur;
// int r = ... // size of combs required
// while(next_comb(set, cur, r)) {
//   ... // process cur
// }

bool next_comb(const vll& set, vll& cur, int n) {
  if(cur.size() == 0) {
    cur.assign(set.begin(), set.begin() + n);
    return true;
  }
  
  int cur_index = cur.size() - 1,
      set_index = set.size() - 1;
  while(cur_index >= 0 && cur[cur_index] == set[set_index]){ 
    cur_index--;
    set_index--;
  }   
  
  if(cur_index < 0) {
    return false;
  }
  else {
    int new_index = upper_bound(set.begin(), set.end(),
      cur[cur_index]) - set.begin();
    
    for(; cur_index < cur.size(); cur_index++, new_index++)
      cur[cur_index] = set[new_index]; 
      
    return true;
  }
}

// loops through combinations of all sizes, EXCEPT 0 
bool next_comb(const vll& set, vll& cur) {
  if(cur.size() == 0)
    return next_comb(set, cur, 1);
  
  const int size = cur.size();
  const bool res = next_comb(set, cur, size);
  
  if(size < set.size() && !res) {
    cur.clear();
    return next_comb(set, cur, size + 1);
  }
  else return res;
}

/* END SOLUTION */

int test() {
  ll test[] = {0, 0, 1, 1, 3, 3, 3, 4, 5, 5, 7};
  vll set(test, test + sizeof(test)/sizeof(test[0]));
  vll cur;
  
  // 432 all together, including null set
  // formula for k items with multiplicity n_i:
  // (n_1 + 1)(n_2 + 1)...(n_k + 1)
  
  int count = 1;
  while(next_comb(set, cur)) {
    for(int i = 0; i < cur.size(); i++)
      cout << cur[i] << " ";
      cout << endl;
      
    count++;
  }
  cout << count << endl;

  return 0;
}

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=&problem=1717
int solve_determine_comb() {
  while(!(cin>>ws).eof()) {
    string in;
    int r;
    vll vin, cur;

    cin >> in >> r;

    for(int i = 0; i < in.size(); ++i)
      vin.push_back(in[i]);

    sort(vin.begin(), vin.end());

    while(next_comb(vin, cur, r)) {
      for(int i = 0; i < r; ++i)
        cout << (char)cur[i];
      cout << endl;
    }
  }
  return 0;
}

int main() {
  return solve_determine_comb();
}
