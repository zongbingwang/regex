说明文档
开发语言：
C++
编译运行环境：
Win 8+ CodeBlocks 13.12 + gcc
类模块说明：
Trans类：
功用：NFA中的边

NFA_Node类：
功用：非确定有穷状态自动机的节点
主要方法:
Void AddTrans(Trans* tt)   添加边，封装了STL的push_back

NFA类：
功用：非确定有穷状态自动机表示
主要属性：
NFA_Node* start  起始状态
NFA_Node* end 	终止状态

Converter类：
功用：将正则表达式转换成NFA
主要方法：
NFA  ToNFA()    转换至NFA
bool  Operator_Less_Than(char t1, char t2)		比较运算符优先级
NFA& Connect(NFA G1, NFA G2)            处理&
NFA& Union(NFA G1, NFA G2)   			处理 |
NFA& Closure(NFA G2,char temp)            处理 *
NFA& Cloqure(NFA G2)					处理 ？
NFA& Cloaure(NFA G2,char temp) 			处理+
NFA& Clopure(NFA G2,char temp,int k) 	    处理{}

DFA_Node类：
功用：	确定有穷状态自动机的节点

DFA_Edge类：
功用：确定有穷状态自动机的边	
           		
NFA_To_DFA类：
功用：将NFA转化到DFA
主要方法：
NFA_Node*& find(int st) 		在NFA中寻找从st点出发的边		
DFA_Node*& finddfa(int st)		在DFA中寻找从st点出发的边			
void findclosure(NFA_Node* p)	寻找空边
int AddStatus()				添加DFA状态
void Convert()					NFA转化到DFA

Regex类:
功用：匹配正则
主要方法：
 string deal_brace(string s)    		处理()和转义的情况
 void match(string s,string *result)	匹配字符串
核心原理：
利用Thompson's Construction 将正则表达式转化为NFA，该理论工作原理如下：
下面这个自动机定义了一个简单的正则表达式
  
接下来的自动机定义了两种连接方式
 
 
例如
*号有由下面的定义
 
+号： 
建立了NFA后，利用消除ε边算法以及构造DFA法将其转化为DFA，从而进行分析。


完成情况：
实现的功能：
除了text29.txt测试用例的其他用例都可以成功实现
未实现的功能：
text29.txt中的{}未实现，但是自己测试实现了分支功能
