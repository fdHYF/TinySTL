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
##### vector<int>
|Container|Size|Times(ms)|
|-----|-------|---|-----|
|TinySTL::vector<int>|10万|1|
 |TinySTL::vector<int>|100万|8|
 |TinySTL::vector<int>|1000万|141|
 |std::vector<int>|10万|1|
 |std::vector<int>|100万|8|
 |std::vector<int>|1000万|109|
