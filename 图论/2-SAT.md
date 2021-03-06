## 2-SAT

update：人傻了 发现关于对称性的问题 其实按照这种思路建图完全不用考虑

我们的建边含义是：如果选择 $x$，就必须选 $y$

那么，如果选择了 $y'$，自然不能再选 $x$ 了，也就同样有 $y'$ => $x'$ 的连边

所以按照这样的含义建图，对称性是自然成立的。

---

大佬链接 [伍昱,03年IOI国家集训队论文](https://blog.csdn.net/zixiaqian/article/details/4492926)

一类适应性问题，如：给出 $n$ 个集合，每个集合中有两个元素，另外给出 $m$ 个条件，条件形如“$a$ 与 $b$ 不能同时出现”，现在问能否构造一种方案，选出 $n$ 个满足条件的元素，每个集合选一个。

2-SAT中的2指一个集合有两个元素，元素更多时，这个问题是NPC的

思路：条件中可能存在各种关系，我们把他们都转化为“如果选了 $x$，就必须选 $y$”这样的形式，因为一个集合只有两个元素，这种转换并不困难，例如：

- $a1,b2$ 不能同时选 =》 若选 $a1$ 则必选 $b1$，若选 $b2$ 则必选 $a2$
- $a1,b2$ 至少选一个 =》 若选 $a2$ 则必选 $b2$，若选 $b1$ 则必选 $a1$
- $a1$ 不能选 =》 $a1$ 连 $a2$

像这样由 $a$ 向 $b$ 连边，**注意：根据上述条件建边时，必须满足对称性，此算法才可解**

即如果有边 $a1$ => $b$1，则必有 $b2$ => $a2$。如果没有这个条件就是无稽之谈

我们发现：一个强联通分量必然得同时选，那么可以进行缩点，如果有一个点对处于同一个联通分量，即无解。否则我们在缩点后的有向无环图上考虑：

有没有可能，即便每个点对都不在同一联通分量中，仍然会冲突呢？

缩点后的图构成了一个DAG，边的含义仍然存在：如果选择了点集 $A$，并且存在 $A$ => $B$，那么 $B$ 也是必选的，接下来我把一个点能走到的所有点叫做它的**覆盖**

如果存在一个点对 $a1,a2$，使得不论选择那个，都会覆盖 $b1,b2$，就又是无解的了。

这时前面所说的“**对称性**”就有意义了，如果有 $a1$ => $b1$，则必然有 $b2$ => $a2$

这使得新图上，没有任何一个点对能完全覆盖另一个点对。

这也就使得整体必然有解，对于每个点对，选择拓扑序较小的那个点一定没错。



另外呢，因为我们用的是Tarjian求强联通分量，如果缩点后的两点间有连边，被连的那一方一定是先确定下来的。所以拓扑序直接按照缩点后的点号反序即可。

```c++
int n, m;
vector<int> vp[maxn];
int dfn[maxn], low[maxn], bel[maxn], dfcnt = 0;
stack<int> stk;
int belid = 0;

void dfs(int p){
    dfn[p] = low[p] = ++dfcnt;
    stk.push(p);
    for(int v: vp[p]){
        if(!dfn[v]) dfs(v);
        if(!bel[v]) low[p] = min(low[p], low[v]);
    }
    if(dfn[p] == low[p]){
        bel[p] = ++belid;
        while(stk.top() != p){
            bel[stk.top()] = belid;
            stk.pop();
        }
        stk.pop();
    }
}

int oth(int x){
    if(x % 2) return x+1;
    return x-1;
}
void solve(){
    cin >> n >> m;
    for(int i=1;i<=m;i++){
        int x, y;
        cin >> x >> y;
        vp[x].push_back(oth(y));
        vp[y].push_back(oth(x));
    }
    for(int i=1;i<=2*n;i++) if(!dfn[i]) dfs(i);
    for(int i=1;i<=n;i++){
        if(bel[2*i-1] == bel[2*i]){
            cout << "NIE\n";
            return;
        }
    }
    for(int i=1;i<=n;i++){
        if(bel[2*i] > bel[2*i-1]){
            cout << 2 * i - 1 << '\n';
        }else{
            cout << 2 * i << '\n';
        }
    }
}
```

