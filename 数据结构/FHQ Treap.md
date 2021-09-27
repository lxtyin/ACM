## FHQ Treap

FHQ Treap 维护BST性质的关键是**merge的时候一定要让左树小于右树**

inline 啥的优化自己看着加 ==

```c++
struct Treap{
    int l,r;
    int val, siz;
    int pri;
}t[maxn*2];
int tcnt = 0;
int root = 0;

int newNode(int val){
    t[++tcnt].siz = 1;
    t[tcnt].pri = rand();
    t[tcnt].val = val;
    return tcnt;
}

void push_up(int p){
    t[p].siz = t[t[p].l].siz + t[t[p].r].siz + 1;
}

void split(int p,int &l,int &r,int k){
    if(!p){
        l=r=0;
        return;
    }
    if(t[p].val <= k){
        l = p;
        split(t[p].r, t[p].r, r, k);
    }else{
        r = p;
        split(t[p].l, l, t[p].l, k);
    }
    push_up(p);
}

//合并时需注意l子树都小于r子树
//merge处尝试了引用式：void merge(int &p, int l, int r)，并不比这种快
int merge(int l,int r){
    if(!l || !r) return l+r;

    if(t[l].pri > t[r].pri){
        t[l].r = merge(t[l].r,r);
        push_up(l);
        return l;
    }else{
        t[r].l = merge(l,t[r].l);
        push_up(r);
        return r;
    }
}

//按val-1拆成两半，加一个val点，再依次合并
void insert(int val){
    if(root == 0){
        root = newNode(val);
        return;
    }
    int x,y;
    split(root,x,y,val-1);
    int nw = newNode(val);
    x = merge(x,nw);
    root = merge(x,y);
}

void delet(int val){
    int x,y,z;
    split(root,x,y,val);//大于val的被y筛走了
    split(x,x,z,val-1);//小于等于val-1的被x筛走了，z中全是val
    z = merge(t[z].l,t[z].r);
    x = merge(x,z);
    root = merge(x,y);
}

//小于val的数量
int getrank(int val){
    int x,y;
    split(root,x,y,val-1);
    int res = t[x].siz + 1;
    root = merge(x,y);
    return res;
}

//第k大值
int getkth(int p,int k){
    if(t[t[p].l].siz == k-1) return t[p].val;
    if(t[t[p].l].siz >= k) return getkth(t[p].l, k);
    else return getkth(t[p].r, k-t[t[p].l].siz-1);
}

//小于val的最大数
int precursor(int val){
    int x,y;
    split(root,x,y,val-1);
    int res = getkth(x,t[x].siz);
    root = merge(x,y);
    return res;
}

//大于val的最小数
int nextcursor(int val){
    int x,y;
    split(root,x,y,val);
    int res = getkth(y,1);
    root = merge(x,y);
    return res;
}

//貌似也可
//int precursor(int val){
//    return getkth(root, getrank(val)-1);
//}
//int nextcursor(int val){
//    return getkth(root, getrank(val+1));
//}

void solve(){
    int n;
    scanf("%d",&n);
    int opt,x;
    while(n--){
        scanf("%d%d",&opt,&x);
        if(opt == 1) insert(x);
        if(opt == 2) delet(x);
        if(opt == 3) printf("%d\n",getrank(x));
        if(opt == 4) printf("%d\n",getkth(root,x));
        if(opt == 5) printf("%d\n",precursor(x));
        if(opt == 6) printf("%d\n",nextcursor(x));
    }
}
```





FHQ treap还能替代某毒瘤数据结构进行区间操作

