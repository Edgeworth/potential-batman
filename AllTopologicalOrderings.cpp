#include "Defines.h"

/* START SOLUTION */

// recursively calculates orderings in lex order
// g      : the adjacency list repn of an input DAC
// indegs : a precalculated list of vertex indegrees
// ord    : the current ordering
// tovis  : the set of vertices immediately visitable
// ords   : the vector to which possible orderings are
//          added (should be passed in empty)
// returns: the number of top orderings there are
ll top_orderings(const vvi& g, vi& indegs, vi& ord, 
                 const set<int>& tovis, vvi& ords) {
    if(ord.size() == g.size()) {
        // alternately: process ord here
        ords.push_back(ord);
        return 1;
    }
    
    ll count = 0;
    
    for(int c : tovis) {
        // choose to use c next
        ord.push_back(c);
        
        set<int> tovis2 = tovis;
        tovis2.erase(c);

        for(int n : g[c])
            if(--indegs[n] == 0)
                tovis2.insert(n);

        count += top_orderings(g, indegs, ord, 
                               tovis2, ords);

        // undo choice
        for(int n : g[c])
            ++indegs[n];
        
        ord.pop_back();
    }

    return count;
}

// precompute necessary info and call helper
ll top_orderings(const vvi& g, vvi& ords) {
    vi indegs(g.size()), ord;
    set<int> tovis;

    for(const vi& ns : g)
        for(int n : ns)
            ++indegs[n];

    for(int c = 0; c < g.size(); ++c)
        if(indegs[c] == 0)
            tovis.insert(c);

    return top_orderings(g, indegs, ord, tovis, ords);
}

/* END SOLUTION */

// solves http://uva.onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=60
int solve_following_orders() {
    bool first = true;
    while(!cin.eof()) {
        string line;
        getline(cin, line);

        set<char> vars;
        map<char, int> idxs;
        map<int, char> names;
        iss ss1(line);
        
        // force vars to be lex sorted
        while(!(ss1>>ws).eof()) {
            char c;
            ss1 >> c;
            vars.insert(c);
        }

        for(char c : vars) {
            const int sz = idxs.size();
            idxs[c] = sz;
            names[sz] = c;
        }

        vvi g(idxs.size()), ords;

        getline(cin, line);
        iss ss2(line);
        while(!(ss2>>ws).eof()) {
            char c1, c2;
            ss2 >> c1 >> ws >> c2;

            g[idxs[c1]].push_back(idxs[c2]);
        }

        if(!first)
            cout << endl;
        top_orderings(g, ords);

        for(vi& ord : ords) {
            for(int c : ord)
                cout << names[c];
            cout << endl;
        }

        first = false;
        cin.peek();
    }
}

int main() {
    return solve_following_orders();
}
