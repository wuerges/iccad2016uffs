#pragma once

#include <map>
#include <vector>


namespace verilog {
namespace solution {
  using namespace std;

  typedef map<string, int > entropy_map;
  typedef map<string, bool > result_map;


  struct Match {
      map<string, vector<pair<string, bool> > > inputs; 
      map<string, vector<pair<string, bool> > > outputs; 
      vector<string> zeroes;
  };


  struct Count {
    Count(): name(""), count(0), value(false) {}
    Count(string n, int c, bool b): name(n), count(c), value(b) {}
    string name;
    int count;
    bool value;

    bool operator<(const Count & o) const {
      return count < o.count;
    }

    friend int max_count(vector<Count> & v) {
      int max = 0;
      for(auto c : v) {
        if(c.count > max)
          max = c.count;
      }
      return max;
    }
  };
  typedef pair<int, int> Segment;



  void get_distances(vector<Count> & counts, vector<pair<int, Segment> > & ds) {
    for(int i = 0; i < counts.size() - 1; ++i) {

      Segment s = make_pair(i, i+1);
      int m = counts[i].count - counts[i+1].count;
      if (m < 0) m = -m;

      ds.push_back(make_pair(m, s));
      sort(ds.begin(), ds.end());
    }

  };

  void group_counts(vector<Count> & counts, vector<vector<Count> > & groups, int n) {
    vector<pair<int, Segment> > ds;
    get_distances(counts, ds);

    vector<int> sets;
    for(int i = 0; i < counts.size(); ++i) {
      sets.push_back(i);
    }

    for(int i = 0; i < n; ++i) {
      Segment seg = ds[i].second;
      sets[seg.first] = sets[seg.second];
    }

    map<int, vector<Count> > g;

    for(int i = 0; i < sets.size(); ++i) {
      g[sets[i]].push_back(counts[i]);
    }

    for(auto it : g) {
      groups.push_back(it.second);
    }
  }

  void build_match(vector<Count> & a, vector<Count> & b, Match & m) {
    // Throws away replicated elements;
    a.resize(a.size() / 2);
    b.resize(b.size() / 2);

    cout << "a.size(): " << a.size() << endl;
    cout << "b.size(): " << b.size() << endl;

    vector<vector<Count> > g;
    group_counts(b, g, b.size() - a.size());

    if (a.size() != g.size()) {
      stringstream ss;
      ss << "Groups should have the same size now:"
        << " Circuit 1: " << a.size() 
        << " Circuit 2:" << b.size() << endl;
      throw std::invalid_argument(ss.str());
    }

    for (int i = 0; i < a.size(); ++i) {
      vector<pair<string, bool> > map_value;
      for(Count c : g[i]) {
        map_value.push_back(make_pair(c.name, a[i].value  == c.value));
      }
      m.outputs[a[i].name] = map_value;
    }
  }

  struct SimulationResult {
    entropy_map count;
    int number_of_runs;

    void account_for(vector<bool> result, vector<string> names) {

      for(int i = 0; i < names.size(); ++i) {
        if(result[i])
          count[names[i]]++;
      }

      number_of_runs++;
    }
    void get_counts(vector<Count> & counts) {
      for(auto it : count) {
        // This will replicate elements
        counts.push_back(Count(it.first, it.second, true));
        counts.push_back(Count(it.first, number_of_runs - it.second, false));
      }

      sort(counts.begin(), counts.end());
    }

  };

}
}
