#include "Defines.h"

/* START SOLUTION */

vi induce(
    const vi& bucketStart, const vi& bucketEnd,
    const vi& s, const vector<bool>& t,
    const vi& lms) {
  vi induced(s.size(), -1);

  vi bucketEndCopy = bucketEnd;
  for (int i = lms.size() - 1; i >= 0; --i) {
    induced[bucketEndCopy[s[lms[i]]]--] = lms[i];
  }

  vi bucketStartCopy = bucketStart;
  bucketEndCopy = bucketEnd;
  for (int i = 0; i < induced.size(); ++i) {
    if (induced[i] > 0 && !t[induced[i] - 1]) {
      induced[bucketStartCopy[s[induced[i] - 1]]++] =
        induced[i] - 1;
    }
  }

  for (int i = induced.size() - 1; i >= 0; --i) {
    if (induced[i] > 0 && t[induced[i] - 1]) {
      induced[bucketEndCopy[s[induced[i] - 1]]--] =
        induced[i] - 1;
    }
  }

  return induced;
}

// NOTE MUST END IN A 0 THAT IS THE LEX SMALLEST SYMBOL
vi sais(const vi& s) {
  if (s.size() == 1) return {0};

  vector<bool> t(s.size(), false);
  vi lms;
  t[s.size() - 1] = true;
  int maxChar = 0;
  for (int i = s.size() - 2; i >= 0; --i) {
    t[i] = (s[i] < s[i + 1]) ||
      ((s[i] == s[i + 1]) && t[i + 1]);
    if (s[i] >= maxChar)
      maxChar = s[i] + 1;

    if (!t[i] && t[i + 1])
      lms.push_back(i + 1);
  }

  vi charCount(maxChar, 0);
  for (int i = 0; i < s.size(); ++i) {
    charCount[s[i]]++;
  }

  vi bucketStart(maxChar, 0);
  vi bucketEnd(maxChar, 0);
  int sum = 0;
  for (int i = 0; i < maxChar; ++i) {
    bucketStart[i] = sum;
    sum += charCount[i];
    bucketEnd[i] = sum - 1;
  }

  vi inducedSorted = induce(
      bucketStart, bucketEnd, s, t, lms);
  vi lmsMap(s.size());
  int curPartition = 0;
  int prev = -1;
  for (int i = 0; i < inducedSorted.size(); ++i) {
    int cur = inducedSorted[i];
    if (cur > 0 && t[cur] && !t[cur - 1]) {
      if (prev != -1) {
        bool prevPastS = false;
        bool curPastS = false;
        for (int j = 0; true; ++j) {
          if (!t[prev + j]) prevPastS = true;
          if (!t[cur + j]) curPastS = true;

          if (s[prev + j] != s[cur + j] ||
          t[prev + j] != t[cur + j]) {
            curPartition++;
            break;
          }

          if (t[prev + j] && t[cur + j] &&
          prevPastS && curPastS) break;
        }
      }
      lmsMap[cur] = curPartition;
      prev = cur;
    }
  }

  vi s1(lms.size(), 0);
  for (int i = 0; i < lms.size(); ++i) {
    s1[i] = lmsMap[lms[lms.size() - i - 1]];
  }

  vi lmsSA = sais(s1);
  vi lmsSAIndices(lmsSA.size(), 0);
  for (int i = 0; i < lmsSA.size(); ++i) {
    lmsSAIndices[i] = lms[lms.size() - lmsSA[i] - 1];
  }
  return induce(
      bucketStart, bucketEnd, s, t, lmsSAIndices);
}

// From Max:
void computeLCP() {
  int i, L;
  Phi[SA[0]] = -1;                                         // default value
  for (i = 1; i < n; i++)                            // compute Phi in O(n)
    Phi[SA[i]] = SA[i-1];    // remember which suffix is behind this suffix
  for (i = L = 0; i < n; i++) {             // compute Permuted LCP in O(n)
    if (Phi[i] == -1) { PLCP[i] = 0; continue; }            // special case
    while (T[i + L] == T[Phi[i] + L]) L++;       // L increased max n times
    PLCP[i] = L;
    L = max(L-1, 0);                             // L decreased max n times
  }
  for (i = 0; i < n; i++)                            // compute LCP in O(n)
    LCP[i] = PLCP[SA[i]];   // put the permuted LCP to the correct position
}

/* END SOLUTION */

void test() {
  string test;
  cin >> test;
  test.push_back(0);
  vi testIn(test.begin(), test.end());
  vi out = sais(testIn);

  vector<string> output;
  for (auto i : out) {
    output.push_back(test.substr(i, test.size() - i));
    printf("%s\n", output.back().c_str());
  }

  vector<string> copy = output;
  sort(copy.begin(), copy.end());
  if (output != copy) {
    printf("DIFFERENT!\n");
  } else {
    printf("SAME!\n");
  }
}

int main() {
  test();
}
