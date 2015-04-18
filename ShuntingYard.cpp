#include "Defines.h"

/* START SOLUTION */

// returns true if s is an operator or paren.
bool is_op(string s) {
  return s.size() == 1 && !isdigit(s[0]) && !isalpha(s[0]);
}

// tokens: a list of tokens (each an int or operator)
// ops: a map of operators to (precedence, is left assoc)
vector<string> infix_to_postfix(vs &tokens, 
    map<char, pair<int, bool>> &ops) {
  vs out;
  stack<char> s;

  for (string t : tokens) {
    if (is_op(t)) {
      if (t[0] == '(') {
        s.push('(');
      } else if (t[0] == ')') {
        // if this block encounters an empty stack,
        // there are mismatched parens
        while (s.top() != '(') {
          out.push_back(string(1, s.top()));
          s.pop();
        }
        s.pop(); // remove the open paren
      } else { // proper operator
        while (!s.empty()) {
          pair<int, bool> o1 = ops[t[0]], o2 = ops[s.top()];
          if ((o1.first < o2.first) ||
              (o1.second && (o1.first == o2.first))) {
            out.push_back(string(1, s.top()));
            s.pop();
          }
          else break;
        }
        s.push(t[0]);
      }
    } else { // number
      out.push_back(t);
    }
  }
  while (!s.empty()) {
    // if this block encounters an open paren, then there
    // are mismatched parens
    out.push_back(string(1, s.top()));
    s.pop();
  }
  return out;
}

/* END SOLUTION */

// evaluates a postfix expression with operators + - * / ^
double evaluate_postfix(vs &postfix) {
  stack<double> s;
  for (string t : postfix) {
    if (is_op(t)) {
      double rhs = s.top(); s.pop();
      double lhs = s.top(); s.pop();
      switch(t[0]) {
        case '+': s.push(lhs + rhs); break;
        case '-': s.push(lhs - rhs); break;
        case '*': s.push(lhs * rhs); break;
        case '/': s.push(lhs / rhs); break;
        case '^': s.push(pow(lhs, rhs)); break;
      }
    } else { // number
      iss ss(t);
      double i;
      ss >> i;
      s.push(i);
    }
  }
  return s.top();
}

// http://uva.onlinejudge.org/external/7/727.html
int solve_equation() {
  map<char, pair<int, bool>> ops = {
    {'-', {1, true}},
    {'+', {1, true}},
    {'/', {4, true}},
    {'*', {4, true}},
  };

  int T;
  cin >> T >> ws;
  for (int t = 0; t < T; t++) {
    if (t) cout << endl;

    vs q;
    while (!cin.eof()) {
      string l;
      getline(cin, l);
      if (!l.size()) break;

      q.push_back(l);
    }
    for (string s : infix_to_postfix(q, ops)) cout << s;
    cout << endl;
  }

  return 0;
}

// http://uva.onlinejudge.org/external/1/172.html
int solve_calculator_language() {
  map<char, pair<int, bool>> ops = {
    {'-', {1, false}},
    {'+', {1, false}},
    {'/', {1, false}},
    {'*', {1, false}},
    {'=', {1, false}},
  };

  int vars[26] = {0};

  while (!cin.eof()) {
    string l;
    getline(cin, l);
    if (l[0] == '#') return 0;

    // pad all operators with spaces and replace underscores
    // with neg signs.
    string ll;
    for (char c : l) {
      if (ops.count(c) || c == '(' || c == ')') {
        ll.push_back(' ');
        ll.push_back(c);
        ll.push_back(' ');
      } else {
        if (c == '_') ll.push_back('-');
        else ll.push_back(c);
      }
    }

    // pull the tokens into an vector
    vs infix;
    iss ss(ll);
    while (!ss.eof()) {
      string s;
      ss >> s >> ws;
      infix.push_back(s);
    }

    vs post = infix_to_postfix(infix, ops);

    for (string s : post) cerr << s << " ";
    cerr << endl;

    // evaluate the postfix
    int old_vars[26];
    for (int i = 0; i < 26; i++) old_vars[i] = vars[i];
    // pair is <var, val>: var==-1 means val is a number 
    stack<pair<int, int>> s; 
    for (string t : post) {
      if (is_op(t)) {
        int rhs = s.top().first >= 0 ? vars[s.top().first] :
          s.top().second; s.pop();
        int lhs_v = s.top().first;
        int lhs = s.top().first >= 0 ? vars[s.top().first] :
          s.top().second; s.pop();
        switch(t[0]) {
          case '+': s.push(make_pair(-1, lhs + rhs)); break;
          case '-': s.push(make_pair(-1, lhs - rhs)); break;
          case '*': s.push(make_pair(-1, lhs * rhs)); break;
          case '/': s.push(make_pair(-1, lhs / rhs)); break;
          case '=':
            vars[lhs_v] = rhs;
            s.push(make_pair(lhs_v, -1));
          break;
        }
      } else { // number or var TODO
        if (isupper(t[0])) s.push(make_pair(t[0]-'A', -1));
        else {
          iss ss(t);
          int i;
          ss >> i;
          s.push(make_pair(-1, i));
        }
      }
    }

    bool first = true;
    for (int i = 0; i < 26; i++) {
      if (old_vars[i] != vars[i]) {
        if (!first) cout << ", ";
        first = false;
        cout << (char)('A' + i) << " = " << vars[i];
      }
    }
    if (first) cout << "No Change";
    cout << endl;
  }

  return 0;
}

int simple_test() {
  map<char, pair<int, bool>> ops = {
    {'-', {1, true}},
    {'+', {2, true}},
    {'/', {3, true}},
    {'*', {4, true}},
    {'^', {5, false}}
  };

  string infix = "3 + 4 * 2 / ( 1 - 5 ) ^ 2 ^ 3";

  vs in;
  stringstream ss(infix);
  while (!(ss >> ws).eof()) {
    string s;
    ss >> s;
    in.push_back(s);
  }

  vs post = infix_to_postfix(in, ops);

  cout << "infix:     ";
  for (string s : in) cout << s << " ";
  cout << endl;

  cout << "postfix:   ";
  for (string s : post) cout << s << " ";
  cout << endl;

  cout << "evaluated: ";
  cout << evaluate_postfix(post);
  cout << endl;

  return 0;
}

int main() {
  return solve_calculator_language();
  return solve_equation();
  return simple_test();
}
