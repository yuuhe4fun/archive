# Pseudo-Randomly Interleaved Memory 

[Pseudo-Randomly-Interleaved-Memory.pdf](./docs/Pseudo-Randomly-Interleaved-Memory.pdf)

## Introduction

Interleaved Memory 交叉存储通常用于提供多个存储instance分摊访问，以实现多处理器并行或高性能单处理器所需带宽的提高。

交叉存储的有效性在于：

- 在一定的交叉存储映射下，访问地址pattern是否具有集聚性
- 当某种pattern下具有集聚性时（如访问集聚在某些bank），交叉存储的性能会迅速下降
- 因此需要设计让交叉存储的映射能够适应更多的pattern

## 常规交叉存储模式

- 处理器内存系统结构

![](./assets/image-20220902105454801.png)

### 顺序交叉存储

![](./assets/image-20220902101111742.png)

### 质数交叉存储

![](./assets/image-20220902101050243.png)

### 偏置存储机制

顺序交叉存储和质数交叉存储问题的关键在于这两种方法一定程度上存在周期性pattern，因此有理由找到对应的访问序列映射在同一个module从而产生集聚，降低带宽。为了解决这一问题，提出了偏置存储机制。虽然这种方法在很大程度上仍然存在集聚现象，如当stride是M*M的倍数的时候，地址会被映射到同一个module bank，但是该方法提出的置换思路可以进一步发展为pseudo-randomly伪随机机制

![](./assets/image-20220902101013806.png)

### buffering缓冲

对于M个module（bank），如果能够均匀散列到m个bank，那么就能够获得m/M的总带宽;

## Pseudo-Randomly Interleaving 伪随机交叉存储模式

$$
B=h(A)
$$

其中，A即physical address物理地址，B即randomized address随机映射后的地址：
$$
A=<{a}_{n-1},...,a_0>,
B=<b_{n-1},...,b_0>
$$
一个较好的伪随机hash函数应具有如下特性：

- 相同的原地址必须映射到相同的随机地址
- 映射应该是一对一的
- 原地址可以被映射到任意给定的随机地址，避免存储的浪费
- 需要尽量避免局部集聚性
- 当$a_{n-1}$,…$a_m$保持常数 的时候，$a_{m-1}$,…, $a_0$遍历2m种组合，$b_{m-1}$,…, $b_0$也应该遍历2m种组合
- 计算必须简单

> 伪随机函数的关键 {a[n-1:m], a[m-1:0]}  -> b[m-1:0]