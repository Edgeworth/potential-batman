#include "Defines.h"

/* START SOLUTION */

void kmp(const string& T,
    const string& P,
    vi& matches) {
  vi lookup(P.size(), 0);
  size_t p_pos = 1;
  size_t lookup_pos = 0;
  while (p_pos < P.size()) {
    if (P[p_pos] == P[lookup_pos]) {
      lookup[p_pos] = lookup_pos + 1;
      p_pos++;
      lookup_pos++;
    } else if (lookup_pos > 0) {
      lookup_pos = lookup[lookup_pos - 1];
    } else {
      lookup[p_pos] = lookup_pos = P[p_pos] == P[0];
      p_pos++;
    }
  }

  p_pos = 0;
  size_t t_pos = 0;
  while (t_pos + p_pos < T.size()) {
    bool matched = T[t_pos + p_pos] == P[p_pos];
    if (matched) {
      p_pos++;
      if (p_pos == P.size()) {
        matches.push_back(t_pos);
      }
    }

    if (!matched || p_pos == P.size()) {
      if (p_pos == 0) {
        t_pos++;
      } else {
        int advance = p_pos - lookup[p_pos - 1];
        t_pos += advance;
        p_pos -= advance;
      }
    }
  }
}

/* END SOLUTION */

int main() {
	vi matches;
	kmp("test test test", "test", matches);
	for (auto i : matches) {
		printf("%d\n", i);
	}
}

