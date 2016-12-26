#include "../Defines.h"

/* START SOLUTION */

// uses the formula:
// p(n) = 0 for n < 0
// p(n) = Sum_{k != 0}( (-1)^k p(n - g_k) ), where
// k can be negative (!) and g_k = k(3k-1)/2 is the kth
// generalised pentagonal number
// O(n^1.5) on MAX_PART
const int MAX_PART = 1000;
ll NUM_PARTS[MAX_PART+1];

ll calc_parts() {
    NUM_PARTS[0] = 1;
    NUM_PARTS[1] = 1;
    for(int i = 2; i < MAX_PART; ++i) {
        NUM_PARTS[i] = 0;
        for(int k = 1, pent_num = 1; i >= pent_num;
            k = k<0?-k+1:-k, pent_num = k*(3*k-1)/2) {
            NUM_PARTS[i] +=
                (k%2?1:-1) * NUM_PARTS[i-pent_num];
        }
    }
}

/* END SOLUTION */

int main() {
    calc_parts();

    for(int i = 0; i < 25; ++i) 
        cout << NUM_PARTS[i] << " ";
    cout << endl;
}
