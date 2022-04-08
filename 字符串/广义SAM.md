## 有一说一 没懂

但是直观上来讲好像又确实是这么回事....

[偶然发现的巨佬](https://www.cnblogs.com/Xing-Ling/p/12038349.html)



对于多个串，我们先构建字典树，然后再按照bfs顺序依次插入到SAM中...就构成了广义SAM

感觉...好像...挺对的

就是感觉有点太理所当然了...不敢信

也不难猜测其性质，基本和SAM差不多：

- 它应该能匹配多个串中的任意子串吧
- 它的节点依然还是等价类，表示若干长度连续，同后缀的子串，并且这些子串在所有串上出现的endpos相同

毕竟这是个自动机，把字符合理的插入进去它自动就是对的吧（

```C++
struct tire{
    int ch[28];//我认为还是把tire的边和sam的边分开比较好 省的判一些奇怪东西
    int fa;
}tr[maxn];
int tcnt = 0;

void insert(string &s){
    int p = 0;
    for(int i=0;i<s.size();i++){
        int v = s[i] - 'a';
        if(!tr[p].ch[v]){
            tr[p].ch[v] = ++tcnt;
            tr[tcnt].fa = p;
        }
        p = tr[p].ch[v];
    }
}

struct node{
    int ch[29];
    int fa, len, cnt;//cnt为该状态下endpos个数，等于该集合下任意子串出现次数
    void clear(){
        for(int i=0;i<28;i++) ch[i] = 0;
        fa = len = cnt = 0;
    }
}sam[maxn * 2];

void expend(int lst, int c){
    int cur = tr[lst].ch[c];//仅仅改了这一处，lst为父节点
    //和tire共用节点编号即可，点是一一对应的，sam中除了tire的点之外还有克隆的点
    int p = lst;
    sam[cur].len = sam[p].len+1;
    sam[cur].cnt = 1;

    while(!sam[p].ch[c]){
        sam[p].ch[c] = cur;
        p = sam[p].fa;
        if(p == -1){
            sam[cur].fa = 0;
            return;
        }
    }

    int q = sam[p].ch[c];
    if(sam[p].len+1 == sam[q].len){
        sam[cur].fa = q;
    }else{
        int clo = ++tcnt;
        sam[clo] = sam[q];
        sam[clo].len = sam[p].len+1;
        sam[clo].cnt = 0;
        sam[q].fa = clo;
        sam[cur].fa = clo;
        while(p != -1 && sam[p].ch[c] == q){
            sam[p].ch[c] = clo;
            p = sam[p].fa;
        }
    }
}

void buildGam(){
    sam[0].fa = -1;//init();
    queue<int> q;
    q.push(0);
    while(!q.empty()){
        int p = q.front();
        q.pop();
        for(int i=0;i<26;i++){
            if(tr[p].ch[i]){
                q.push(tr[p].ch[i]);
                expend(p, i);
            }
        }
    }
}
```

板子它莫名其妙的就过了

---

上述做法是离线做法，还有不建字典树的直接插入做法。在线做法在很多应用上会更简单（因为很清晰的知道当前是哪个串在插入）

做法就是：每个串依次插入sam，并且在一个串开始插入时将last指向根节点。

直接这么写好像也能过很多题，但它并不严谨，会产生空节点和一个等价类拆分成多个节点的情况，所以需要加上一些特判

这里建议看[巨佬博客](https://www.cnblogs.com/Xing-Ling/p/12038349.html)，详细将了怎么特判才能用在线做法造出一个和离线做法一模一样的自动机~~（甚至他还给出了卡盗版的方案）~~

~~ps：私以为在线搞法在各种情况下都比离线做法好~~ 所以直接抄大佬的在线思路了，这一份板子同样对拍过，大量数据下都和离线做法产生的自动机节点数相同。

这处特判可能没那么好理解，可以从“返回值要作为下一个lst”入手，我们希望返回的值必须是一个前缀。

具体见注释：

```C++
struct node{
    int ch[29];
    int fa, len;
    void clear(){
        for(int i=0;i<28;i++) ch[i] = 0;
        fa = len = 0;
    }
}sam[maxn * 2];
int tcnt = 0;

//在插入一个字符后，返回包含 当前串下以这个字符为终点的前缀 的等价类
//也即下一次做lst的节点。另外返回值是前缀这件事也能有所应用
int expend(int lst, int c){
    if(sam[lst].ch[c]){//如果已经存在这一点
        int q = sam[lst].ch[c], p = lst;
        //并且它的maxlen就等于当前前缀，那么它即前缀，直接返回
        if(sam[p].len + 1 == sam[q].len) return q; 
        else{
            //否则说明存在这样的点，但它可能更长，也就是说当前前缀和它是两个等价类
            //且当前前缀是其fa，要插入其中，此处逻辑其实和下面的克隆是一样的，详见sam里我画的那个图
            int clo = ++tcnt;
            sam[clo] = sam[q];
            sam[clo].len = sam[p].len+1;
            sam[q].fa = clo;
            while(p != -1 && sam[p].ch[c] == q){
                sam[p].ch[c] = clo;
                p = sam[p].fa;
            }
            return clo;
        }
    }
    //上面是加了的特判，下面没有区别

    int cur = ++tcnt;
    int p = lst;
    sam[cur].len = sam[p].len+1;

    while(!sam[p].ch[c]){
        sam[p].ch[c] = cur;
        p = sam[p].fa;
        if(p == -1){
            sam[cur].fa = 0;
            return cur;
        }
    }

    int q = sam[p].ch[c];
    if(sam[p].len+1 == sam[q].len){
        sam[cur].fa = q;
    }else{
        int clo = ++tcnt;
        sam[clo] = sam[q];
        sam[clo].len = sam[p].len+1;
        sam[q].fa = clo;
        sam[cur].fa = clo;
        while(p != -1 && sam[p].ch[c] == q){
            sam[p].ch[c] = clo;
            p = sam[p].fa;
        }
    }
    return cur;
}


void solve(){
    int n;
    cin >> n;
    sam[0].fa = -1;
    for(int i=1;i<=n;i++){
        string s;
        cin >> s;
        int lst = 0;
        for(char c: s) lst = expend(lst, c-'a');
    }
    cout  << tcnt << '\n';
}
```



## 应用

##### 1.求多串中不同子串个数

还是把各个节点的 $len-fa.len$ 加起来就行了

**2.多串间最长公共子串**

对每个节点开一个长度为 $n$ （字符串总数）的标记数组，标记这个节点是否包含了第 $i$ 个串的子串。

建立时用在线写法（离线也行但比较麻烦），每次插入的点打上当前这个串的标记，所有串都插入完之后，惯例在parentTree上让父节点继承子节点的标记，最后扫一遍所有点，拥有所有串标记的点即为公共子串，拿它的len来更新答案。

**重点说一下这些标记**：在单个串的SAM上，如果我们要维护endpos，只需要在前缀所在的等价类（非克隆得到的节点）上打标记，然后parentTree上推即可得到所有等价类的endpos。克隆的节点等价类中不含前缀，利用这个性质可以不重复不遗漏。

然后是多串的EXSAM，如果是这种在线构造法，其实也是将作为前缀的点打标记，不作前缀的不打。那么选择要返回的节点打标记即可。



```C++
struct node{
    int ch[29];
    int fa, len, vis;
}sam[maxn * 2];
int tcnt = 0;
int nowv;

//返回值为下一次做lst的节点
int expend(int lst, int c){
    if(sam[lst].ch[c]){
        int q = sam[lst].ch[c], p = lst;
        if(sam[p].len + 1 == sam[q].len){
            sam[q].vis |= nowv;//这个标记要打
            return q;
        }else{
            int clo = ++tcnt;
            sam[clo] = sam[q];
            sam[clo].len = sam[p].len+1;
            sam[q].fa = clo;
            while(p != -1 && sam[p].ch[c] == q){
                sam[p].ch[c] = clo;
                p = sam[p].fa;
            }
            sam[clo].vis |= nowv;//这个标记要打
            return clo;
        }
    }
    //上面是加了的特判，下面没有区别

    int cur = ++tcnt;
    int p = lst;
    sam[cur].len = sam[p].len+1;
    sam[cur].vis |= nowv;//这个标记要打

    while(!sam[p].ch[c]){
        sam[p].ch[c] = cur;
        p = sam[p].fa;
        if(p == -1){
            sam[cur].fa = 0;
            return cur;
        }
    }

    int q = sam[p].ch[c];
    if(sam[p].len+1 == sam[q].len){
        sam[cur].fa = q;
    }else{
        int clo = ++tcnt;//这个标记不能打！！！
        sam[clo] = sam[q];
        sam[clo].len = sam[p].len+1;
        sam[q].fa = clo;
        sam[cur].fa = clo;
        while(p != -1 && sam[p].ch[c] == q){
            sam[p].ch[c] = clo;
            p = sam[p].fa;
        }
    }
    return cur;
}

void solve(){
    sam[0].fa = -1;
    string s;
    nowv = 1;
    while(cin >> s){
        int lst = 0;
        for(char c: s) lst = expend(lst, c-'a');
        nowv <<= 1;
    }

    vector<int> rk(tcnt);
    for(int i=0;i<tcnt;i++) rk[i] = i+1;
    sort(rk.begin(), rk.end(), [](int x, int y){
        return sam[x].len > sam[y].len;
    });
    for(int i: rk) sam[sam[i].fa].vis |= sam[i].vis;

    int ans = 0;
    for(int i=1;i<=tcnt;i++) if(sam[i].vis == nowv-1) ans = max(ans, sam[i].len);
    cout << ans << '\n';
}
```



