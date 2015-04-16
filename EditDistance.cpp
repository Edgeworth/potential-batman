#include "Defines.h"

/* START SOLUTION */

// protip: remember that input strings might be blank, so read them in
// with getline rather than stream extraction

enum { REP = 0, ADD, REM };

// ts: a 2d vector of size u.size() + 1 by v.size() + 1
//     during execution of this function, the (i,j)th
//     entry of ts is set to the optimum operation used to
//     transform the length i prefix of u into the length j
//     prefix of v
// returns: the edit distance between u and v
int edit_distance(const string u, const string v, vvi& ts) {
    vvll dp(u.size() + 1, vll(v.size() + 1, LLONG_MAX)); 
    for(int j = 0; j <= v.size(); ++j) {
        dp[0][j] = j;
        ts[0][j] = ADD;
    }
    ts[0][0] = -1;
    
    // i represents index in u, j represents index in v
    for(int i = 0; i < u.size(); ++i) {
        dp[i+1][0] = i+1;
        ts[i+1][0] = REM;

        for(int j = 0; j < v.size(); ++j) {
            // replace u[i] with v[j]
            if(dp[i+1][j+1] > dp[i][j] + (u[i] != v[j] ? 1 : 0)) {
                dp[i+1][j+1] = dp[i][j] + (u[i] != v[j] ? 1 : 0);
                ts[i+1][j+1] = REP;
            }

            // add v[j] after u[i]
            if(dp[i+1][j+1] > dp[i+1][j] + 1) {
                dp[i+1][j+1] = dp[i+1][j] + 1;
                ts[i+1][j+1] = ADD;
            }

            // remove u[i]
            if(dp[i+1][j+1] > dp[i][j+1] + 1) {
                dp[i+1][j+1] = dp[i][j+1] + 1;
                ts[i+1][j+1] = REM;
            }
        }
    }

    return dp[u.size()][v.size()];
}

void print_transforms(const string u, const string v, const vvi& ts) {
    // how much to change i/j by after each op
    const int di[] = {-1, 0, -1}, dj[] = {-1, -1, 0};

    vector<string> to_print;

    int i = u.size(), j = v.size();
    while(i > 0 || j > 0) {
        ostringstream oss(ostringstream::ate);
        const int op = ts[i][j];

        if(op == REP) {
            // only print if something was actually replaced
            if(u[i-1] != v[j-1]) 
                oss << "Replace " << j << "," << v[j-1];
        }
        else if(op == ADD) {
            oss << "Insert " << j << "," << v[j-1];
        }
        else {
            oss << "Delete " << (j+1);
        }

        if(oss.str().size() > 0) 
            to_print.push_back(oss.str());

        i += di[op];
        j += dj[op];
    }

    for(int k = 1; k <= to_print.size(); ++k)
        cout << k << " " << to_print[to_print.size()-k] << endl;
}

/* END SOLUTION */

// solves: http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=467
int solve1() {
    bool first = true;
    while(!cin.eof()) {
        string u, v;
        getline(cin, u);
        getline(cin, v);
        vvi ts(u.size() + 1, vi(v.size() + 1, -1));
        if(!first)
            cout << endl;
        cout << edit_distance(u, v, ts) << endl;
        print_transforms(u, v, ts);
        first = false;
        cin.peek();
    }
    return 0;
}

// solves: http://www.spoj.com/problems/EDIST/
int solve2() {
    int T;
    cin >> T;
    for(int t = 0; t < T; ++t) {
        string u, v;
        cin >> u >> v;
        vvi ts(u.size() + 1, vi(v.size() + 1, -1));
        cout << edit_distance(u, v, ts) << endl;
    }
    return 0;
}

int main() {
    return solve1();
}
