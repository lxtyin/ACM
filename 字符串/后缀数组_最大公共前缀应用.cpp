#include <algorithm>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <queue>
#include <iostream>
using namespace std;
#define ll long long
const int maxn=1005;
int mode=998244353;
void swap(int *a, int *b){int ins=*a;*a=*b;*b=ins;}

string s;
int n;
int sa[maxn],rk[maxn],brk[maxn];
int id[maxn],cnt[maxn];
int ht[maxn],miH[maxn][25];

void getSA(){
    int m = 140;
    for(int i=1;i<=n;i++) cnt[rk[i] = s[i]]++;
    for(int i=1;i<=m;i++) cnt[i] += cnt[i-1];
    for(int i=1;i<=n;i++) sa[cnt[rk[i]]--] = i;

    int tail;
    for(int w=1;;w<<=1){
        tail = 0;
        for(int i=n-w+1;i<=n;i++) id[++tail] = i;
        for(int i=1;i<=n;i++){
            if(sa[i] > w) id[++tail] = sa[i] - w;
        }

        memset(cnt,0,sizeof(int)*(m+5));
        for(int i=1;i<=n;i++) cnt[rk[id[i]]]++;
        for(int i=1;i<=m;i++) cnt[i] += cnt[i-1];
        for(int i=n;i>=1;i--) sa[cnt[rk[id[i]]]--] = id[i];

        tail = 0;
        for(int i=1;i<=n;i++) brk[i] = rk[i];
        for(int i=1;i<=n;i++){
            if(brk[sa[i]] != brk[sa[i-1]] || brk[sa[i]+w] != brk[sa[i-1]+w]) tail++;
            rk[sa[i]] = tail;
        }
        m = tail;
        if(m == n) break;
    }
}

//ht[i]: 后缀第i名与第i-1名的最大公共前缀
//Theorem: ht[rk[i]] >= ht[rk[i-1]] - 1
void getHeight(){
    int k = 0;
    for(int i=1;i<=n;i++){
        if(k) k--;
        while(s[i+k] == s[sa[rk[i]-1]+k]) k++;
        ht[rk[i]] = k;
    }
}

//lcp: 最大公共前缀
//lcp(i,j) = min{ ht[rk[i]+1] ... ht[rk[j]] } 从i开始的后缀和从j开始的后缀的最大公共前缀
//lcp(sa[i],sa[j]) = min{ ht[i+1] ... ht[j] }
//trans into RMQ problem
void getMinHeight(){
    for(int i=1;i<=n;i++) miH[i][0] = ht[i];
    for(int k=1;k<=20;k++){
        for(int i=1;i+(1<<k)-1<=n;i++){
            miH[i][k] = min(miH[i][k-1], miH[i+(1<<(k-1))][k-1]);
        }
    }
}

//获取两子串的lcp
int getLcp(int i,int j,int x,int y){
    int l = rk[i], r = rk[x];
    if(l>r) swap(&l,&r);
    l++;
    int m = log(r-l+1)/log(2);
    int lcp = min(miH[l][m], miH[r-(1<<m)+1][m]);
    return min(lcp, min(j-i+1, y-x+1));
}

//O(1)比较两子串大小，按<返回
bool cmp(int i,int j,int x,int y){
    int lcp = getLcp(i,n,x,n);//拿两后缀来求lcp
    if(lcp >= min(j-i+1, y-x+1)) return j-i+1 < y-x+1;
    else return rk[i] < rk[x];
}

//计算不重复子串总数
//子串总数 - 重复数 = 子串总数 - 每个后缀的前缀重复次数（意会
int cntSum(){
    int res = n*(n+1)/2;
    for(int i=2;i<=n;i++) res-=ht[i];
    return res;
}

void solve(){
    cin>>s;
    n=s.size();
    s=' '+s;
    getSA();
    getHeight();
    getMinHeight();
    printf("%d\n",getLcp(7,19,18,26));
    for(int i=1;i<n;i++) printf("%d ",sa[i]);
    printf("%d\n",sa[n]);
}

int main() {
#ifdef ACM_LOCAL
    freopen("x.txt","r",stdin);
#endif
    int T=1;
//    scanf("%d",&T);
    while(T--){
        solve();
    }
    return 0;
}
