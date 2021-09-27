## AC自动机

传统板子（已经沦为暴力了

常备称为暴力跳fail搞法

```c++
int ans[maxn];
struct Tire{
    int ch[30];
    int fail;
    int edid;//以此为终点的模式串id
    int edcnt;//以此为终点的模式串数量
} t[maxn];
int tcnt = 1;

inline int newNode(){//自带初始化的
    int nw = ++tcnt;
    for(int i = 0;i < 28;i++) t[nw].ch[i] = 0;
    t[nw].fail = 0;
    t[nw].edcnt = 0;
}

inline void insert(string s, int id){
    int ln = s.size(), p = 1;
    for(int i = 0;i < ln;i++){
        int c = s[i] - 'a';
        if(!t[p].ch[c]) t[p].ch[c] = ++tcnt;//不带初始化
        p = t[p].ch[c];
    }
    t[p].edid = id;
    t[p].edcnt++;
}

//感觉fail指针更像一个工具，不如叫next指针更合适（其指向的是与自身相同的节点）
//暴力跳fail的过程相当于遍历了当前串的后缀
void getFail(){
    for(int i = 0;i < 28;i++) t[0].ch[i] = 1;//小细节，很好地处理了边界，免得特判
    queue<int> q;
    q.push(1);//1为根，0是为了避免特判边界搞的点
    while(!q.empty()){
        int u = q.front();
        int faf = t[u].fail;
        q.pop();
        for(int i = 0;i < 26;i++){
            int v = t[u].ch[i];
            //把所有的空节点都连到可行点，不如说这才是真正的失配指针
            if(!v) t[u].ch[i] = t[faf].ch[i];
            else t[v].fail = t[faf].ch[i], q.push(v);
        }
    }
}

void Match(string s){
    int ln = s.size();
    int p = 1;
    for(int i = 0;i < ln;i++){
        p = t[p].ch[s[i] - 'a'];
        int v = p;
        while(v){//暴力跳fail
           	//操作，这里是计算模式串出现次数
            ans[t[v].edid]++;
            v = t[v].fail;
        }
    }
}

string s;
void solve(){
    int n;
    cin>>n;
    for(int i = 1;i <= n;i++){
        cin >> s;
        insert(s, i);
    }
    getFail();
    cin >> s;
    Match(s);
    for(int i = 1;i <= n;i++) cout << ans[i] << '\n';
}
```

加入lazytag思想，就不用每次都跳所有fail指针了

fail指针构成的边组合起来就是一张有向无环图，所有点的lazytag都向后push，非常好写

而且AC自动机莫种意义上来说是离线的（先处理所有模式串最后再统计答案），所以只需要最后一遍push所有lazytag就可以了，方法是拓扑排序

既然用到了lazytag，自然可以写成线段树那种形式（将单点统计和加tag合并到一起作为元操作）

注意：AC自动机的tire边和fail指针分别构成有向无环图（除了0节点fail指针为自环），但放一起就不是了

[求模式串出现次数](https://www.luogu.com.cn/problem/P5357)

```c++
int pos[maxn];
struct Tire{
    int ch[30];
    int fail, ans;
    int edcnt;//以此为终点的模式串数量
    int tag, indu;//用于拓扑
} t[maxn];
int tcnt = 1;

inline int push_add(int p, int ad){
    t[p].ans += ad;
    t[p].tag += ad;
}

inline int newNode(){
    int nw = ++tcnt;
    for(int i = 0;i < 28;i++) t[nw].ch[i] = 0;
    t[nw].fail = 0;
    t[nw].ans = 0;
    t[nw].edcnt = 0;
    t[nw].tag = 0;
    return nw;
}

inline void insert(char s[], int id){
    int ln = strlen(s), p = 1;
    for(int i = 0;i < ln;i++){
        int c = s[i] - 'a';
        if(!t[p].ch[c]) t[p].ch[c] = newNode();
        p = t[p].ch[c];
    }
    pos[id] = p;
    t[p].edcnt++;
}

void getFail(){
    for(int i = 0;i < 28;i++) t[0].ch[i] = 1;
    queue<int> q;
    q.push(1);
    while(!q.empty()){
        int u = q.front();
        int faf = t[u].fail;
        q.pop();
        for(int i = 0;i < 26;i++){
            int v = t[u].ch[i];
            if(!v) t[u].ch[i] = t[faf].ch[i];
            else{
                t[t[faf].ch[i]].indu++;
                t[v].fail = t[faf].ch[i], q.push(v);
            }
        }
    }
}

void Match(char s[]){
    int ln = strlen(s);
    int p = 1;
    for(int i = 0;i < ln;i++){
        p = t[p].ch[s[i] - 'a'];
        push_add(p, 1);
    }
}

void Topu(){
    queue<int> q;
    for(int i=2;i<=tcnt;i++){
        if(t[i].indu == 0) q.push(i);
    }
    while(!q.empty()){
        int u = q.front(); q.pop();
        int v = t[u].fail;
        if(v == 1) continue;
        push_add(v, t[u].tag);
        t[u].tag = 0;
        t[v].indu--;
        if(t[v].indu == 0) q.push(v);
    }
}

char s[maxn*10];
void solve(){
    int n;
    cin>>n;
    for(int i = 1;i <= n;i++){
        cin >> s;
        insert(s, i);
    }
    getFail();
    cin >> s;
    Match(s);
    Topu();
    for(int i = 1;i <= n;i++) cout << t[pos[i]].ans << '\n';
}
```