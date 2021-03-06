### 数理基础

#### 函数正交

正交是向量的概念，对于函数来说，如果我们对一个函数在 $[a,b]$ 上取样 $m$ 次，那么它可以表示为一个 $m$ 维的向量。两个函数如果在 $[a,b]$ 上逐点相乘，结果恰好为0，我们可以称它们是正交的。

理论上，$n$ 维空间中只需要取 $n$ 个线性不相关的向量，就能表示任意向量了，用两两正交的 $n$ 个向量可以简化问题。

函数也是如此，正弦函数和余弦函数是正交的（从定义上容易看出来），后面会用到。

#### 复平面

首先，我们可以在复平面上用向量表示一个复数

复数相加满足向量加法的性质， 你可以画一个平行四边形

复数乘法则满足一个更玄妙的规律：模长相乘，辐角相加

模长相乘很好理解，幅角相加即：我把其中一个向量转到x轴正方向上，另一个向量也要跟着转同样角度，最终位置即为乘积向量的角度。

那么 $2^i$ 这个复数对应的向量在哪里呢？它在单位向量顺时针旋转





### 傅里叶变换

大佬：https://www.zhihu.com/question/19714540

傅里叶告诉我们，任意一个周期函数可以表示为无数个余弦函数之和（当然正弦也一样）。



一个函数，它有两种表现方式：

时域：将函数的x轴看做时间轴，函数（通常我们说的是周期函数）的图像，是我们通常看到的函数图像

频域：将x轴看做频率，频域上某一点 $F(x)$ 的含义是：原函数在频率为 $x$ 的余弦函数和正弦函数上的分量

具体而言，我们拿一个频率为 $v$ 余弦函数去和函数 $f(x)$ 逐点相乘（内积），得到的结果之和 $sum$ 可以衡量这个 $f(x)$ 和频率为 $v$ 的余弦函数的相似程度。就拿这个 $sum$ 作为频域上 $v$ 点的一部分。

如果时域和频域能够一一对应且能相互转换，我们就可以随便转换，在容易处理的域上为所欲为，最后再变回来就行了。

比如多项式，系数表示法对应于频域，点值表示法对应于时域。两个多项式相乘在点值表示法下可以 $O(n)$ 进行，算法竞赛中常用的FFT正是利用了这一点，加速了多项式的乘法。

---

在复平面上的理解：

我认为傅里叶变换在复平面上要好理解的多。







### 快速傅里叶变换

用途：两个多项式乘积的快速计算

首先需要知道多项式有两种表示方式，一种是用 $n+1$ 个系数表示，另一种是用 $n+1$ 个点值来表示。

任取 $n+1$ 个不重复的点，都能唯一确定一个 $n$ 阶多项式

证明：~~举不出反例，易得~~

那么我们可以取一组相同的 $x_{1...n}$，分别代入两个多项式求得其点值表示，对应点值相乘不就得到新多项式了吗（

FFT分两个步骤，DFT和IDFT，它们可以在 $nlogn$ 的时间里完成多项式系数表示法和点值表示法之间的切换



#### DFT：

如果随便取 $n$ 个坐标代入计算点值，那么总体是 $O(n^2)$ 的

我们希望计算尽可能简单，如果 $x$ 取1和-1，那么它们的次幂会很好算，但这还不够。为了找更多类似1和-1的数，我们不惜扩张数系（

##### 单位复根

在复数域上，我们可以找到大量模长为1的数，但性质可以更好一些。

首先介绍一下复数乘法的几何含义：模长相乘，辐角相加

也就是说任意两个复数相乘，把它们都在复平面上用向量表示，那么它们的乘积即这两个向量的角度叠加，模长乘的结果。

我们最终选取这组 $x$ 点值是：将复平面上的单位圆等分成 $n$ 份，每份取一点

图源oiwiki：

![](https://oi-wiki.org/math/poly/images/fft2.jpg)

设其中第 $i$ 条分界线上的点为 $w_n^i$，我们就要取 $w_n^0...w_n^{n-1}$

这些数有很好的性质，具体而言我们可以考虑复数乘法，$w_n^k$ 就是 $(1,0)$ 这个向量旋转 $k/n$ 个单位圆，那么显然：

- $w_n^{k}=(w_n^1)^k$
- $w_n^k=w_{2n}^{2k}$
- $-w_{2n}^k=w_{2n}^{k+n}$

接下来就是DFT的过程了：

设现有系数表示的多项式为 $f(x)=a_0x+a_1x+...a_nx^n$

我们可以把它分奇偶拆开：

$f(x)=(a_0+a_2x^2+...)+(a_1x+a_3x^3+...)$

右边再提出一个 $x$

$=(a_0+a_2x^2+...)+x(a_1+a_3x^2+...)$

将两边的系数看成新多项式 $G(x)$ 和 $H(x)$ 的系数，那么：

$f(x)=G(x^2)+x\times H(x^2)$

这时候我们尝试将 $w_n^k$ 代入 $x$：

$G(x^2)=G(w_n^{2k})=G(w_{n/2}^k)$ （只需要 $n$ 是偶数即可）

因为 $G$ 和 $H$ 的数据规模本就是 $n/2$，相当于已经成为完全独立的子问题了，我们可以递归求解

另外，还可以把所有的 $w_n^k$ 丢进去一起算，这样就能 $O(nlogn)$ 时间解决了。



最终我们得到了一组 $y$，为这个多项式的点值表示法下的 $y$ 坐标，其 $x$ 取的是这一组 $w_n^k$

#### IDFT：

好消息：点值转系数不需要写多余的代码

我们从矩阵的角度考虑系数转点值 $y$ 的过程，其实是这样的一个变换：（图源oiwiki）

![](pic/1.png)

因为我们始终采用同一套 $x$，在反变换时，是不是让 $y$ 乘上这个矩阵的逆矩阵就可以了（

而这个矩阵的逆矩阵也很好看：就是原矩阵每一项都取倒数，再除 $n$ 

那么矩阵集体除 $n$ 我们先提出来，其实就是每个 $w_n^k$ 都变成 $w_n^{-k}$

所以直接对 $y$ 再进行一次DFT，其中把 $w_n^k$ 变成 $w_n^{-k}$，最后再集体除 $n$ 即可！ 

