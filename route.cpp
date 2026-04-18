/*
 * ╔══════════════════════════════════════════════════════════════════╗
 * ║         DELIVERY ROUTE PLANNER — DIJKSTRA ENGINE v2.0           ║
 * ║         Compatible with GCC 6.x / MinGW on Windows             ║
 * ╚══════════════════════════════════════════════════════════════════╝
 *
 *  Compile:  g++ -O2 -std=c++11 -o route route.cpp
 *  Run:      route.exe > output.json
 *  Benchmark terminal: route.exe 2>&1
 */

#include <bits/stdc++.h>
using namespace std;

// ─── Data Structures ──────────────────────────────────────────────

struct Edge {
    int to;
    int weight;
};

struct Step {
    int  node;
    int  dist;
    int  via;
    bool relaxed;
    Step(int nd, int d, int v, bool r) : node(nd), dist(d), via(v), relaxed(r) {}
};

struct BenchResult {
    string density;
    int    nodes;
    int    edges;
    double timeMs;
    int    cost;
};

// ─── Globals ──────────────────────────────────────────────────────

vector<vector<Edge> > graph;
int N;

void initGraph(int n) {
    N = n;
    graph.assign(N, vector<Edge>());
}

void addEdge(int u, int v, int w) {
    Edge e1; e1.to = v; e1.weight = w;
    Edge e2; e2.to = u; e2.weight = w;
    graph[u].push_back(e1);
    graph[v].push_back(e2);
}

// ─── Dijkstra ─────────────────────────────────────────────────────

struct DijkstraResult {
    vector<int>  dist;
    vector<int>  parent;
    vector<Step> steps;
    double       timeMs;
};

DijkstraResult dijkstra(int src) {
    vector<int>  dist(N, INT_MAX);
    vector<int>  parent(N, -1);
    vector<bool> visited(N, false);
    vector<Step> steps;

    // min-heap: pair<dist, node>
    priority_queue<pair<int,int>,
                   vector<pair<int,int> >,
                   greater<pair<int,int> > > pq;

    dist[src] = 0;
    pq.push(make_pair(0, src));

    clock_t t0 = clock();

    while (!pq.empty()) {
        int d = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (d > dist[u] || visited[u]) continue;
        visited[u] = true;

        bool relaxed = false;
        for (int i = 0; i < (int)graph[u].size(); i++) {
            int v = graph[u][i].to;
            int w = graph[u][i].weight;
            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v]   = dist[u] + w;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
                relaxed = true;
            }
        }
        steps.push_back(Step(u, dist[u], parent[u], relaxed));
    }

    clock_t t1 = clock();
    double ms  = 1000.0 * (t1 - t0) / CLOCKS_PER_SEC;

    DijkstraResult res;
    res.dist   = dist;
    res.parent = parent;
    res.steps  = steps;
    res.timeMs = ms;
    return res;
}

// ─── Path reconstruction ──────────────────────────────────────────

vector<int> getPath(const vector<int>& parent, int dest) {
    vector<int> path;
    for (int v = dest; v != -1; v = parent[v])
        path.push_back(v);
    reverse(path.begin(), path.end());
    return path;
}

// ─── JSON helpers ─────────────────────────────────────────────────

string jsonInt(int v) {
    if (v == INT_MAX) return "null";
    ostringstream oss;
    oss << v;
    return oss.str();
}

string jsonDbl(double v) {
    ostringstream oss;
    oss << fixed << setprecision(9) << v;
    return oss.str();
}

void printJSON(
    const DijkstraResult&              res,
    const vector<int>&                 path,
    int                                src,
    int                                dest,
    const vector<tuple<int,int,int> >& edgeList,
    const vector<string>&              nodeNames)
{
    cout << "{\n";

    // nodes
    cout << "  \"nodes\": [\n";
    for (int i = 0; i < N; i++) {
        cout << "    {\"id\":" << i << ",\"name\":\"" << nodeNames[i] << "\"}";
        if (i < N-1) cout << ",";
        cout << "\n";
    }
    cout << "  ],\n";

    // edges
    cout << "  \"edges\": [\n";
    for (int i = 0; i < (int)edgeList.size(); i++) {
        int u = get<0>(edgeList[i]);
        int v = get<1>(edgeList[i]);
        int w = get<2>(edgeList[i]);
        cout << "    {\"id\":" << i
             << ",\"u\":" << u
             << ",\"v\":" << v
             << ",\"w\":" << w << "}";
        if (i < (int)edgeList.size()-1) cout << ",";
        cout << "\n";
    }
    cout << "  ],\n";

    // path
    cout << "  \"path\": [";
    for (int i = 0; i < (int)path.size(); i++)
        cout << path[i] << (i < (int)path.size()-1 ? "," : "");
    cout << "],\n";

    // dist map
    cout << "  \"dist\": {";
    for (int i = 0; i < N; i++) {
        cout << "\"" << i << "\":" << jsonInt(res.dist[i]);
        if (i < N-1) cout << ",";
    }
    cout << "},\n";

    // parent map
    cout << "  \"parent\": {";
    for (int i = 0; i < N; i++) {
        cout << "\"" << i << "\":"
             << (res.parent[i] == -1 ? "null" : jsonInt(res.parent[i]));
        if (i < N-1) cout << ",";
    }
    cout << "},\n";

    // steps
    cout << "  \"steps\": [\n";
    for (int i = 0; i < (int)res.steps.size(); i++) {
        const Step& s = res.steps[i];
        cout << "    {\"step\":" << (i+1)
             << ",\"node\":" << s.node
             << ",\"dist\":" << s.dist
             << ",\"via\":"  << (s.via == -1 ? "null" : jsonInt(s.via))
             << ",\"relaxed\":" << (s.relaxed ? "true" : "false") << "}";
        if (i < (int)res.steps.size()-1) cout << ",";
        cout << "\n";
    }
    cout << "  ],\n";

    cout << "  \"src\":"        << src                     << ",\n";
    cout << "  \"dest\":"       << dest                    << ",\n";
    cout << "  \"cost\":"       << jsonInt(res.dist[dest]) << ",\n";
    cout << "  \"timeMs\":"     << jsonDbl(res.timeMs)     << ",\n";
    cout << "  \"nodeCount\":"  << N                       << ",\n";
    cout << "  \"edgeCount\":"  << (int)edgeList.size()    << ",\n";
    cout << "  \"pathLength\":" << (int)path.size()        << "\n";
    cout << "}\n";
}

