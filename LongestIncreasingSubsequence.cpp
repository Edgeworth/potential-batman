#include "Defines.h"

/* START SOLUTION */

// runs in O(n lg n) time, where n is the length of in
// in: the input sequence
// liss   : a vector (which can be passed in as any size) which will be
//          set to contain the length of the lis ending at each index
// parents: a vector (which can be passed in as any size) which will be
//          set to contain the index of the penultumate element in the
//          lis ending at each index
// returns: the length of the lis in in
int lis(const vll& in, vi& liss, vi& parents) {
    // position j gives index of the last element in "best" known lis
    // of length j (where best means ends in smallest element)
    vi length_idx(in.size() + 1, -1);
    // position i gives the immediate predecessor of element i in best
    // known lis
    parents.resize(in.size(), -1);
    // gives the length of the best known lis ending in pos i 
    liss.resize(in.size(), -1);

    // best known length
    int L = 0;

    for(int i = 0; i < in.size(); ++i) {
        // binary search for largest pos j <= L s.t.
        // in[length_idx[j]] < in[i]
        // i.e. the lis we can append in[i] onto
        int low = 1, high = L;
        while(low <= high) {
            int mid = ceil((low+high)/2);

            if(in[length_idx[mid]] < in[i])
                low = mid+1;
            else
                high = mid-1;
        }

        // low is length of lis ending in in[i]
        parents[i] = length_idx[low-1];
        length_idx[low] = i;
        liss[i] = low;

        // update best known length
        L = max(L, low);
    }

    return L;
}

// print lis ending at position index
void print_lis(const vll& in, const vi& parents, int index) {
    vi idx_seq;
    for(int i = index; i >= 0; i = parents[i])
        idx_seq.push_back(i);
    reverse(idx_seq.begin(), idx_seq.end());

    for(int i = 0; i < idx_seq.size(); ++i)
        cout << in[idx_seq[i]] << " ";
}

/* END SOLUTION */

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&category=&problem=1475
int solve_wavio() {
    while(!(cin>>ws).eof()) {
        int N;
        cin >> N;

        vll in(N), rin(N);
        for(int n = 0; n < N; ++n) {
            cin >> in[n];
            rin[N-1-n] = in[n];
        }

        vi liss, rliss, p, rp;
        lis(in, liss, p);
        lis(rin, rliss, rp);

        int ma = 0;
        for(int n = 0; n < N; ++n) {
            int cur = min(liss[n], rliss[N-n-1]);
            ma = max(ma, 2 * cur - 1);
        }

        cout << ma << endl;
    }
    return 0;
}

int main() {
    return solve_wavio();
}
