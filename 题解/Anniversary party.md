# Anniversary party



**Problem Description**

There is going to be a party to celebrate the 80-th Anniversary of the Ural State University. The University has a hierarchical structure of employees. It means that the supervisor relation forms a tree rooted at the rector V. E. Tretyakov. In order to make the party funny for every one, the rector does not want both an employee and his or her immediate supervisor to be present. The personnel office has evaluated conviviality of each employee, so everyone has some number (rating) attached to him or her. Your task is to make a list of guests with the maximal possible sum of guests' conviviality ratings.

**Input**

Employees are numbered from 1 to N. A first line of input contains a number N. 1 <= N <= 6 000. Each of the subsequent N lines contains the conviviality rating of the corresponding employee. Conviviality rating is an integer number in a range from -128 to 127. After that go T lines that describe a supervisor relation tree. Each line of the tree specification has the form:
L K
It means that the K-th employee is an immediate supervisor of the L-th employee. Input is ended with the line
0 0

**Output**

Output should contain the maximal sum of guests' ratings.

**Sample Input**

```
7
1
1
1
1
1
1
1
1 3
2 3
6 4
7 4
4 5
3 5
0 0
```

**Sample Output**

```
5
```



题目大意：

公司决定开展一场舞会，选取一些员工参加，每个员工有一定快乐值（可能为负），要求员工和其直接上司不能同时参加，求可能达到的最大快乐值。员工之间的所属关系构成一棵树。

思路：

树上dp

设$dp[i,0]$表示以$i$节点为根的子树中，不选择$i$节点的最大快乐值。

$dp[i,1]$表示以$i$节点为根的子树中，选择$i$节点的最大快乐值。

转移思路也很简单，选择$i$时，$i$的子节点都不能选，不选$i$时，其子节点可选可不选。
$$
\begin{align}
&dp[u,1] = ∑dp[v,0]\\
&dp[u,0] = ∑max(dp[v,0], dp[v,1])
\end{align}
$$
转移方程中$v$表示$u$的所有子节点。

具体上，输入时记录每个员工“是否有上司”，然后找到没有上司的节点$root$作为跟节点，向下dfs更新得到$dp[root,0],dp[root,1]$值，取其较大值作为贡献。

需注意的是，本题输入边时不是直接输入$n-1$条边，而是以00结束，意味着这不一定只是一棵树，所以要考虑森林的情况，对于每个没有上司的节点都做一遍dfs，统计总贡献。

注意有多组测试数据。

```c++
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <vector>
#include <map>
using namespace std;
#define ll long long
const ll maxn = 100015;
ll mode = 1000000007;

struct Edge{
    int t,nt;
}e[maxn*2];

int n;
int val[maxn];
int ecnt=0, head[maxn];
int dp[maxn][2];
bool isroot[maxn];

void add(int x,int y){
    e[++ecnt].t=y;
    e[ecnt].nt=head[x];
    head[x]=ecnt;
}

void dfs(int p){
    dp[p][1]=val[p];
    dp[p][0]=0;
    for(int i=head[p];i;i=e[i].nt){
        int v=e[i].t;
        dfs(v);
        dp[p][0]+=max(dp[v][0],dp[v][1]);
        dp[p][1]+=dp[v][0];
    }
}

void solve(){

    ecnt = 0;
    for(int i=1;i<=n;i++){
        isroot[i]=true;
        head[i]=0;
        scanf("%d",&val[i]);
    }
    int ipx,ipy;
    while(1){
        scanf("%d%d",&ipx,&ipy);
        if(ipx==0 && ipy==0) break;
        add(ipy,ipx);
        isroot[ipx]= false;
    }

    int ans = 0;
    for(int i=1;i<=n;i++){
        if(isroot[i]){
            dfs(i);
            ans += max(dp[i][0], dp[i][1]);
            break;
        }
    }
    printf("%d\n",ans);

}

signed main() {
#ifdef ACM
    freopen("x.txt","r",stdin);
#endif

    while(scanf("%d",&n)!=EOF){
        solve();
    }

    return 0;
}
```

