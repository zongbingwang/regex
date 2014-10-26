#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<string>
#include<stack>
#include<sstream>
#include<vector>
#include<queue>
#include<set>
#include<algorithm>

using namespace std;

const int MAX=1024;
int T[MAX][MAX];

class NFA_Node;
/*NFA边*/
class Trans
{
	public:
		char incept;
		int ID;
		NFA_Node* des;
		Trans(int ID,char incept,NFA_Node* des)
		{
		    this->ID=ID;
			this->incept=incept;
			this->des=des;
		}
};

/*NFA节点*/
class NFA_Node
{
	public:
		int stateID;
		vector<Trans*> t;
		bool visit;
		NFA_Node(int stateID)
		{
			visit=false;
			this->stateID=stateID;
		}
		void AddTrans(Trans* tt)
		{
			t.push_back(tt);
		}

};

/*NFA图*/
class NFA
{
	public:
		NFA_Node* start;
		NFA_Node* end;
		NFA(){}
		NFA(int SID,int ID,char c)
		{
			NFA_Node* s1=new NFA_Node(SID);
			NFA_Node* s2=new NFA_Node(SID+1);
			Trans* tt=new Trans(ID,c,s2);
			s1->AddTrans(tt);
			start=s1;
			end=s2;
		}

};

/*将正则表达式转到NFA*/
class Converter
{
 	public:
        int S_ID;
        int C[MAX];
        vector <char>* T;
        Converter(string str, vector <char>* P)
        {
            pretreat(str);
            Houzhui(this->lamb);
            S_ID=1;
            T=P;


        }
        Converter(){S_ID=1;}

        void show()
        {
            cout<<this->lamb<<endl;
        }

        NFA ToNFA()
        {
            NFA tempb,tempb1,tempb2;

            char tempc1;
            int i=0;

            while (i<lamb.size())
            {
                tempc1 = lamb[i];
                if (isOperator(tempc1)|| tempc1=='{')
                {
                    switch (tempc1)
                    {
                        case '|':
                            tempb1 = stNFA.top();
                            stNFA.pop();
                            tempb2 = stNFA.top();
                            stNFA.pop();
                            tempb1=Union(tempb2,tempb1);
                            stNFA.push(tempb1);
                            break;
                        case '&':
                            tempb1 = stNFA.top();
                            stNFA.pop();
                            tempb2 = stNFA.top();
                            stNFA.pop();
                            tempb2=Connect(tempb1,tempb2);
                            stNFA.push(tempb2);
                            break;
                        case '*':
                            tempb1 = stNFA.top();
                            stNFA.pop();
                            tempb1=Closure(tempb1,tempc1);
                            stNFA.push(tempb1);
                            break;
                        case '?':
                            tempb1 = stNFA.top();
                            stNFA.pop();
                            tempb1=Cloqure(tempb1);
                            stNFA.push(tempb1);
                            break;
                        case '+':
                            tempb1 = stNFA.top();
                            stNFA.pop();
                            tempb1=Cloaure(tempb1,tempc1);
                            stNFA.push(tempb1);
                            break;
                        case '{':
                            tempb1 = stNFA.top();
                            stNFA.pop();
                            tempb1=Clopure(tempb1,tempc1,i);
                            stNFA.push(tempb1);
                            while (lamb[i]!='}') i++;
                            break;
                    }
                }
                else
                {
                    tempb = NFA(S_ID,T[tempc1][C[tempc1]++],tempc1);
                    S_ID+=2;
                    stNFA.push(tempb);
                }
                i++;
            }
            tempb = stNFA.top();
            stNFA.pop();
            return tempb;
        }


 	private:
        stack<NFA> stNFA;
        stack<char> Operator_Stack;
        string lamb;

        bool isOperator(char c)
        {
            switch (c)
            {
                case '|':
                case '&':
                case '(':
                case ')':
                case '!':
                case '+':
                case '?':
                case '*': return true;
                default: return false;
            }
        }

        bool isNewOperator(char c)
        {
            switch (c)
            {
                case '}':
                case '{':
                case '|':
                case '&':
                case '(':
                case ')':
                case '!':
                case '+':
                case '?':
                case '*': return true;
                default: return false;
            }
        }

        bool isTwoOperator(char c)
        {
            switch (c)
            {
                case '{':
                case '}':
                case '+':
                case '?':
                case '*': return true;
                default: return false;
            }
        }


