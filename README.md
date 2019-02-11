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
|Container|Size|Times(ms)|
|-----|----------|-----|
|TinySTL::vector&lt;int>|10万|1|
 |TinySTL::vector&lt;int>|100万|8|
 |TinySTL::vector&lt;int>|1000万|141|
 |std::vector&lt;int>|10万|1|
 |std::vector&lt;int>|100万|8|
 |std::vector&lt;int>|1000万|109|
 
 |Container|Size|Times(ms)|
 |--------|------|-----|
 TinySTL::vector&lt;std::string>|10万|5|
 TinySTL::vector&lt;std::string>|100万|47|
 TinySTL::vector&lt;std::string>|1000万|579|
 |std::vector&lt;std::string>|10万|5|
 |std::vector&lt;std::string>|100万|46|
 |std::vector&lt;std::string>|1000万|579|
 
|Container|Size|Times(ms)|
|-----|----------|-----|
|TinySTL::list&lt;int>|10万|2|
 |TinySTL::list&lt;int>|100万|18|
 |TinySTL::list&lt;int>|1000万|148|
 |std::list&lt;int>|10万|5|
 |std::listr&lt;int>|100万|65|
 |std::list&lt;int>|1000万|826|
 
