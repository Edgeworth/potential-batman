#include "Defines.h"

// returns true if s is an operator or paren.
bool is_op(string s) {
	return s.size() == 1 && !isdigit(s[0]);
}

// tokens: a list of tokens (each an int or operator)
// ops: a map of operators to (precedence, is left associative)
vector<string> infix_to_postfix(vs &tokens, map<char, pair<int, bool>> &ops) {
	vs out;
	stack<char> s;

	for (string t : tokens) {
		cerr << "token " << t << endl;
		if (is_op(t)) {
			if (t[0] == '(') {
				cerr << "  (" << endl;
				s.push('(');
			} else if (t[0] == ')') {
				cerr << "  )" << endl;
				// if this block encounters an empty stack, there are mismatched parens
				while (s.top() != '(') {
					out.push_back(string(1, s.top()));
					s.pop();
				}
				s.pop(); // remove the open paren
			} else { // proper operator
				cerr << "  op" << endl;
				while (!s.empty()) {
					cerr << "    looking at s.top()" << s.top() << endl;
					pair<int, bool> o1 = ops[t[0]], o2 = ops[s.top()];
					if ((o1.first < o2.first) || (o1.second && (o1.first == o2.first))) {
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
		// if this block encounters an open paren, then there are mismatched parens
		out.push_back(string(1, s.top()));
		s.pop();
	}

	return out;
}

// evaluates a postfix expression with operators + - * / ^
int evaluate(vs &postfix) {
	stack<int> s;
	for (string t : postfix) {
		if (is_op(t)) {
			int rhs = s.top(); s.pop();
			int lhs = s.top(); s.pop();
			switch(t[0]) {
				case '+': s.push(lhs + rhs); break;
				case '-': s.push(lhs - rhs); break;
				case '*': s.push(lhs * rhs); break;
				case '/': s.push(lhs / rhs); break;
				case '^': s.push(pow(lhs, rhs)); break;
			}
		} else { // number
			s.push(parseInt(t));
		}
	}
}


int main() {
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
	
	cout << endl;
}
