#include "Defines.h"

/* START SOLUTION */

// coins: a list of the values of allowed coins
// val: the target value to be constructed
// returns: the number of ways of making val cents from the
// given coins runs in O(nV) time, where n is the number of
// coins, and V is the target value
// subproblem: how many ways of making v using coins of
// value c or less?
// recurrence: every way either uses coin c or does not
ll coin_change(const vl& coins, ll val) {
    vvl dp(val + 1, vl(coins.size(), 0));
    
    for(int c = 0; c < coins.size(); ++c)
        dp[0][c] = 1;

    for(int v = 1; v <= val; ++v) {
        for(int c = 0; c < coins.size(); ++c) {
            if(c > 0)
                dp[v][c] += dp[v][c-1];
            if(v - coins[c] >= 0) 
                dp[v][c] += dp[v-coins[c]][c];
        }
    }

    return dp[val][coins.size()-1];
}

/* END SOLUTION */

// attemps to solve http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=615
// times out, but probably due to broken uva servers
// correctly solves all input given in help thread http://online-judge.uva.es/board/viewtopic.php?f=7&t=2533
int solve1() {
    vl coins = {1, 5, 10, 25, 50};
    while(!(cin>>ws).eof()) {
        int val;
        cin >> val;
        cout << coin_change(coins, val) << endl;
    }
    return 0;
}

int main() {
    return solve1();
}
