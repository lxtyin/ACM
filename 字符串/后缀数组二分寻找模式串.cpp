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
    for(int i=n;i>=1;i--) sa[cnt[rk[i]]--] = i;

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

    string st;
    cin>>st;
    int mi,mx;
    int l=1, r=n,mid;//first one >= st
    while(l<r){
        mid = (l+r)/2;
        string sb = s.substr(sa[mid]);
        if(sb < st){
            l = mid+1;
        }else{
            r = mid;
        }
    }
    mi = l;

    st=st + char('z'+1);//find the last one <= st + 'z'
    l=1, r=n;
    while(l<r){
        mid = (l+r+1)/2;
        string sb = s.substr(sa[mid]);
        if(sb < st){
            l = mid;
        }else{
            r = mid-1;
        }
    }
    mx = l;
    cout<<sa[mx]<<' '<<sa[mi]<<endl;
    cout<<abs(mx - mi) + 1<<endl;

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
