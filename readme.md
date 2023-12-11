# KuinKer-M

## KuinKer-M 是什么

KuinKer-M是一个基于STL标准库的常用算法库，C++17实现。

KuinKer-M追求简单好用、高鲁棒性，高安全性和低耦合性。

正因如此，用户可以使用KuinKer-M进行高度抽象的思考，而不用顾虑代码上的细节，尤其是将同一数据在多个算法框架下进行连续计算处理的时候。

## KuinKer-M 架构

在KuinKer-M中，每一种或者一系列算法都分别封装在了一个类中。

用户可以根据想要解决的问题，查阅本算法库的文档，选择自己想要使用的算法，创建相应的对象并调用该对象下的函数解决问题。

## KuinKer-M 算法

1. 背包类（01背包、完全背包、多重背包）
2. 迪杰斯特拉最短路算法
3. TSP旅行商问题
4. LIS/LDS
5. 高精度整数
6. 并查集
7. 分块（查和，区间查询，区间修改）
8. 哈希（基于平板电视）
9. 优先队列（基于平板电视）
10. 旋转树（基于平板电视）
11. 可变长数组（基于平板电视）
12. 树状数组
13. 线段树
14. Link-Cut Tree
15. ST表
16. 字符串处理(分割与哈希)
17. 字典树
18. 博弈论
19. 数论基础（快速幂，最大公因数等）
20. 素数判定
21. 素因子提取
22. 组合数计算
23. 二分判定
24. Tarjan
24. ... ...TBC

## KuinKer-M 规范

1. 尽量不使用宏定义等不具备类型安全性的语句。
2. 类名、函数名及所有变量名采用驼峰命名法，但若只有一个单词则首字母大写，若只有一个字母则小写；常量全部大写。
3. 高度封装，除了变量和函数会加注详细的注释以外，函数内算法不加注任何注释。
4. 优先使用KuinKer-M内以及标准库中的容器作为数据结构储存和转发数据。

## KuinKer-M 使用

### 施工中，第一版发布后会同时发布API文档