#include <algorithm>
#include <cstdio>
#include <cstring>
#include <queue>
#include <stdio.h>
#include <iostream>
using namespace std;
#define ll long long
const int maxn=1000005;
int mode=998244353;
void Swap(int *a, int *b){int ins=*a;*a=*b;*b=ins;}

string s;
int sa[maxn],rk[maxn],brk[maxn];
int id[maxn],cnt[maxn];

int n;
int m = 140;

void solve(){
    cin>>s;
    n=s.size();
    s=' '+s;

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