[【模板】文艺平衡树](https://www.luogu.com.cn/problem/P3391)

下文复制自某博客，加以个人注释

> 学 Fhq 就是为了尽量不去写某毒瘤数据结构,所以自然要来杠一杠某数据结构的经典操作:区间反转
>
> 听起来玄乎,但只需要一个小 trick 就行了:把原来的区间以下标作为权值建成 Treap , 这样整棵 Treap 的中序遍历就是原区间.
>
> 按照这种方法建树,是进行区间操作的第一步.接下来我们考虑如何去在 Θ(log2n)Θ(log2⁡n) 的时间内完成这件事.
>
> 一个基本的思路是将区间 Split 为 [1,l-1],[l,r],[r+1,n] 三部分,对中间的 [l,r] 进行反转
>
> 反转的具体操作是从根到叶子把每个节点的左右儿子互换
>
> 显然,这样复杂度十分糟糕,甚至达到了暴力都比不上的 Θ(n×log2n)Θ(n×log2⁡n)
>
> 所以,我们必须考虑去减少我们的操作次数.
>
> 这里我们借鉴一下之前学习线段树时的 trick : lazytag (我不信你都学平衡树了还不会线段树)
>
> 聪明的你应该已经想到了,对没错,就是通过打 lazytag 来减少我们的操作,想必原理也不用赘述
>
> (这里有个小细节,最后输出前,别忘了把所有的 tag 全部下传到底)
>
> 那么什么时候去下传 tag 呢 ? 聪明的你肯定也已经想到了,对,就是在 merge 和 Split 两个函数中,优先下传 tag

补充：

- 普通平衡树的中序遍历是递增数列，忽略了数的位置信息。而这种的中序遍历即为原序列
- 这个平衡树的权值是不满足 BST 性质的！满足性质的其实是**它在序列中的位置**
- 接上一条，所以在 split 的时候一定不能按权值分，要按 size 值分且向下推的时候注意修改分界点，详见代码
- 上面三点说的其实是一个东西 **但真的很重要**
- 作为一种**区间树**，可以像线段树一样进行区间操作和使用lazytag，虽然树会被不断地重排，但因为我们在任一个节点split和merge前都进行了push_down，tag已经下传，所以并不会打乱我们对区间的操作
- 所以只要将 $[l,r]$​ 区间的树split出来，就可以放心大胆地对其进行区间操作（其他区间加，查询啥的也和旋转一样）

```c++
int root = 0;
struct Treap{
    int l, r;
    int val, siz;
    int pri, tag;
}t[maxn*2];
int tcnt = 0;

inline int newNode(int val){
    t[++tcnt].siz = 1;
    t[tcnt].pri = rand();
    t[tcnt].val = val;
    return tcnt;
}

inline void push_up(int p){
    t[p].siz = t[t[p].l].siz + t[t[p].r].siz + 1;
}

inline void push_rot(int p){
    if(!p) return;
    swap(t[p].l, t[p].r);
    t[p].tag ^= 1;
}

void push_down(int p){
    if(!t[p].tag) return;
    push_rot(t[p].l);
    push_rot(t[p].r);
    t[p].tag = 0;
}

void split(int p, int &l, int &r, int k){
    if(!p){
        l = r = 0;
        return;
    }
    push_down(p);
    if(t[t[p].l].siz+1 <= k){
        l = p;
        split(t[p].r, t[p].r, r, k-t[t[p].l].siz-1);
    }else{
        r = p;
        split(t[p].l, l, t[p].l, k);
    }
    push_up(p);
}

int merge(int l, int r){
    if(!l || !r) return l+r;
    if(t[l].pri > t[r].pri){
        push_down(l);
        t[l].r = merge(t[l].r, r);
        push_up(l);
        return l;
    }else{
        push_down(r);
        t[r].l = merge(l, t[r].l);
        push_up(r);
        return r;
    }
}

void rotate(int l, int r){
    int x, y, z;
    split(root, x, y, l-1);
    split(y, y, z, r-l+1);
    push_rot(y);
    root = merge(x, y);
    root = merge(root, z);
}

void dfs(int p){
    if(!p) return;
    push_down(p);
    dfs(t[p].l);
    cout << t[p].val << '\n';
    dfs(t[p].r);
}

void solve(){
    int n,m;
    cin >> n >> m;
    root = newNode(1);
    for(int i=2;i<=n;i++){
        int x = newNode(i);
        root = merge(root, x);
    }
    for(int i=1;i<=m;i++){
        int x, y;
        cin >> x >> y;
        rotate(x, y);
    }
    dfs(root);
}
```