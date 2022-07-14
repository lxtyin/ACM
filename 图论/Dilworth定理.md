## Dilworth定理

偏序集能划分成的最少全序集个数等于最长反链的元素个数。

什么是偏序集？

：集合中元素关系构成一个DAG；

什么是全序集？

：集合中元素关系构成一条有向链，最少全序集覆盖也就是最小链覆盖；

什么是反链？

：并不是指将边反过来连，而是要求反链集合中任意两个点都不可比较（即DAG上任意两点间不可互达）

对于DAG来说，这个定理相当于：最小链覆盖 = 最长反链



[【例题】G. Robot and Candies](https://codeforces.com/contest/1611/problem/G)

奇数点和偶数点是两个完全不相干的图，分别考虑这两个图：

题目让我们求的是一个最小链覆盖，我们可以转化成求最长反链来做。

详见：https://www.bilibili.com/video/BV1Ji4y1o7Pz?spm_id_from=333.999.0.0

```c++
int n, m;
int a2[maxn], f2[maxn];
inline int& a(int x, int y){
    return a2[(x-1)*m+y];
}
inline int& f(int x, int y){
    return f2[(x-1)*m+y];
}

void solve(){
    cin >> n >> m;
    fill(f2, f2+n*m+10, 0);
    for(int i=1;i<=n;i++){
        string s;
        cin >> s;
        for(int j=0;j<s.size();j++){
            a(i, j+1) = s[j] - '0';
        }
    }
    int num[2] = {0, 0};
    for(int j=1;j<=m;j++){
        for(int i=1;i<=n;i++){
            if(a(i, j) == 1) f(i, j) = 1;
            if(j > 2) f(i, j) = max(f(i, j), f(i, j-2) + a(i, j));
            if(j > 1){
                if(i > 1) f(i, j) = max(f(i, j), f(i-1, j-1));
                if(i < n) f(i, j) = max(f(i, j), f(i+1, j-1));
            }
            int tp = (i + j) % 2;
            num[tp] = max(num[tp], f(i, j));
        }
    }
    cout << num[0] + num[1] << '\n';
}
```

