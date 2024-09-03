# 视觉SLAM十四讲-笔记



## 第一讲 预备知识

https://gaoyichao.com/Xiaotu/







## 第四章 李群与李代数

https://blog.fseasy.top/2022/03/vslam14c-4th/



## 第五讲 相机与图像



https://blog.csdn.net/hl_zzl/article/details/84944494 

reverse 反向，reserve储留

https://zhuanlan.zhihu.com/p/344823955

https://cslinzhang.github.io/home/#Education:



## 第六讲 非线性优化

Ceres库主要求解无约束或有界约束最小二乘问题，形式如下（带边界的核函数最小二乘）：
$$
\min_x\quad\frac12\sum_i\rho_i\left(\left\|f_i\left(x_1,\cdots,x_k\right)\right\|^2\right)
\\
\mathrm{s.t.}\quad l_j\leq x_j\leq u_j
$$
我们的任务是找到一组满足约束$l_j\leq x_j \leq u_j$的$x_i,\cdots,x_k$，使得优化目标函数最小。

只要能把实际问题描述成上述的数学形式，就可以使用Ceres求解，提供**代价函数**计算方式，描述**残差块**以及**核函数**即可。

使用ceres求解最小二乘问题，需要提供三个对象：

> ceres::Problem problem
>
> ceres::Solver::Options options
>
> ceres::Solver::Summary summary

Ceres的求解过程包括构建最小二乘和求解最小二乘两部分，构建问题方法包含在Ceres::Problem类中，涉及的成员函数主要包括Problem::AddResidualBlock()和Problem::AddParameterBlock()

Ceres只接受最小二乘优化，若要对残差添加权重，则要对权重矩阵做Cholesky分解。



使用Ceres求解非线性优化问题，主要包括以下部分：

> 1. 构建优化问题ceres::Problem
> 2. 构建代价函数ceres::CostFunction























