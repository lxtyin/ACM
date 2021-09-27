## 树上启发式合并（dsu on tree）

~~虽然叫dsu但这和并查集貌似没什么关系~~

> 例：
>
> 给你一棵树，每个节点有一个颜色，要求出每个子树中数量最多的颜色并输出
>
> （数量相同的情况先不考虑 ~~不重要~~）

当我们需要在每个子树上统计一些信息的时候，往往会开一个全局的cnt数组，试图 dfs $O(n)$ 扫一遍，一边加点一边得到答案

但对于一棵树而言显然有问题：当我们统计完其左子树的信息后，必须清空整个cnt数组才能去扫右子树，这样其实就已经变成 $O(n^2)$ 了

当然我们可以稍微偷工减料一点，因为最后一棵子树统计完后不用清空，我们可以最后遍历最大的那棵子树

最大子树可以通过一遍dfs预处理出子树的size，记录每个点的重儿子得到（类似树剖）

然而就是这一点偷工减料，**使得整个算法复杂度直接降至 $O(nlogn)$**

如果不关心证明的话，**你已经学会 dsu on tree 了**



#### 证明~~为什么这样瞎搞就能获得$nlogn$​的复杂度~~：

以下通过**感性理解**的方式说明为什么这东西能优化这么多

回顾一下在每个节点处我们要做什么：

- dfs轻儿子，并消除影响
- dfs重儿子，不消除影响
- 再统计轻子树的影响

前两步的操作一共是 $O(n)$ 的，就是最朴素的从头到尾扫一遍

现在需要考虑：在每个点处对每个轻子树扫一遍的复杂度

如果一个点和根节点之间一共有 x 条轻边，那么它会被遍历差不多 x+1 次

而轻重链剖分有个很好的性质：走一条轻边时，**节点数量至少被砍一半**，否则这就不是轻边了

那么从根节点到任意节点经过的轻边数量最多是 $logn$​ 级别的

所以其实很显然了：复杂度就是 $O(nlogn)$



再看看极端情况加深理解：

树上问题最容易被出题人的各种链，菊花图，链加菊花图啥的卡掉

如果这棵树长得像链，它将被**最后走最大子树**这一小贪心优化掉一大半；

如果这棵树长得像菊花图，，那么根节点到任意节点间的轻边数量都将是极少的；

所以你可以相信dsu on tree



代码（[这道题](https://www.luogu.com.cn/problem/CF600E)的）

```c++
int n;
int col[maxn];
int cnt[maxn];
ll ans[maxn];
int siz[maxn], son[maxn];

struct Edge{
    int t, nt;
}e[maxn*2];

int hd[maxn], ecnt = 0;

inline void add(int x, int y){
    e[++ecnt].t = y;
    e[ecnt].nt = hd[x];
    hd[x] = ecnt;
}

void dfs1(int p, int fa){
    siz[p] = 1;
    son[p] = 0;
    for(int i=hd[p];i;i=e[i].nt){
        int v = e[i].t;
        if(v!=fa){
            dfs1(v, p);
            siz[p] += siz[v];
            if(siz[v] > siz[son[p]]) son[p] = v;
        }
    }
}

ll tot = 0, mxc = 0;
void addcol(int c, int ad){//只计加不计减（减肯定减到0）
    cnt[c] += ad;
    if(cnt[c] > mxc){
        mxc = cnt[c];
        tot = c;
    }else if(cnt[c] == mxc){
        tot += c;
    }
}

void cntall(int p, int fa, int d){
    for(int i=hd[p];i;i=e[i].nt){
        int v = e[i].t;
        if(v!=fa){
            cntall(v, p, d);
        }
    }
    addcol(col[p], d);
}

void dfs(int p, int fa, int sav){
    for(int i=hd[p];i;i=e[i].nt){
        int v = e[i].t;
        if(v!=fa && v!=son[p]){
            dfs(v, p, 0);
        }
    }
    if(son[p]) dfs(son[p], p, 1);
    for(int i=hd[p];i;i=e[i].nt){
        int v = e[i].t;
        if(v!=fa && v!=son[p]){
            cntall(v, p, 1);
        }
    }
    //此时所有子节点均已记录
    addcol(col[p], 1);
    ans[p] = tot;
    if(!sav) cntall(p, fa, -1), tot = mxc = 0;
}

void solve(){
    cin >> n;
    for(int i=1;i<=n;i++) cin >> col[i];
    for(int i=1;i<n;i++){
        int x, y;
        cin >> x >> y;
        add(x, y); add(y, x);
    }
    dfs1(1, -1);
    dfs(1, -1, 1);
    for(int i=1;i<n;i++) cout << ans[i] << ' ';
    cout << ans[n] << '\n';
}
```