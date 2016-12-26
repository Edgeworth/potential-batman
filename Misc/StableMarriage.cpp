#include "Defines.h"

/* START SOLUTION */

// calculates the "male optimal" stable marriage (i.e. there
// is no stable marriage where any man prefers his match
// over the one given here) a female optimal matching can be
// found by swapping men and women
// mpref: vector i is the list of i's preferred partners, in
//    *decreasing* order of preference (0 indexed)
// fpref: as above, but for women
// match: a vector (passed in with any size) filled so that
//    position i gives the man matched with woman i
void stable_marriage(const vvi& mpref, const vvi& fpref,
           vi& match) {
  // initially no one matched
  match.resize(mpref.size(), -1); 

  // get a map from (w, m) to w's "rank" for m
  vvi fmap(match.size(), vi(match.size()));
  for(int i = 0; i < match.size(); ++i)
    for(int j = 0; j < match.size(); ++j)
      fmap[i][fpref[i][j]] = j;

  vi next_prop(match.size(), 0);
  queue<int> mfree;
  for(int i = 0; i < match.size(); ++i)
    mfree.push(i);

  while(!mfree.empty()) {
    const int m = mfree.front();
    const int w = mpref[m][next_prop[m]];
    mfree.pop();

    if(match[w] == -1) {
      match[w] = m;
    }
    else if(fmap[w][match[w]] > fmap[w][m]) {
      mfree.push(match[w]);
      match[w] = m;
    }
    else if(++next_prop[m] < match.size()) {
      mfree.push(m);
    }
  }
}

/* END SOLUTION */

// solves https://icpcarchive.ecs.baylor.edu/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=1838
int solve_sm() {
  int T;
  cin >> T;

  for(int t = 0; t < T; ++t) {
    int N;
    cin >> N;

    map<char, int> names[2];
    map<int, char> inames[2];
    for(int j = 0; j < 2; ++j) {
      for(int n = 0; n < N; ++n) {
        string name;
        cin >> name;

        const int i = names[j].size();
        names[j][name[0]] = i;
        inames[j][i] = name[0];
      }
    }

    vvi prefs[2] = {vvi(N), vvi(N)};
    for(int j = 0; j < 2; ++j) {
      for(int n = 0; n < N; ++n) {
        string line;
        cin >> line;

        const int p1 = names[j][line[0]];  
        for(int p2 = 0; p2 < N; ++p2)
          prefs[j][p1].push_back(names[1-j][line[2+p2]]);
      }
    }
    
    vi match;
    vector<pair<char, char>> pairs;
    stable_marriage(prefs[0], prefs[1], match);
    for(int i = 0; i < N; ++i) 
      pairs.push_back(make_pair(inames[0][match[i]], inames[1][i]));
    sort(pairs.begin(), pairs.end());

    if(t != 0)
      cout << endl;
    for(int i = 0; i < N; ++i) {
      cout << pairs[i].first << " " << pairs[i].second << endl;
    }
  }
  return 0;
}

// solves http://www.spoj.com/problems/STABLEMP/
int solve_stablemp() {
  int T;
  cin >> T;

  for(int t = 0; t < T; ++t) {
    int N;
    cin >> N;

    vvi prefs[2] = {vvi(N), vvi(N)};
    for(int j = 0; j < 2; ++j) {
      for(int i = 0; i < N; ++i) {
        int p;
        cin >> p; // throw away line #

        for(int k = 0; k < N; ++k) {
          cin >> p;
          prefs[j][i].push_back(p-1);
        }
      }
    }

    vi match;
    stable_marriage(prefs[1], prefs[0], match);

    for(int i = 0; i < N; ++i)
      cout << match[i]+1 << " " << i+1 << endl;
  }
  return 0;
}

int main() {
  return solve_stablemp();
}
