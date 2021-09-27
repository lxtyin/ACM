## CDQ分治

主要用于处理和点对有关的问题，核心思路是将 $[l.r]$ 上的区间点对问题转化为：

- $[l,mid]$ 和 $[mid+1,r]$ 上的区间点对问题
- 统计或计算一点在左一点在右的情况

最主要的应用是[三维偏序](https://www.luogu.com.cn/problem/P3810)

其实和模板有一些差别，这题要处理的三维偏序包括等于的情况，要注意一下

```c++
int n,m;//m为最大属性值
struct BitTree{//4行树状数组
    int t[maxn];
    inline int lb(int x) {return x&-x;}
    void add(int p, int ad){for(int i=p;i<=m;i+=lb(i)) t[i]+=ad;}
    int sum(int p){ return p?t[p] + sum(p-lb(p)):0;}
}bt;

struct node{
    int aa,bb,cc;
    int cnt;
    int ans;
    bool operator ==(const node &x)const{
        return aa==x.aa && bb==x.bb && cc==x.cc;
    }
}a[maxn];

bool cmp1(node x,node y){
    if(x.aa != y.aa) return x.aa < y.aa;
    //因为要统计等于的情况，所以
    //后两步不能忽略！在a相等时，要保证后面的数都不会对前面的数产生贡献
    //（正常的CDQ默认第一维上右边比左边大，a相等时只会统计左对右的贡献）
    //后两步保证了ai=aj，i<j时，必有bi<bj或ci<cj，要么就完全相等
    if(x.bb != y.bb) return x.bb < y.bb;
    if(x.cc != y.cc) return x.cc < y.cc;
}
bool cmp2(node x,node y){
    if(x.bb != y.bb) return x.bb < y.bb;
}

//主要的板子是这一块
void cdq(int l,int r){
    if(l>=r) return;
    int mid = (l+r)/2;
    cdq(l, mid);
    cdq(mid+1, r);
    sort(a+l, a+mid+1, cmp2);
    sort(a+mid+1, a+r+1, cmp2);

    int j=l;
    for(int i=mid+1;i<=r;i++){
        while(j <= mid && a[j].bb <= a[i].bb){
            bt.add(a[j].cc, a[j].cnt);
            j++;
        }
        a[i].ans += bt.sum(a[i].cc);
    }
    for(int i=l;i<j;i++) bt.add(a[i].cc, -a[i].cnt);
}

void solve(){
    scanf("%d%d%",&n,&m);
    for(int i=1;i<=n;i++){
        scanf("%d%d%d",&a[i].aa,&a[i].bb,&a[i].cc);
        a[i].cnt = 1;
    }
    sort(a+1,a+n+1, cmp1);
    int nk = 1;
   	//去重，记录权
    for(int i=2;i<=n;i++){
        if(a[i] == a[nk]){
            a[nk].cnt++;
            a[nk].ans++;
        }
        else a[++nk] = a[i];
    }
    swap(n,nk);
    cdq(1,n);

    int cnt[maxn];
    memset(cnt,0,sizeof(cnt));
    for(int i=1;i<=n;i++) cnt[a[i].ans]+=a[i].cnt;
    for(int d=0;d<nk;d++) printf("%d\n", cnt[d]);
}
```