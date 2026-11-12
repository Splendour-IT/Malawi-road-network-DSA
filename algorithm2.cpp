
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <numeric>
#include <limits>
using namespace std;

const int N = 20;
const long long INF = numeric_limits<long long>::max() / 4;

string names[N] = {
    "Lilongwe", "Mchinji", "Dowa", "Salima",
    "Nkhotakota", "Dedza", "Ntcheu", "Balaka",
    "Mangochi", "Zomba", "Blantyre", "Thyolo",
    "Mulanje", "Phalombe", "Liwonde", "Mzuzu",
    "Rumphi", "Karonga", "Kasungu", "Nkhata Bay"
};

struct Edge {
    int u, v, w;
};

vector<Edge> edges = {
    {0,1,110}, {0,2,60}, {0,3,110}, {0,5,90},
    {0,18,130}, {1,18,120}, {2,15,280},
    {3,4,110}, {3,18,100}, {4,15,180},
    {5,6,90}, {6,7,80}, {7,14,40},
    {14,9,55}, {7,8,90}, {8,9,120},
    {9,10,65}, {10,11,40}, {11,12,55},
    {12,13,45}, {13,9,80}, {15,16,65},
    {16,17,120}, {15,19,50}, {18,15,150},
    {6,14,100}, {4,19,120}, {10,7,130}
};

void dijkstra(int source, int destination) {
    vector<vector<pair<int, int> > > adj(N);

    for (int i = 0; i < (int)edges.size(); i++) {
        Edge e = edges[i];
        adj[e.u].push_back(make_pair(e.v, e.w));
        adj[e.v].push_back(make_pair(e.u, e.w));
    }

    vector<long long> dist(N, INF);
    vector<int> parent(N, -1);

    priority_queue<
        pair<long long, int>,
        vector<pair<long long, int> >,
        greater<pair<long long, int> >
    > pq;

    dist[source] = 0;
    pq.push(make_pair(0, source));

    while (!pq.empty()) {
        pair<long long, int> current = pq.top();
        pq.pop();

        long long d = current.first;
        int u = current.second;

        if (d != dist[u])
            continue;

        for (int i = 0; i < (int)adj[u].size(); i++) {
            int v = adj[u][i].first;
            int w = adj[u][i].second;

            if (d + w < dist[v]) {
                dist[v] = d + w;
                parent[v] = u;
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    cout << "\n--- DIJKSTRA RESULTS ---\n";
    cout << "Starting location: " << names[source] << "\n";
    cout << "Destination: " << names[destination] << "\n\n";

    cout << "Shortest distances from "
         << names[source] << ":\n";

    for (int i = 0; i < N; i++) {
        cout << names[i] << ": ";

        if (dist[i] == INF)
            cout << "Unreachable\n";
        else
            cout << dist[i] << " km\n";
    }

    cout << "\nShortest route:\n";

    if (dist[destination] == INF) {
        cout << "No route exists.\n";
        return;
    }

    vector<int> path;

    for (int v = destination; v != -1; v = parent[v])
        path.push_back(v);

    reverse(path.begin(), path.end());

    for (int i = 0; i < (int)path.size(); i++) {
        if (i > 0)
            cout << " -> ";

        cout << names[path[i]];
    }

    cout << "\nTotal distance: "
         << dist[destination] << " km\n";
}

struct DSU {
    vector<int> parent, rankValue;

    DSU(int n) : parent(n), rankValue(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int x) {
        if (parent[x] != x)
            parent[x] = find(parent[x]);

        return parent[x];
    }

    bool unite(int a, int b) {
        a = find(a);
        b = find(b);

        if (a == b)
            return false;

        if (rankValue[a] < rankValue[b])
            swap(a, b);

        parent[b] = a;

        if (rankValue[a] == rankValue[b])
            rankValue[a]++;

        return true;
    }
};

void kruskal() {
    vector<Edge> sortedEdges = edges;

    sort(sortedEdges.begin(), sortedEdges.end(),
         [](const Edge &a, const Edge &b) {
             return a.w < b.w;
         });

    DSU dsu(N);
    vector<Edge> mst;
    long long total = 0;

    for (int i = 0; i < (int)sortedEdges.size(); i++) {
        Edge e = sortedEdges[i];

        if (dsu.unite(e.u, e.v)) {
            mst.push_back(e);
            total += e.w;
        }

        if ((int)mst.size() == N - 1)
            break;
    }

    cout << "\n--- KRUSKAL MST RESULTS ---\n";

    if ((int)mst.size() != N - 1) {
        cout << "The graph is disconnected.\n";
        cout << "A complete spanning tree does not exist.\n";
        return;
    }

    for (int i = 0; i < (int)mst.size(); i++) {
        cout << names[mst[i].u] << " - "
             << names[mst[i].v] << " : "
             << mst[i].w << " km\n";
    }

    cout << "\nNumber of vertices: " << N;
    cout << "\nNumber of MST edges: " << mst.size();
    cout << "\nTotal MST weight: " << total << " km\n";
}

void showLocations() {
    cout << "\n--- AVAILABLE LOCATIONS ---\n";

    for (int i = 0; i < N; i++) {
        cout << i + 1 << ". " << names[i] << "\n";
    }
}

int main() {
    int choice;

    do {
        cout << "\n================================\n";
        cout << " MALAWI ROAD NETWORK PROGRAM\n";
        cout << "================================\n";
        cout << "1. Test Dijkstra's shortest path\n";
        cout << "2. Test Kruskal's MST\n";
        cout << "3. Show available locations\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            return 1;
        }

        if (choice == 1) {
            int start, destination;

            showLocations();

            cout << "\nEnter starting location number: ";
            if (!(cin >> start)) {
                cout << "Invalid input.\n";
                return 1;
            }

            cout << "Enter destination location number: ";
            if (!(cin >> destination)) {
                cout << "Invalid input.\n";
                return 1;
            }

            if (start < 1 || start > N ||
                destination < 1 || destination > N) {
                cout << "Invalid location number. "
                     << "Choose from 1 to " << N << ".\n";
                continue;
            }

            dijkstra(start - 1, destination - 1);
        }
        else if (choice == 2) {
            kruskal();
        }
        else if (choice == 3) {
            showLocations();
        }
        else if (choice == 4) {
            cout << "Program terminated.\n";
        }
        else {
            cout << "Invalid menu choice. Try again.\n";
        }

    } while (choice != 4);

    return 0;
}
