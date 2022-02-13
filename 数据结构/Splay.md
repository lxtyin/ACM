## Splay

基本操作是rotate（将一个节点转上一层）和splay（将一个节点向上旋到根为止）

每次访问一个节点，都将其Splay，所以 splay 用于处理存在部分数据高频访问的问题更有优势

其他各种操作大多是普通的BST操作

rotate就是将当前节点向上旋转一层；

而splay就是一直把当前节点rotate到根为止，注意如果当前节点和其父节点的方向相同，需要先旋转父节点（俗称双旋），否则会被卡到 $O(n)$（画一个链就卡掉了qwq）

上旋的时候要注意push_up，向下查找时注意push_down（有懒标记的话）

```c++
struct Splay
{
    int rt, tot;//根节点，总数
    int son[maxn][2], fa[maxn], siz[maxn];
    int val[maxn], cnt[maxn];
    void init(){ rt = tot = 0;}
    inline void push_up(int p){ siz[p] = siz[son[p][0]] + siz[son[p][1]] + cnt[p];}
    inline bool get_dir(int p){ return son[fa[p]][1] == p;}//返回节点是左节点还是右节点（0左1右）
    inline void clear(int p){ son[p][0] = son[p][1] = fa[p] = siz[p] = val[p] = cnt[p] = 0;}

    inline int newNode(int vl){
        val[++tot] = vl;
        siz[tot] = cnt[tot] = 1;
        son[tot][0] = son[tot][1] = fa[tot] = 0;
        return tot;
    }

    inline void rotate(int p){
        if(fa[p] == 0) return;
        int lf = get_dir(fa[p]), lp = get_dir(p), ls = lp ^ 1;
        int s = son[p][ls], f = fa[p], ff = fa[f];
        son[p][ls] = f; fa[f] = p;
        son[f][lp] = s; fa[s] = f;
        son[ff][lf] = p; fa[p] = ff;
        push_up(f); push_up(p);//ff不用push_up，子树没变
        clear(0);//不存在的连接都是0，一般要多写if防止0节点的信息被改变，这里直接清空省的判
    }
    void splay(int p, int tar = 0){
        while(fa[p] != tar){
            if(fa[fa[p]] == tar || get_dir(p) != get_dir(fa[p])) rotate(p);
            else rotate(fa[p]);
        }
        if(!tar) rt = p;
    }

    int find(int vl){
        int p = rt;
        while(val[p] != vl){
            if(val[p] > vl) p = son[p][0];
            else p = son[p][1];
        }
        splay(p);
        return p;
    }

    int get_rk(int x){
        int p = rt, res = 0;
        while(val[p] != x){
            if(val[p] < x){
                res += siz[son[p][0]] + cnt[p];
                p = son[p][1];
            }else{
                p = son[p][0];
            }
        }
        res += siz[son[p][0]];
        splay(p);
        return res + 1;
    }

    int get_kth(int k){
        int p = rt;
        while(k > 0){
            if(siz[son[p][0]] >= k){
                p = son[p][0];
            }else{
                k -= siz[son[p][0]] + cnt[p];
                if(k <= 0){
                    splay(p);
                    return val[p];
                }
                p = son[p][1];
            }
        }
    }

    //返回的是根节点的前驱，查找x的前驱的步骤是：insert(x)，precursor，delet(x)
    int precursor(){
        int p = son[rt][0];
        if(p == 0) return -1;//不存在
        while(son[p][1] != 0) p = son[p][1];
        splay(p);
        return val[p];
    }

    int nextcursor(){//同理
        int p = son[rt][1];
        if(p == 0) return -1;//不存在
        while(son[p][0] != 0) p = son[p][0];
        splay(p);
        return val[p];
    }

    void insert(int x){
        if(!rt){
            rt = newNode(x);
            return;
        }
        int p = rt;
        while(1){
            if(x == val[p]){
                cnt[p]++;
                splay(p);
                return;
            }
            int f = p, dir = (x > val[p]);
            p = son[p][dir];
            if(p == 0){
                p = newNode(x);
                son[f][dir] = p;
                fa[p] = f;
                splay(p);
                return;
            }
        }
    }

    void delet(int x){
        int p = find(x);//此时p已经为rt了
        if(cnt[p] > 1){
            cnt[p]--;
            push_up(p);
            return;
        }
        if(!son[p][0] || !son[p][1]){
            rt = son[p][0] + son[p][1];
            fa[son[p][0]] = fa[son[p][1]] = 0;
            clear(p);
            return;
        }
        precursor();//把前驱挪到根
        son[rt][1] = son[p][1];
        fa[son[p][1]] = rt;
        clear(p);
        push_up(rt);
    }
}t;
```