// ─── Benchmark ────────────────────────────────────────────────────

BenchResult runBenchmark(const string& density, int nodeCount, int edgeCount) {
    initGraph(nodeCount);
    srand(42);

    // spanning chain for connectivity
    for (int i = 0; i < nodeCount - 1; i++)
        addEdge(i, i+1, rand() % 20 + 1);

    int extra = edgeCount - (nodeCount - 1);
    for (int i = 0; i < extra; i++) {
        int u = rand() % nodeCount;
        int v = rand() % nodeCount;
        if (u != v) addEdge(u, v, rand() % 20 + 1);
    }

    DijkstraResult res = dijkstra(0);
    int cost = (res.dist[nodeCount-1] == INT_MAX) ? -1 : res.dist[nodeCount-1];

    BenchResult br;
    br.density = density;
    br.nodes   = nodeCount;
    br.edges   = edgeCount;
    br.timeMs  = res.timeMs;
    br.cost    = cost;
    return br;
}

// ─── Main ─────────────────────────────────────────────────────────

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    vector<string> nodeNames;
    nodeNames.push_back("Warehouse");
    nodeNames.push_back("Hub-Alpha");
    nodeNames.push_back("Hub-Beta");
    nodeNames.push_back("Hub-Gamma");
    nodeNames.push_back("Hub-Delta");
    nodeNames.push_back("Hub-Epsilon");
    nodeNames.push_back("Destination");

    initGraph(7);

    vector<tuple<int,int,int> > edgeList;
    edgeList.push_back(make_tuple(0,1,4));
    edgeList.push_back(make_tuple(0,2,2));
    edgeList.push_back(make_tuple(1,3,5));
    edgeList.push_back(make_tuple(2,3,8));
    edgeList.push_back(make_tuple(2,4,3));
    edgeList.push_back(make_tuple(3,6,6));
    edgeList.push_back(make_tuple(4,5,7));
    edgeList.push_back(make_tuple(5,6,1));
    edgeList.push_back(make_tuple(1,4,2));

    for (int i = 0; i < (int)edgeList.size(); i++) {
        int u = get<0>(edgeList[i]);
        int v = get<1>(edgeList[i]);
        int w = get<2>(edgeList[i]);
        addEdge(u, v, w);
    }

    int src = 0, dest = 6;

    DijkstraResult res  = dijkstra(src);
    vector<int>    path = getPath(res.parent, dest);

    printJSON(res, path, src, dest, edgeList, nodeNames);

    // ── Benchmark (stderr) ────────────────────────────────────────
    cerr << "\n";
    cerr << "==========================================================\n";
    cerr << "           DIJKSTRA BENCHMARK RESULTS\n";
    cerr << "==========================================================\n";
    cerr << left
         << setw(10) << "Density"
         << setw(8)  << "Nodes"
         << setw(8)  << "Edges"
         << setw(14) << "Time (ms)"
         << "Cost\n";
    cerr << "----------------------------------------------------------\n";

    // benchmark cases
    struct BCase { const char* d; int n; int e; };
    BCase cases[5] = {
        {"Sparse",   20,   25},
        {"Medium",   50,  100},
        {"Dense",   100,  400},
        {"X-Dense", 500, 3000},
        {"Huge",   1000, 8000}
    };

    for (int i = 0; i < 5; i++) {
        BenchResult br = runBenchmark(cases[i].d, cases[i].n, cases[i].e);
        cerr << left
             << setw(10) << br.density
             << setw(8)  << br.nodes
             << setw(8)  << br.edges
             << fixed << setprecision(6)
             << setw(14) << br.timeMs;
        if (br.cost == -1) cerr << "N/A\n";
        else               cerr << br.cost << "\n";
    }

    cerr << "==========================================================\n";
    cerr << "\nPath: ";
    for (int i = 0; i < (int)path.size(); i++) {
        cerr << nodeNames[path[i]];
        if (i < (int)path.size()-1) cerr << " -> ";
    }
    cerr << "\nCost: " << res.dist[dest] << " units\n";
    cerr << "Time: " << fixed << setprecision(6) << res.timeMs << " ms\n\n";

    return 0;
}