        int getOperatorNumber(char t1)
        {
            switch (t1)
            {
                case '$': return 0;
                case '!': return 1;
                case ')': return 2;
                case '|': return 3;
                case '&': return 4;
                case '*': return 5;
                case '+': return 6;
                case '?': return 7;
                case '(': return 8;
                default: return 9;
            }
        }

        bool Operator_Less_Than(char t1, char t2)
        {
            int temp1 = getOperatorNumber(t1);
            int temp2 = getOperatorNumber(t2);
            if (temp1 <= temp2)
                return true;
            return false;
        }


        void pretreat(string str)
        {
        	int i=0;
        	char c,pc;
        	bool can_add=true;

            memset(C,0,sizeof(C));
        	pc=str[i];
        	c=str[++i];
        	while(str[i]!='\0')
        	{
        	    if (pc=='{') can_add=false;
        	    if (pc=='}') can_add=true;
        	    if (can_add)
        		if((pc==')'&&c=='(')||(!isNewOperator(pc)&&!isNewOperator(c))||(!isNewOperator(pc)&&c=='(')||(pc==')'&&!isNewOperator(c))||(isTwoOperator(pc)&&!isNewOperator(c))||(isTwoOperator(pc)&&c=='('))//四种情况需要加&
        		str.insert(i,"&");
        		pc=str[i++];
        		c=str[i];
        	}
        	str+="!";
            this->lamb=str;
        }

        int find_min(int k)
        {
            int l=k;
            int i=k;
            while (lamb[i]!='}' && lamb[i]!=',') i++;
            int r=i-1;
            int ans=0, d=1;
            for (i=r;i>=l;i--) if (lamb[i]!='&' ) ans+=d*(lamb[i]-'0'),d*=10;
            return ans;
        }

        int find_max(int k)
        {
            int l=k;
            int i=k;
            while (lamb[i]!=',' && lamb[i]!='}') i++;
            if (lamb[i]=='}') return -1;
            l=i+1;
            while (lamb[i]!='}') i++;
            int r=i-1;
            int ans=0,d=1;
            for (i=r;i>=l;i--) if (lamb[i]!='&' ) ans+=d*(lamb[i]-'0'),d*=10;
            return ans;
        }


        void Houzhui(string lamb)
        {
            string l="";
            Operator_Stack.push('$');
            char tempc,tempc2;

            for(int i=0;i<(int)lamb.size();i++)
            {
                tempc = lamb[i];
                if (isOperator(tempc))
                {
                    switch (tempc)
                    {
                        case '(': Operator_Stack.push(tempc); break;
                        case ')':
                            while (Operator_Stack.top() != '(')
                            {
                                tempc2 = Operator_Stack.top();
                                Operator_Stack.pop();
                                l += tempc2;
                            }
                            Operator_Stack.pop();
                            break;
                        default :
                            tempc2 = Operator_Stack.top();
                            while (tempc2!='('&&Operator_Less_Than(tempc,tempc2))
                            {
                                tempc2 = Operator_Stack.top();
                                Operator_Stack.pop();
                                l += tempc2;
                                tempc2 = Operator_Stack.top();
                            }
                            Operator_Stack.push(tempc);
                            break;
                    }
                }
                else
                    l += tempc;
            }
            this->lamb=l;
        }
        /* 处理 &*/
        NFA& Connect(NFA G1, NFA G2)
        {
			Trans* t=new Trans(-1,'@',G1.start);
			G2.end->AddTrans(t)	;
  			G2.end = G1.end;
  			return G2;
		}
		/* 处理 |*/
        NFA& Union(NFA G1, NFA G2)
        {
			NFA_Node* n1=new NFA_Node(S_ID++);
			Trans* t1=new Trans(-1,'@',G1.start);
			Trans* t2=new Trans(-1,'@',G2.start);
			n1->AddTrans(t1);
			n1->AddTrans(t2);

			NFA_Node* n2=new NFA_Node(S_ID++);
			Trans* t3=new Trans(-1,'@',n2);
			Trans* t4=new Trans(-1,'@',n2);
			G1.end->AddTrans(t3);
			G2.end->AddTrans(t4);

			NFA* G=new NFA();
			G->start=n1;
			G->end=n2;

			return *G;
		}
		/* 处理 * */
        NFA& Closure(NFA G2,char temp)
        {
			Trans* t=new Trans(T[temp][C[temp]],'#',G2.start);
			G2.end->AddTrans(t);

			NFA_Node* n1=new NFA_Node(S_ID++);
			Trans* t1=new Trans(-1,'@',n1);
			G2.end->AddTrans(t1);

			NFA_Node* n2=new NFA_Node(S_ID++);
			Trans* t2=new Trans(-1,'@',G2.start);
			n2->AddTrans(t2);


			Trans* t3=new Trans(-1,'@',n1);
			n2->AddTrans(t3);

			NFA* G=new NFA();
			G->start=n2;
			G->end=n1;

  			return *G;
		}
        /* 处理 ? */
		NFA& Cloqure(NFA G2)
        {

			NFA_Node* n1=new NFA_Node(S_ID++);
			Trans* t1=new Trans(-1,'@',n1);
			G2.end->AddTrans(t1);

			NFA_Node* n2=new NFA_Node(S_ID++);
			Trans* t2=new Trans(-1,'@',G2.start);
			n2->AddTrans(t2);


			Trans* t3=new Trans(-1,'@',n1);
			n2->AddTrans(t3);

			NFA* G=new NFA();
			G->start=n2;
			G->end=n1;

  			return *G;
		}
        /* 处理 + */
		NFA& Cloaure(NFA G2,char temp)
        {
			Trans* t=new Trans(T[temp][C[temp]],'#',G2.start);
			G2.end->AddTrans(t);

			NFA_Node* n1=new NFA_Node(S_ID++);
			Trans* t1=new Trans(-1,'@',n1);
			G2.end->AddTrans(t1);

			NFA_Node* n2=new NFA_Node(S_ID++);
			Trans* t2=new Trans(-1,'@',G2.start);
			n2->AddTrans(t2);

			NFA* G=new NFA();
			G->start=n2;
			G->end=n1;

  			return *G;
		}