Splay 的一个重要操作是区间翻转

要做区间操作，首先不能按照权值构建平衡树了，要按照下标构建，我们可以把实际的权值存放起来，但是不能单纯地把下标当成一种值存起来！还是因为要区间翻转！

下标就是：左子树大小+1 ！

然后就是对区间操作，首先可以通过将l-1旋转到根，再将r+1旋转到根的方法筛出`[l, r]`这个区间，然后的操作和FHQ搞法一样，更改时用到懒标记，注意在向下查找时，经过的点需要下传标记。



[洛谷模板](https://www.luogu.com.cn/problem/P3391)

```C++
int n;
int a[maxn];

struct Splay{
    int rt, tot = 0;
    int ch[maxn][2], fa[maxn], val[maxn], siz[maxn], swp[maxn];
    inline int dir(int p){ return ch[fa[p]][1] == p;}
    inline void push_up(int p){ siz[p] = siz[ch[p][0]] + siz[ch[p][1]] + 1;}
    inline void push_down(int p){
        if(swp[p]){
            swp[p] = 0;
            swap(ch[p][0], ch[p][1]);
            swp[ch[p][0]] ^= 1;
            swp[ch[p][1]] ^= 1;
        }
    }
    int build(int l, int r){	//省略了插入，直接根据数组建树，类似线段树的建树
        if(l > r) return 0;
        int p = ++tot;
        int mid = (l + r) / 2;
        val[p] = a[mid];
        swp[p] = 0;
        ch[p][0] = build(l, mid-1);
        ch[p][1] = build(mid+1, r);
        fa[ch[p][0]] = fa[ch[p][1]] = p;
        push_up(p);
        return p;
    }
    void rotate(int p){
        if(fa[p] == 0) return;
        int f = fa[p], ff = fa[f];
        int d = dir(p), s = ch[p][d ^ 1];
        fa[s] = f;
        fa[p] = ff; ch[p][d ^ 1] = f;
        fa[f] = p; ch[f][d] = s;
        if(ff) ch[ff][ch[ff][1] == f] = p;
        push_up(f); push_up(p);
        fa[0] = ch[0][0] = ch[0][1] = 0;
    }
    void splay(int p, int tar = 0){	//splay部分需要注意：祖父节点是目标位置时，不能再旋转父节点。
        while(fa[p] != tar){
            if(fa[fa[p]] == tar || dir(p) != dir(fa[p])) rotate(p);
            else rotate(fa[p]);
        }
        if(!tar) rt = p;
    }
    int find(int k){
        int p = rt;
        while(true){
            push_down(p);
            if(siz[ch[p][0]] >= k){
                p = ch[p][0];
            }else{
                if(siz[ch[p][0]] + 1 == k) return p;
                k -= siz[ch[p][0]] + 1;
                p = ch[p][1];
            }
        }
    }
    void reverse(int l, int r){
        int x = find(l-1);
        int y = find(r+1);
        splay(x, 0);
        splay(y, x);
        swp[ch[ch[rt][1]][0]] ^= 1;
    }
    void print(int p){
        if(!p) return;
        push_down(p);
        print(ch[p][0]);
        if(0 < val[p] && val[p] <= n)
        cout << val[p] << ' ';
        print(ch[p][1]);
    }
}t;

void solve(){
    int q;
    cin >> n >> q;
    for(int i=1;i<=n+2;i++) a[i] = i-1;
    t.rt = t.build(1, n+2);
    while(q--){
        int l, r;
        cin >> l >> r;
        t.reverse(l+1, r+1);
    }
    t.print(t.rt);
}
```
