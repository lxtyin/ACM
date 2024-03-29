处理遍历问题，环问题的有力**思考方式**

![](https://oi-wiki.org/graph/images/dfs-tree.svg)

考虑dfs过程经过的这棵树，我们会走很多树边并且遇到很多非树边。

对于有向图而言，非树边有连儿子、连祖先、横跨边三种情况，分析起来会很麻烦（有向图不缩点直接跑本来就很麻烦）

而对于无向图而言就简单的多了，只可能存在连祖先的非树边。

分析无向图环问题时，可以在dfs生成树上进行，每个返祖的非树边代表了一个简单环，并且图中所有环都是这些简单环的**线性组合**，这个性质会非常有帮助。

另外，把图变成了树，就可以更方便地搞树剖、差分之类的了。