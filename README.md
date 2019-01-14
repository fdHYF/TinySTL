# TinySTL
此项目是学习《STL源码剖析》自己实现的一个简易版本的STL
---------
### 项目目的
- 学习STL
- 练习算法和模板元编程
### 开发环境
- Win10 + Code::Blocks
### 测试环境
- Win10 Centos
### 进度
#### 顺序容器
- vector
- list 
- deque
- priority_queue
- queue
- stack

#### 关联容器
- map
- set
- multiset
- multimap
### 性能测试
测试环境为Win10+Code::Blocks+release模式
#### vector
##### vector&lt,int>
|Container|Size|Times(ms)|
|-----|----------|-----|
|TinySTL::vector&lt;int>|10万|1|
 |TinySTL::vector&lt;int>|100万|8|
 |TinySTL::vector&lt;int>|1000万|141|
 |std::vector&lt;int>|10万|1|
 |std::vector&lt;int>|100万|8|
 |std::vector&lt;int>|1000万|109|