        /* 处理 {} */
		NFA& Clopure(NFA G2,char temp,int k)
        {
            Trans* t=new Trans(T[temp][C[temp]],'#',G2.start);
			G2.end->AddTrans(t);

			NFA_Node* n1=new NFA_Node(S_ID++);
			Trans* t1=new Trans(-1,'@',n1);
			G2.end->AddTrans(t1);

			NFA_Node* n2=new NFA_Node(S_ID++);
			Trans* t2=new Trans(-1,'@',G2.start);
			n2->AddTrans(t2);

			NFA* G=new NFA();
			G->start=n2;
			G->end=n1;

  			return *G;
		}
};

/*生成字符集*/
void Display(NFA &G,set<int> &S,vector<NFA_Node*> &V)
{
	queue<NFA_Node*> MyQueue;
	MyQueue.push(G.start);
	char tt;
	while(!MyQueue.empty())
	{
		NFA_Node* tmp=MyQueue.front();
		MyQueue.pop();
		V.push_back(tmp);
		tmp->visit=true;

		if(tmp->t.size()>0)
		for(int i =0;i<tmp->t.size();i++)
		{
			tt=tmp->t[i]->incept;
			if(tt!='@'&&tt!='#')
			S.insert(tmp->t[i]->ID);
			if(tt=='@')
			{
				if(tmp->t[i]->des->visit==false)
				{MyQueue.push(tmp->t[i]->des);tmp->t[i]->des->visit=true;}
			}
			else if(tt=='#')
			{

			}
			else
			{
				if(tmp->t[i]->des->visit==false)
				{MyQueue.push(tmp->t[i]->des);tmp->t[i]->des->visit=true;}
			}
		}
	}
}

class DFA_Edge;

/*DFA节点*/
class DFA_Node
{
	public:
		int stateID;
		vector<DFA_Edge*> t;
		vector<int> ori;
		bool flag;
		DFA_Node(int s)
		{
			flag=false;
			stateID=s;
		}

		DFA_Node()
		{

		}
};

/*DFA边*/
class DFA_Edge
{
	public:
		int incept;
		DFA_Node* des;
		bool visit;
		DFA_Edge(int a,DFA_Node* &b)
		{
			incept=a;
			des=b;
			visit=false;
		}
};

/*NFA转到DFA*/
class NFA_To_DFA
{
	public:
		int MaxStatus;
		int visit[MAX];
		vector<NFA_Node*> tmp;
		set<int> Alpha;
		vector<NFA_Node*> nfa;
		NFA_Node* start;
		vector<DFA_Node*> dfa;
		int cando;

		NFA_To_DFA()
		{

		}

