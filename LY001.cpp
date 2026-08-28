#include <bits/stdc++.h>
using namespace std;
using ll = long long;
using pii = pair<int, int>;
using pll = pair<ll, ll>;
const int maxn = 1e5 + 5;
const ll inf = 4e18;

int n, k;
int x[maxn], h[maxn], c[maxn];

int st[maxn][20];
int query(int l, int r) {
    if (l > r) return 0;
    int k = 31 - __builtin_clz(r - l + 1);
    return max(st[l][k], st[r - (1 << k) + 1][k]);
}

int r[maxn];

struct seg {
    ll k, b;
    seg() : k(0), b(-inf) {}
    seg(ll kk, ll bb) : k(kk), b(bb) {}
    ll f(int x) {
        return k * x + b;
    }
} sgt[maxn << 2];
#define ls k << 1
#define rs k << 1 | 1

void insert(int k, int l, int r, int L, int R, seg lin) {
    if (L > r || l > R) return ;
    int mid = (l + r) >> 1;
    if (L <= l && r <= R) {
        seg &cur = sgt[k];
        bool lw = lin.f(x[l]) > cur.f(x[l]), mw = lin.f(x[mid]) > cur.f(x[mid]), rw = lin.f(x[r]) > cur.f(x[r]);
        if (mw) swap(cur, lin);
        if (l == r) return ;
        if (lw != mw) insert(ls, l, mid, L, R, lin);
        if (rw != mw) insert(rs, mid + 1, r, L, R, lin);
        return ;
    }
    insert(ls, l, mid, L, R, lin);
    insert(rs, mid + 1, r, L, R, lin);
}

ll query(int k, int l, int r, int i) {
    ll res = sgt[k].f(x[i]);
    if (l == r) return res;
    int mid = (l + r) >> 1;
    if (i <= mid) return max(res, query(ls, l, mid, i));
    else return max(res, query(rs, mid + 1, r, i));
}

ll dp[maxn];
ll x2[maxn], kx[maxn];

void solve() {
    cin >> n >> k;
    for (int i = 1; i <= n; ++i) {
        cin >> x[i] >> h[i] >> c[i];
        x2[i] = 1ll * x[i] * x[i];
        kx[i] = 1ll * k * x[i];
        st[i][0] = h[i];
    }
    for (int j = 1; j <= 16; ++j) {
        for (int i = 0; i + (1 << j) - 1 <= n; ++i) {
            st[i][j] = max(st[i][j - 1], st[i + (1 << j - 1)][j - 1]);
        }
    }
    for (int i = 0; i < n; ++i) {
        int ll = i, rr = n;
        while (ll < rr) {
            int mm = (ll + rr + 1) >> 1;
            int hh = query(i, mm);
            if (hh - h[i] + x[mm] - x[i] <= k) ll = mm;
            else rr = mm - 1;
        }
        if (rr == i) {
            cout << "-1\n";
            return ;
        }
        r[i] = rr;
    }
    insert(1, 1, n, 0, r[0], seg(0, 0));
    for (int i = 1; i <= n; ++i) {
        dp[i] = query(1, 1, n, i) + kx[i] - x2[i] - c[i];
        if (i < n) insert(1, 1, n, i, r[i], seg(2ll * x[i], dp[i] - kx[i] - x2[i]));
    }
    if (dp[n] < 0) cout << "-1\n";
    else cout << dp[n] << "\n";
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    solve();
    return 0;
}
