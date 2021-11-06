### Const

```c++
const int x;//变量不能改
const int *x;//指向的变量内容不能改，即使指向的是普通int
int const *p//p的指向不能改，要赋初值
bool operator <(const node &x)//常见，const+临时对象+引用 的写法，节省时间，对于非对象（int,double)等这个const没必要
struct node{
    int getSum()const{//这个const表示函数不会修改任何东西
        ...
    }
};

```

