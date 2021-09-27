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

也可以用在 sort 里面

```c++
vector<int> v;
sort(v.begin(), v.end());//常用的对vector排序
```



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

