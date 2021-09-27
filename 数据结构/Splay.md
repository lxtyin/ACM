## Splay

基本操作是rotate（将一个节点转上一层）和splay（将一个节点向上旋到根为止）

每次访问一个节点，都将其Splay，所以 splay 用于处理存在部分数据高频访问的问题更有优势

其他各种操作大多是普通的BST操作

注意更新节点信息：rotate时需要将相关的点都push_up

而splay就是一堆rotate，所以有splay的地方，就不需要push_up了，否则需要注意

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
    inline void splay(int p){//有修改的地方，有splay就不需要push_up
        while(fa[p] != 0){
            if(get_dir(fa[p]) == get_dir(p)) rotate(fa[p]);
            else rotate(p);
            rotate(p);
        }
        rt = p;
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

要做区间操作，首先不能按照权值构建平衡树了，要按照下标构建，这里涉及修改BST的排序方式，要从原本的”左子树权值均小于根，右子树权值均大于根“ 改为 ”左子树下标均小于根，右子树下标均大于根“。

思考：Splay的BST性质依赖什么维持的？如果想更改，要改哪些？

其实：在每次插入的时候我们都按照BST性质找到插入位置，后面的所有操作都只会单纯地维持当前树的左右关系，而不涉及具体的BST顺序。

所以只要改insert就好了。
