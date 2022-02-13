## Link Cut Tree



LCT类似树剖，尝试通过维护链上信息来解决树上问题。但它不限制重儿子必须是子树最大的儿子，而是可以随时变化的（称为实儿子），故它可以支持删边，连边等操作，能够维护一个森林，也叫动态树。

由于实链不断变化，不能再像树剖一样用线段树维护，而是改用splay维护，这棵splay按照深度构建，也就是说它的中序遍历就是原树中从上到下的一条链。

（其他太多懒得写了 贴 https://blog.csdn.net/qq_36551189/article/details/79152612

https://www.cnblogs.com/flashhu/p/8324551.html 这位大佬的图非常好，完全展示了“把链当集合来看待”的思想

------

写一下代码实现中我理解比较困难的部分

首先不要忘了splay是一棵平衡树，它是一个集合！不要把它想成树！把它简化成一个集合来看就会好理解很多。



一个集合中仅有一条**fa连出去的虚边**，它从splay的根节点连出去，但实际上，在原树中，这个虚边是从深度最小的那个点（也就是splay中序遍历最早的点）连出去的！事实上我们不需要关注它是从splay的哪个节点连出去的，因为splay只是一个集合，这条虚边的含义是：这条链的起始点的父亲在哪里



实际上写LCT只需要建一棵Splay就可以了，它包含多个根节点，好消息是我们完全不用管根节点

初始时这个Splay的所有节点都是独立的，连实边时我们让两个节点互认父子关系，他们就合并到一个集合（链）上了；

连`x,y`间一条虚边的时候，让 **$y$ 所在的splay根节点**认 $x$ 作父亲，但父亲不认儿子（这里比较神奇），这样 $xy$ 仍属于不同的集合，这条虚边表示的是原树上的关系

这样的话，实边转虚边也非常容易了，直接由父亲单方面接触关系即可；虚边转实边则可以无视虚边的存在重新连一遍



另外还有一点：由于access的操作是由下到上的，不便于push_down，所以一般要改造一下splay，先用一个栈预判路径，从上到下push_down一遍。



理解的时候需要分清楚splay的根节点和原树的根节点，搞清楚就好理解了（详见下面代码注释

```C++
struct LCT{
    int ch[maxn][2], fa[maxn], val[maxn], sum[maxn], swp[maxn];
    inline int dir(int p){ return ch[fa[p]][1] == p;}
    inline void push_up(int p){ sum[p] = sum[ch[p][0]] ^ sum[ch[p][1]] ^ val[p];}
    inline bool isroot(int p){ return fa[p] == 0 || (ch[fa[p]][0] != p && ch[fa[p]][1] != p);} //是否为splay的根（不是原树的根！）
    inline void push_swap(int p){
        if(!p) return;
        swap(ch[p][0], ch[p][1]), swp[p] ^= 1;
//        push_down(p);//调试用 立即下放所有懒标记
    }
    inline void push_down(int p){
        if(swp[p]){
            swp[p] = 0;
            push_swap(ch[p][0]);
            push_swap(ch[p][1]);
        }
    }
    void rotate(int p){//出bug了必须首先来查rotate 注意变化前后顺序的问题 太麻了
        if(fa[p] == 0) return;
        int f = fa[p], ff = fa[f];
        int d = dir(p), s = ch[p][d ^ 1];
        if(!isroot(f)) ch[ff][dir(f)] = p;//与普通splay不同之处：如果父亲节点已经是根节点，一定不能让ff认儿子
        fa[s] = f;
        fa[p] = ff; ch[p][d ^ 1] = f;
        fa[f] = p; ch[f][d] = s;
        push_up(f);//最后一个节点的push_up在splay那
    }
    void push_all(int p){
        if(!isroot(p)) push_all(fa[p]);
        push_down(p);
    }
    void splay(int p){//将p转到其所在splay（实链节点集合）的根节点
        push_all(p);//从顶向下push_down
        while(!isroot(p)){
            if(!isroot(fa[p])) rotate(dir(p) == dir(fa[p]) ? fa[p] : p);
            rotate(p);//双旋，不管旋了父还是子，第二次旋转都要跟上，不然会假
        }
        push_up(p);
    }
    void access(int p){//打通p所在真实树的根节点到p的路径
        int lst = 0;
        while(p){
            splay(p);
            ch[p][1] = lst;
            push_up(p);
            lst = p;
            p = fa[p];
        }
    }
    void makeroot(int p){//p变为其所在真实树中的根节点（同时也是splay根）
        access(p);
        splay(p);
        push_swap(p);
    }
    int findroot(int p){//查找所在真实树的根节点，注意这个也会改变p所在splay结构
        access(p);
        splay(p);
        while(ch[p][0]) push_down(p), p = ch[p][0];
        return p;
    }
    void split(int x, int y){//将x,y之间的路径剖出来作为splay，然后x作为原树根，y作为splay根
        makeroot(x);
        access(y);
        splay(y);
    }
    void link(int x, int y){//连边（虚边）
        makeroot(x);
        if(x != findroot(y)) fa[x] = y;//注意，连虚边一定要让所在splay的根节点的fa连出去，这里x已经是splay根节点了而y不一定
    }
    void cut(int x, int y){
        if(findroot(x) != findroot(y)) return;
        split(x, y);//记牢此时x为原树根，y为splay根，原树根仅仅是深度最小的节点，完全可以有fa
        if(fa[x] != y || ch[x][1]) return;//要x,y之间有连边，就要它们在splay的中序遍历上相邻
        ch[y][0] = fa[x] = 0;
        push_up(y);
    }
}lct;

void solve(){
    int n, m;
    cin >> n >> m;
    for(int i=1;i<=n;i++){
        cin >> lct.val[i];
        lct.sum[i] = lct.val[i];
    }
    while(m--){
        int tp, x, y;
        cin >> tp >> x >> y;
        if(tp == 0){
            lct.split(x, y);
            cout << lct.sum[y] << '\n';
        }else if(tp == 1){
            lct.link(x, y);
        }else if(tp == 2){
            lct.cut(x, y);
        }else{
            lct.splay(x);//先转到根再改，省的上传
            lct.val[x] = y;
            lct.push_up(x);
        }
    }
}
```





