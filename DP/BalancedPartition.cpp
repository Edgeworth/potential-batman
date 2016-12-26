#include "../Defines.h"
//#include <fstream>

/* START SOLUTION */

// partitions a multiset of integers into two parts such
// that the difference between their sums is minimised
// dp[i][v] = "is there a subset S of the first i integers
//             such that sum(S) = v?"
// recurrence: "can I make v without item i, or make v-s[i]
//              without item i?"
// 
// s      : the set of integers to balance  
// sel    : a vector of bools wherein sel[i] == true iff
//          element i is included in the first part  
// returns: the minimum difference between partition sums
ll balanced_part(vl& s, vb& sel) {
    ll top = 0, bot = 0; // range must include 0 
    for(ll v : s) {
        if(v > 0) top += v;
        if(v < 0) bot += v;
    }

    vvb dp(s.size() + 1, vb(top - bot + 1));
    vvb ps(s.size() + 1, vb(top - bot + 1));

    // fill out dp table
    dp[0][0-bot] = true;
    for(int i = 1; i <= s.size(); ++i) {
        for(int v = bot; v <= top; ++v) {
            if(dp[i-1][v-bot]) {
                dp[i][v-bot] = true;
                ps[i][v-bot] = false;
            }

            if(v - s[i-1] >= bot && v - s[i-1] <= top &&
                    dp[i-1][v-s[i-1]-bot]) {
                dp[i][v-bot] = true;
                ps[i][v-bot] = true;
            }
        }
    }
    
    // sweep through to find best difference
    ll tot = top + bot;
    ll best, best_diff = LLONG_MAX;
    for(int v = bot; v <= top; ++v) {
        ll diff = abs(v - (tot - v));
        if(dp[s.size()][v-bot] && diff < best_diff) {
            best = v;
            best_diff = diff;
        }
    }

    // reconstruct optimal decomposition
    ll v = best;
    for(int i = s.size(); i > 0; --i) {
        if(!ps[i][v-bot]) continue;
        
        sel[i-1] = true;
        v -= s[i-1];
    }

    return best_diff;
}

/* END SOLUTION */

int simple_test() {
    vl s = {-1, -2, -3, -2, -1, -1};
    vb sel(6);
    cout << balanced_part(s, sel) << endl;
    for(bool b : sel) cout << (b ? 't' : 'f') << " ";
    cout << endl;

    return 0;
}

// solves: http://orac.amt.edu.au/cgi-bin/train/problem.pl?problemid=33 
int solve_aeroplanes() {
    ifstream in("balin.txt");
    ofstream out("balout.txt");

    int n;
    in >> n;

    vl s(n);
    vb sel(n);
    for(ll& v : s) in >> v;
    
    balanced_part(s, sel);

    bool empty = true;
    for(int i = 0; i < n; ++i) {
        if(sel[i]) {
            out << i + 1 << " ";
            empty = false;
        }
    }
    if(empty) out << 0;
    out << endl;

    empty = true;
    for(int i = 0; i < n; ++i) {
        if(!sel[i]) {
            out << i + 1 << " ";
            empty = false;
        }
    }
    if(empty) out << 0;
    out << endl;

    return 0;
}

int main() {
    return solve_aeroplanes();
}