        void ready(int max,NFA_Node* &S,int cando)
		{
			this->MaxStatus=max;
			this->start=S;
			this->cando=cando;
		}

		void clear_all()
		{
		    Init();
			for(int i=0;i<=MAX;i++)
			for(int j=0;j<=MAX;j++)
			T[i][j]=0;
			Alpha.clear();
			nfa.clear();
			dfa.clear();
		}

		void Init()
		{
			for(int i=0;i<=this->MaxStatus;i++)
			{
				this->visit[i]=0;
			}
			this->tmp.clear();
		}

		NFA_Node*& find(int st)
		{
			for(int i=0;i<nfa.size();i++)
			if(nfa[i]->stateID==st) return nfa[i];
		}

		DFA_Node*& finddfa(int st)
		{
			for(int i=0;i<dfa.size();i++)
			if(dfa[i]->stateID==st) return dfa[i];
		}

		void findclosure(NFA_Node* p)
		{
			visit[p->stateID]=1;
			visit[0]++;
			if(p->t.size()==0) return;
			for(int i=0;i<p->t.size();i++)
			{
				if(!visit[p->t[i]->des->stateID]&&(p->t[i]->incept=='#'||p->t[i]->incept=='@'))
				findclosure(p->t[i]->des);
			}
		}

		void closure()
		{
			for(int i=0;i<tmp.size();i++)
			{
				findclosure(tmp[i]);
			}
		}

		int AddStatus()
		{
		    if(visit[0]==0)
			return -2;
			for(int i=1;i<=T[0][0];i++)
		    {
				if(visit[0]!=T[i][0])
				continue;
		    	int j=1;
				for(;j<=MaxStatus;j++)
		        if(visit[j]!=T[i][j])
		                    break;
		        if(j==(MaxStatus+1))
		                return i;
		    }
		    T[0][0]++;
		    for(int i=0;i<=MaxStatus;i++)
		        T[T[0][0]][i]=visit[i];


		    return -1;
		}

		void moveto(int st,int c)
		{

			for(int i=1;i<=MaxStatus;i++)
			{
				if(T[st][i])
				{
					NFA_Node* p=find(i);
					if(p->t.size()>0)
					for(int j=0;j<p->t.size();j++)
					if(p->t[j]->ID==c)
					{tmp.push_back(p->t[j]->des);}
				}
			}
		}
		/*转换算法*/
		void Convert()
		{
			int i,j;

			findclosure(start);
			AddStatus();
			DFA_Node* s1=new DFA_Node(1);
			if(visit[cando]) s1->flag=true;
			for(i=1;i<=MaxStatus;i++)
			if(visit[i]) s1->ori.push_back(i);

			dfa.push_back(s1);
			Init();

			for(i=1;i<=T[0][0];i++)
			{
				for(set<int>::iterator t1=Alpha.begin();t1!=Alpha.end();t1++)
				{
					moveto(i,*t1);
					closure();
					if((j=AddStatus())>=0)
					{
						DFA_Edge* e1=new DFA_Edge(*t1,finddfa(j));
						finddfa(i)->t.push_back(e1);

					}
					else if(j==-1)
					{
						DFA_Node* d1=new DFA_Node(T[0][0]);
						if(visit[cando]) d1->flag=true;
						for(int tt=1;tt<=MaxStatus;tt++)
						if(visit[tt]) d1->ori.push_back(tt);
						dfa.push_back(d1);
						DFA_Edge* e1=new DFA_Edge(*t1,finddfa(T[0][0]));
						finddfa(i)->t.push_back(e1);
					}

					Init();
				}
			}
		}

};

/*正则表达式主体*/
class Regex
{
public:
    string str;
    bool F[MAX];
    char S[MAX];
    bool accept;
    bool word_match, word_not_match, text_start, text_end, word_start, word_end, line_start, line_end,deal_line,deal_word;
    NFA_To_DFA ntd;

    vector<char> G[MAX];
    vector<char> T[MAX];
    vector< pair<int,int> > ans;

    Regex()
    {

    }

    Regex(string s)
    {
        clear_all();
        ready(deal_brace(s));
        str=S;
    }

    ~Regex()
    {

    }

