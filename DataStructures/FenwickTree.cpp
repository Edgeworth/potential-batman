#include "../Defines.h"

/* START SOLUTION */

// 0-indexed
struct fenwick_tree {
    vll arr;
    fenwick_tree(int size) : arr(size) {}

    void adjust(int i, ll amt) {
        while (i < arr.size()) {
            arr[i] += amt;
            i |= (i + 1);
        }
    }

    void set(int i, ll amt) {
        ll oldamt = query(i, i);
        adjust(i, amt - oldamt);
    }

    // get sum in inclusive range
    ll query(int i, int j) {
        if(i == 0) {
            ll res = 0;
            while (j >= 0) {
                res += arr[j];
                j = (j & (j + 1)) - 1;
            }
            return res;
        }
        else return query(0, j) - query(0, i - 1);
    }
};

/* END SOLUTION */

// solves:
// http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=441&problem=3977
int solve_interval_product() {
    while(!(cin>>ws).eof()) {
        int N, K;
        cin >> N >> K;

        fenwick_tree ns(N), zs(N);
        for(int n = 0; n < N; ++n){
            ll j;
            cin >> j;
            zs.adjust(n, j == 0 ? 1 : 0);
            ns.adjust(n, j  < 0 ? 1 : 0);
        }

        for(int k = 0; k < K; ++k) {
            string s;
            int i, j;
            cin >> s >> i >> j;

            if(s[0] == 'C') {
                zs.set(i - 1, j == 0 ? 1 : 0);
                ns.set(i - 1, j  < 0 ? 1 : 0);
            }
            else {
                if(zs.query(i - 1, j - 1) > 0) cout << '0';
                else cout << (ns.query(i - 1, j - 1) % 2 == 0 ? '+' : '-');
            }
        }
        cout << "\n";
    }
    return 0;
}

int main() {
    return solve_interval_product();
}
