#### Kruskal重构树

结构：kruskal过程中，每个节点作为叶子节点，每条边作为中间节点。连边时将两节点所在子树的根节点相连。这样得出的kruskal重构树中，两节点的LCA即为原图中两点间的**最小瓶颈路**（边权为最大值意义下的最短路）

写起来很简单，对边建立节点可以直接利用kruskal中的并查集，直接拿边节点作为集合根

```C++
int n, m;
int fa[maxn * 2];

int find(int x){
    if(x == fa[x]) return x;
    return fa[x] = find(fa[x]);
}

struct edge{
    int x, y, w;
    bool operator <(const edge &t)const{
        return w < t.w;
    }
}e[maxn];

vector<int> vp2[maxn * 2];
ll val[maxn * 2], ecnt;//边节点编号

void solve(){
    cin >> n >> m;
    for(int i=1;i<=m;i++) cin >> e[i].x >> e[i].y >> e[i].w;
    for(int i=1;i<=n+m;i++) fa[i] = i;
    sort(e+1, e+m+1);
    ecnt = n;
    for(int i=1;i<=m;i++){
        int fx = find(e[i].x), fy = find(e[i].y);
        if(fx == fy) continue;
        fa[fx] = fa[fy] = ++ecnt;//直接用lca作为集合根节点
        vp2[ecnt].push_back(fx);
        vp2[ecnt].push_back(fy);
        val[ecnt] = e[i].w;
    }
    //...
}
```