    string deal_brace(string s)
    {
        stack <int> T;
        vector<pair<int,int> > brace;
        string st;
        char sr[MAX];
        int v=0;
        memset(sr,0,sizeof(sr));

        for (int i=0;i<s.length();i++)
        {
            if (s[i]=='(')
            {
                T.push(v++);
                brace.push_back(make_pair(i,i));
            }
            if (s[i]==')')
            {
                brace[T.top()].second=i;
                T.pop();
            }
        }

        int l=0;

        for (int i=0;i<s.length();i++)
        {
            if (i+1<s.length() && s[i]=='\\' && s[i+1]>='1' && s[i+1]<='9')
            {
                for (int j=brace[s[i+1]-'0'-1].first;j<=brace[s[i+1]-'0'-1].second;j++)
                {
                    sr[l++]=s[j];
                }
                i++;
            }
            else sr[l++]=s[i];
        }
        st=sr;
        return st;
    }


    void clear_all()
    {
        memset(F,false,sizeof(F));
        memset(S,0,sizeof(S));
        for (int i=0;i<MAX;i++)
        {
            G[i].clear();
            T[i].clear();
        }
        ans.clear();
        accept=false;
        ntd.clear_all();

        word_match=false;
        word_not_match=false;
        line_end=false;
        line_start=false;
        deal_line=false;
        deal_word=false;

        text_start=false;
        text_end=false;
        word_start=false;
        word_end=false;
    }

    void ready(string s)
    {
        int l=0;
        int r=0;

        while (l<s.length())
        {
            if (s[l]=='.')
            {
                F[r]=false;
                S[r]='a';
                T['a'].push_back(r);
                put_in(r,'\n','\n');
                l++;
                r++;
                continue;
            }

            if (s[l]=='\\')
            {
                l++;
                switch (s[l])
                {
                case 'n':
                case 't':
                case 'w':
                case 's':
                case 'd': F[r]=true; S[r]='a';T['a'].push_back(r);
                }

                switch (s[l])
                {
                case 'W':
                case 'S':
                case 'D': F[r]=false; S[r]='a';T['a'].push_back(r);
                }

                switch(s[l])
                {
                case 'n': put_in(r,'\n','\n');break;
                case 't': put_in(r,'\t','\t');break;
                case 'w': put_in(r,'a','z'),put_in(r,'A','Z'),put_in(r,'0','9'); break;
                case 'W': put_in(r,'a','z'),put_in(r,'A','Z'),put_in(r,'0','9'); break;

                case 's': put_in(r,'\t','\t'),put_in(r,'\n','\n'),put_in(r,' ', ' '); break;
                case 'S': put_in(r,'\t','\t'),put_in(r,'\n','\n'),put_in(r,' ', ' '); break;

                case 'd': put_in(r,'0','9'); break;
                case 'D': put_in(r,'0','9'); break;

                case 'b': word_match=true,r--,deal_word=true; break;
                case 'B': word_not_match=true,r--,deal_word=true; break;
                }
                l++;
                r++;
                continue;
            }

            if (s[l]=='^')
            {
                deal_line=true;
                text_start=true;
                l++;
                continue;
            }

            if (s[l]=='$')
            {
                deal_line=true;
                text_end=true;
                l++;
                continue;
            }

            if (s[l]=='[')
            {
                l++;
                if (s[l]=='^')
                {
                    F[r]=false;
                    l++;
                }
                else
                    F[r]=true;
                while (s[l]!=']')
                {
                    if (s[l+1]=='-') put_in(r,s[l],s[l+2]),l=l+3;
                    else put_in(r,s[l],s[l]),l++;
                }
                S[r]='a';
                T['a'].push_back(r);
                l++;
                r++;
                continue;
            }
            F[r]=true;
            S[r]=s[l];
            T[S[r]].push_back(r);
            G[r].push_back(S[r]);
            l++;
            r++;
        }
    }

    void put_in(int r, char x, char y)
    {
        for (char i=x;i<=y;i++)
            G[r].push_back(i);
    }

    void parse()
    {
        Converter cnt(str,T);
		NFA result=cnt.ToNFA();
		ntd.ready(cnt.S_ID-1,result.start,result.end->stateID);
		Display(result,ntd.Alpha,ntd.nfa);
		ntd.Convert();

    }

    bool is_w(char c)
    {
        if ('0'<=c && c<='9') return true;
        if ('a'<=c && c<='z') return true;
        if ('A'<=c && c<='Z') return true;
        return false;
    }

    bool is_b(char c)
    {
        switch (c)
        {
        case ' ':
        case '\n':
        case '\t': return true;
        }
        return false;
    }

