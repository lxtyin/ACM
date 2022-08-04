## STL探究



#### 迭代器

迭代器基本就是指针

```c++
vector<int>::iterator it = t.begin();//声明迭代器
```

所有容器的 $.begin().end()$ 方法都能获取容器第一个元素，最后一个元素的下一位的迭代器

```c++
for(set<int>::iterator it = t.begin(); it != t.end(); it++){
	cout << *it << '\n';
}//输出set中所有元素
```

可以看到获取元素值（*it）和枚举（it++）都和指针是一样的

迭代器分为两种：顺序型（vector, set等），节点型（map, set等）

两种迭代器都可以自增或自减（节点型也有遍历顺序），但只有顺序型可以进行加减操作（ it+2，it1-it2 等）

另外有反向迭代器 $.rend(), rbegin()$，$rbegin()$ 指向最后一个元素，$rend()$ 指向第一个元素的前一个元素。反向迭代器的加减会反过来。



用迭代器遍历时，插入一般不会有问题，删除时，被删除元素的迭代器会变得无效，不能对无效的迭代器做加减操作。正确做法：

节点型：$set.erase(it++)$ 这么写，删除 $it$ 后它仍能正确移动到下一位。

顺序型：$it=set.erase(it)$ erase 会返回删除元素的下一个位置的迭代器。



#### set

其实是一棵红黑树，插入时自动去重

感觉比较怪的地方是，set 的 begin() 获取的是 set 中最小的元素迭代器

然后 set 的迭代器++是走向其后继，是 $logn$ 的

正常这样用就是按序输出

```c++
for(set<int>::iterator it = t.begin(); it != t.end(); it++){
    cout << *it << '\n';
}//输出set中所有元素
```

另外，其 find 等方法，如果没有找到，就会返回一个和 end() 相等的迭代器。



#### map

以前一直有个错误理解，以为map是靠“==”符号判断两个key是否相等的

直到有一次拿结构体做key时，被坑了。。

结构体作map的key时，不需要重载等于号，但是需要重载小于号，map中依据小于号排序

注意：两个元素 $a, b$，map认为他们相等的条件其实是：$a<b$ 不成立 且 $b<a$ 不成立

被坑的代码：

```c++
struct node{
    int cs[12];
    ll num;
    bool operator <(const node &x)const{
        return num < x.num;
    }
    bool operator ==(const node &x)const{
        if(num != x.num) return false;
        for(int i=1;i<=n;i++) if(cs[i] != x.cs[i]) return false;
        return true;
    }
};
```

这里重载等于号是完全没有用的，而重载小于的时候仅仅依赖 $num$ 判断，也就是说在map中，两个 $num$ 相同而 $cs$ 值不同的元素将被视为同一个。

```c++
struct node{
    int cs[12];
    ll num;
    bool operator <(const node &x)const{
        if(num == x.num){
            for(int i=1;i<=n;i++){//num相同时，依据cs随便定一个规则排序
                if(cs[i] != x.cs[i])//否则map中会将两个同num的node视为相同，即便他们cs不同
                    return cs[i] < x.cs[i];
            }
        }
        return num < x.num;
    }
};
```

正确搞法，在 $num$ 相同时，随便定一个规则来按照 $cs$ 排序，保证不同元素不会同时满足 $!(a<b)$ 和 $!(b<a)$



#### lower_bound和upper_bound

```C++
lower_bound(起始位置，结束位置，比较值)
```

寻找`[起始位置, 结束位置)` 上大于等于比较值的第一个位置，返回的是地址或迭代器，若未找到则返回结束位置

upper_bound则为寻找大于值的第一个

```c++
lower_bound(起始位置，结束位置，比较值，条件)
```

寻找`[起始位置, 结束位置)` 上第一个不符合条件的位置，upper_bound也是这样

这个条件函数的两个参数中的第二个是比较值，其中一段源码大概长这样：

```C++
while (__len > 0) {
  _DistanceType __half = __len >> 1;
  _ForwardIterator __middle = __first;
  std::advance(__middle, __half);
  if (__comp(__middle, __val))//主要看这里，比较值在第二个参数
      ...
```

正常使用时，需要重载的运算符是 `枚举值<比较值`（我也不知道怎么描述 看例子：

```c++
struct node{
	int l, r;
};
vector<node> s;
lower_bound(s.begin(), s.end(), 3);
```

我试图在 `s` 中找到第一个 `l>=3` 的node，需要的运算符是：`node < int`



#### emplace_back

和push_back的本质区别在于，它直接在目标位置构造，而非先构造一个对象，再移动

使用上，emplace_back可以直接传入参数列表，自动调用相应对象的初始化列表

```c++
vector<pair<int, int>> v;
v.push_back({1, 2});
v.emplace_back(1, 2);
```





#### bitset

用法整理：

```c++
bitset<n> b;//初始化为0
bitset<n> b(x);//初始化为unsigned long类型的二进制表示
bitset<n> b(s);//用字符串初始化

b.any();//是否存在1
b.count();//1的个数
b.size();//总长度
b[x];//第x位（最低位为0）的数
b.set();//全部置1
b.reset();//全部置0
b.flip();//全部取反
b.to_ulong();//返回unsigned long值
```

同时，位运算对bitset都适用。
