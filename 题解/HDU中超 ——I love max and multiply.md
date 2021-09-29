## [HDU中超 ——I love max and multiply](https://acm.dingbacode.com/showproblem.php?pid=6971)

> 言语比较抽象 写给自己看



#### 题意

给两个序列 $A,B$

设 $C_k = max\{A_i*B_j\}$​​ ，$i,j$​ 满足 $i\&j >= k$ 

求 $\sum^{n-1}_{i=0}C_k$​ 答案对 $998244353$ 取模



#### 思路

首先注意到 $C_k$ 是单调的

如果设 $D_k = max\{A_i*B_j\},i\&j=k$​

那么对 $D_k$ 从大到小取max即可得到每一位的 $C_k$

再适当扩展一下，使 $D_k = max\{A_i*B_j\},k∈i\&j$

也就是 $k∈i$​ 且  $k∈j$​ 

那么先预处理出 $mxa_k$ ，表示满足 $k∈i$ 时，$A_i$ 的最大值

这个可以dp，设 $p$  比 $k$ 多了一位1，那么 $mxa_k$ 包含了 $mxa_p$

每一个k都由比它多一位1的p转移而来

同理还需要预处理出 $mxb，mia，mib$​ （因为存在负数，结果的最大值可能是两负数相乘

结果由这四个数两两相乘取最大即可

```c++
int mxa[maxn],mxb[maxn];
int mia[maxn],mib[maxn];

int a[maxn],b[maxn];
void solve(){
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d",&a[i]);
        mxa[i] = a[i];
        mia[i] = a[i];
    }
    for(int i=0;i<n;i++){
        scanf("%d",&b[i]);
        mxb[i] = b[i];
        mib[i] = b[i];
    }
    int m=1;
    while(m<n) m<<=1;
    for(int i=n;i<m;i++){
        mxa[i] = mxb[i] = -1e9;
        mia[i] = mib[i] = 1e9;
    }
    for(int k=1;k<m;k<<=1){
        for(int i=0;i<m;i++){
            if(i&k) continue;
            mxa[i] = max(mxa[i], mxa[i|k]);
            mxb[i] = max(mxb[i], mxb[i|k]);
            mia[i] = min(mia[i], mia[i|k]);
            mib[i] = min(mib[i], mib[i|k]);
        }
    }
    ll Nmax = -1e18;
    ll ans = 0ll;
    for(int k = n-1; k >= 0; k--){
        Nmax = max(Nmax, (ll)mxa[k] * mxb[k]);
        Nmax = max(Nmax, (ll)mxa[k] * mib[k]);
        Nmax = max(Nmax, (ll)mia[k] * mxb[k]);
        Nmax = max(Nmax, (ll)mia[k] * mib[k]);
        ll nm = (Nmax%mode + mode )%mode;
        ans = (ans + nm)%mode;
    }
    printf("%lld\n",ans);
}
```



个人对dp过程的理解：

例如10110，它应该包括11110,10111,11111的结果

倒着推的话就是一个从左向右溯源的过程

10110 <- 11110 <- 11111

因为是从低位到高位枚举的，所以11110肯定在10110接受其值之前就先接受了11111的值

那么传递到10110时就没有遗漏了