    bool check_word_start(int k)
    {
        if (!is_w(str[k])) return false;
        if (k==0) return true;
        if (is_w(str[k-1])) return false;
        return true;
    }

    bool check_word_end(int k)
    {
        if (!is_w(str[k])) return false;
        if (k==str.length()) return true;
        if (is_w(str[k+1])) return false;
        return true;
    }

    bool check_text_start(int k)
    {
        if (k==0) return true;
        if (str[k-1]=='\n') return true;
        return false;
    }

    bool check_text_end(int k)
    {
        if (k+1==str.length()) return true;
        if (str[k+1]=='\n') return true;
        return false;
    }


    void match(string s,string *result)
    {
        accept=false;
        str=s;
        for (int i=0;i<s.length();i++)
        {
            word_start=check_word_start(i);
            line_start=check_text_start(i);
            if (deal_line && text_start && !line_start) continue;
            if (deal_word && word_start && word_not_match) continue;

            dfs(ntd.dfa[0],i,i);
            if (accept) break;
        }

		int Max = 0;
		if (!accept) return;
		for (int i = 0; i<ans.size(); i++)
		if (ans[i].second - ans[i].first + 1>Max) Max = ans[i].second - ans[i].first + 1;
		*result=str.substr(ans[0].first, Max);
    }

    void dfs(DFA_Node* p, int k, int st)
    {
        if (p->flag)
        {
            bool can_go=true;

            if (word_match)
            {
                word_end=check_word_end(k-1);
                if (!word_start && !word_end) can_go=false;
            }

            if (word_not_match)
            {
                word_end=check_word_end(k-1);
                if (word_start || word_end) can_go=false;
            }

            if (text_end)
            {
                line_end=check_text_end(k-1);
                if (!line_end) can_go=false;
            }

            if (can_go)
            {
                ans.push_back(make_pair(st, k-1));
                accept=true;
            }
        }

        if (k==str.length()) return;

        for (int i=0;i<p->t.size();i++)
        {
            int u=p->t[i]->incept;
            DFA_Node* f=p->t[i]->des;
            if (F[u])
            for (int j=0;j<G[u].size();j++)
            {
                char c=G[u][j];
                if (str[k]!=c) continue;
                k++;
                dfs(f,k,st);
                k--;
            }
            else
            {
                bool flag=false;
                for (int j=0;j<G[u].size();j++)
                {
                    char c=G[u][j];
                    if (str[k]==c) flag=true;
                    if (flag) break;
                }
                if (flag) continue;
                k++;
                dfs(f,k,st);
                k--;
            }
        }
    }
};

/*文件操作*/
void read_file(string *read, const char *a);
void output_file(string out,string a);
void search(const char* rejex, const char* str, string *result);

int main()
{
	string regex, result;
	read_file(&regex, "regex.txt");
	string temp;
	int sbegin = 0;
	int scount = 0, k = 0;
	for (int i = 0; i < regex.size(); i++)
	{
		string a = "text";
		string out = "output";
		string tempstr;
		char integer[4];

		if (regex[i] == '\n' || i == regex.size() - 1)
		{
			++k;

			_itoa(k, integer, 10);
			a = a + integer + ".txt";
			out = out + integer + ".txt";

			i == regex.size() - 1 ? scount = i - sbegin + 1 : scount = i - sbegin;
			temp = regex.substr(sbegin, scount);
			sbegin = i + 1;


			read_file(&tempstr, a.c_str());

			result = "";
			search(temp.c_str(), tempstr.c_str(), &result);
            output_file(out, result);
		}

	}
	return 0;
}

void read_file(string *read, const char* a)
{

	FILE *fp;
	char *sre;
	int ilength;

	fp = fopen(a, "r");

	fseek(fp, 0, SEEK_END);
	ilength = ftell(fp);
	sre = (char *)malloc(ilength + 1);
	memset(sre, 0, ilength);

	fseek(fp, 0, SEEK_SET);
	fread(sre, ilength, 1, fp);
	fclose(fp);

	sre[ilength] = '\0';
	(*read).append(sre);
}

void search(const char* regex, const char* str, string *result)
{
	Regex re(regex);
	re.parse();
	re.match(str, result);
}

void output_file(string out, string a)
{
	FILE *fp;
	fp = fopen(out.c_str(), "w");
	fwrite(a.c_str(),sizeof(char),a.size(),fp);
	fclose(fp);
}
