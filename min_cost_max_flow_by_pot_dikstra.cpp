
struct MCMF {
    struct Edge {
        int to, rev;
        ll cap, cost;
    };

    int n;
    vector<vector<Edge>> g;

    MCMF(int n) : n(n), g(n) {}

    void add_edge(int u, int v, ll cap, ll cost) {
        int a = g[u].size(), b = g[v].size();
        if (u == v) b++;
        g[u].push_back({v, b, cap, cost});
        g[v].push_back({u, a, 0, -cost});
    }

    pair<ll, ll> flow(int s, int t, ll limit = (1LL << 60)) {
        const ll INF = (1LL << 60);
        vector<ll> p(n, 0), d(n);
        vector<int> pv(n), pe(n);
        ll f = 0, cost = 0;

        while (f < limit) {
            fill(d.begin(), d.end(), INF);
            priority_queue<pair<ll, int>,
                vector<pair<ll, int>>,
                greater<pair<ll, int>>> q;

            d[s] = 0;
            q.push({0, s});

            while (!q.empty()) {
                auto [dv, v] = q.top();
                q.pop();
                if (dv != d[v]) continue;

                for (int i = 0; i < (int)g[v].size(); i++) {
                    auto &e = g[v][i];
                    if (e.cap == 0) continue;

                    ll nd = dv + e.cost + p[v] - p[e.to];
                    if (nd < d[e.to]) {
                        d[e.to] = nd;
                        pv[e.to] = v;
                        pe[e.to] = i;
                        q.push({nd, e.to});
                    }
                }
            }

            if (d[t] == INF) break;

            for (int v = 0; v < n; v++) {
                if (d[v] != INF) p[v] += d[v];
            }

            ll add = limit - f;
            for (int v = t; v != s; v = pv[v]) {
                add = min(add, g[pv[v]][pe[v]].cap);
            }

            for (int v = t; v != s; v = pv[v]) {
                Edge &e = g[pv[v]][pe[v]];
                e.cap -= add;
                g[v][e.rev].cap += add;
            }

            f += add;
            cost += add * (p[t] - p[s]);
        }

        return {f, cost};
    }
};


MCMF mf(n);

mf.add_edge(u, v, cap, cost);

// Максимальный поток минимальной стоимости
auto [f, cost] = mf.flow(s, t);

// Поток не больше k минимальной стоимости
auto [f2, cost2] = mf.flow(s, t, k);

auto [f, cost] = mf.flow(s, t, k);
if (f < k) {
    cout << -1 << '\n';
} else {
    cout << cost << '\n';
}
