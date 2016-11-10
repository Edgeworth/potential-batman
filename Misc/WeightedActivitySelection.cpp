// From Max
/* START SOLUTION */
struct Interval {
  int left, right;
  double weight;
  bool operator< (const Interval& other) const
  {
    return right < other.right;
  }
};


vector<int> weighted_activity_selection(vector<Interval>& intervals) {
  //sort by right end point
  sort( intervals.begin(), intervals.end() );


  vector<int> q(intervals.size()+1, 0);
  //compute q values
  for(int i = 1; i < intervals.size(); ++i) {
    //binary search for last compatible index
    int l = 0, r = i-1, mid;
    while(l <= r) {
      mid = l + (r-l)/2;
      if(intervals[mid].right < intervals[i].left &&
          !(intervals[mid+1].right < intervals[i].left)) {
        q[i+1] = mid+1;
        break;
      }
      else if(intervals[i].left > intervals[mid].right)
        l = mid+1;
      else
        r = mid-1;
    }
  }


  vector<double> dp(intervals.size()+1);
  //bottom up dp fill
  dp[0] = 0;
  for(int i = 1; i < dp.size(); ++i) {
    double iScore = intervals[i-1].weight + dp[q[i]];
    if(iScore > dp[i-1])
      dp[i] = iScore;
    else
      dp[i] = dp[i-1];
  }

  //traceback
  int curr = intervals.size();
  vector<int> traced;
  while(curr > 0) {
    if (intervals[curr-1].weight + dp[q[curr]] > dp[curr-1])
    {
      traced.push_back(curr-1);
      curr = q[curr];
    }
    else
      --curr;
  }

  reverse(traced.begin(), traced.end());
  return traced;
}
/* END SOLUTION */
