# Jd-STL
基于侯捷的《STL源码剖析》  
C++ STL 模板实现  
## STL 六大组件
1. 容器（containers）
2. 算法（algorithms）
3. 迭代器（iterators）
4. 仿函数（functors）
5. 配接器（adapters）
6. 配置器（allocators）

*六大组件的交互关系：Container通过Allocator取得数据储存空间，Algorithm通过Iterator存取Container内容，Functor可以协助Algorithm完成不同的策略变化，Adapter可以修饰或套接Functor。*

<p>STL的中心思想在于：将数据容器和算法分开彼此独立设计，最后再以一贴胶着剂将它们撮合在一起。</p>