const int INF = 0x3f3f3f3f;
const int N = 210, M = N * N;

int n, m;
int head[N], pre[M], nxt[M], g[M], e, from[M];
int level[N], S, T, path[N], cur[N], q[N];

void init() {
    clr(head, -1);
    e = 0;
}

void addedge(int u, int v, int c) {
    g[e] = c, pre[e] = v, from[e] = u, nxt[e] = head[u], head[u] = e++;
    g[e] = 0, pre[e] = u, from[e] = v, nxt[e] = head[v], head[v] = e++;
}

bool bfs() {
    int st = 0, ed = -1;
    q[++ed] = S;
    clr(level, -1);
    level[S] = 1;
    while (st <= ed) {
        int u = q[st++];
        for (int i = head[u]; ~i; i = nxt[i]) {
            int v = pre[i];
            if (g[i] && level[v] == -1) {
                level[v] = level[u] + 1;
                q[++ed] = v;
            }
        }
    }
    if (~level[T]) return true;
    return false;
}

int Dinic() {
    int ret = 0;
    while (1) {
        if (!bfs()) break;
        int pn = 0, u = S;
        memcpy(cur, head, sizeof(head));
        while (1) {
            if (u == T) {
                int delta = INF, tmp;
                for (int i = 0; i < pn; ++i) {
                    int j = path[i];
                    if (g[j] < delta)  {
                        delta = g[j];
                        tmp = i;
                    }
                }
                for (int i = 0; i < pn; ++i) {
                    int j = path[i];
                    g[j] -= delta;
                    g[j ^ 1] += delta;
                }
                u = from[ path[tmp] ];
                pn = tmp;
                ret += delta;
            }
            int &i = cur[u];
            for (; ~i; i = nxt[i]) {
                if (!g[i]) continue;
                int v = pre[i];
                if (level[v] == level[u] + 1) break;
            }
            if (~i) {
                path[pn++] = i;
                u = pre[i];
            }
            else {
                if (pn == 0) break;
                level[u] = -1;
                --pn;
                u = from[ path[pn] ];
            }
        }
    }
    return ret;
}
