//Astar实现八数码问题 
#include<iostream>
#include<queue>
#include<vector>
#include<map>
using namespace std; 
int computeestep(int s[][3]){  //计算该布局到目标状态预计需要多少步 
	int count=0;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			if(i==2&&j==2&&s[i][j]==0) continue; 
		    if(s[i][j]!=(i*3+j+1)) count++;//相应位置的数与目标状态的不一致则加一 
		}
	}
	return count;
}
struct state{
	int s[3][3],nowstep,estep;//方格棋盘目前布置，从初状态到现在走的步数，预计到目标状态的步数。 
	int ox,oy;
	bool operator<(const state& s2)const{ 
	    if((this->nowstep+this->estep)<(s2.nowstep+s2.estep)) return true;
	    return false;
	} 
	bool operator==(const state& s2){ //重载状态相等的判断条件 
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++)
			if(this->s[i][j]!=s2.s[i][j]) return false;
		}
		return true;
	}
	bool operator!=(const state& s2){
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++)
			if(this->s[i][j]!=s2.s[i][j]) return true;
		}
		return false;
	}
};
//定义操作算子数组 
int operatorx[]={0,0,-1,1};
int operatory[]={-1,1,0,0};
vector<state> closed,visited;  //closed表 
map<state,int> depth;
map<state,state> father;
vector<state> ans;
bool ok(state s,int x,int y){  //检查能够做某操作 
	if(s.ox+x>2||s.ox+x<0||s.oy+y<0||s.oy+y>2) return false;
	return true;
}
struct cmp{
	bool operator()(const state &s1,const state &s2){
	     return (s1.nowstep+s1.estep)>(s2.nowstep+s2.estep);
	}
};
struct less{
	bool operator()(const state &s1,const state &s2){
	     return (s1.nowstep+s1.estep)>(s2.nowstep+s2.estep);
	}
};
bool isclosed(state s){  //判断是否在closed表里 
	for(int i=0;i<closed.size();i++){
		if(closed[i]==s) return true;
	}
	return false;
}
bool isvisited(state s){  //判断是否在visited表里 
	for(int i=0;i<visited.size();i++){
		if(visited[i]==s) return true;
	}
	return false;
}
int findstepvisited(state now){
	for(int i=0;i<visited.size();i++){
		if(visited[i]==now) return visited[i].nowstep;
	}
}
int main(){
	cout<<"初始化你的初状态："<<endl;
	state sf;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++){
			cin>>sf.s[i][j]; 
			if(sf.s[i][j]==0){
				sf.ox=i;sf.oy=j;
			}
		} 
	}
	sf.estep=computeestep(sf.s);
	state end;
	for(int i=0;i<3;i++){
		for(int j=0;j<3;j++) end.s[i][j]=i*3+j+1;
	}
	end.s[2][2]=0;end.nowstep=0;end.estep=0;end.ox=2;end.oy=2;
	priority_queue<state,vector<state>,cmp> q;
	q.push(sf);
	visited.push_back(sf);
	int o=0;
	while(!q.empty()){
		state top=q.top();
		q.pop();
		closed.push_back(top);
		for(int i=0;i<3;i++){
					for(int j=0;j<3;j++)
					cout<<top.s[i][j]<<" ";
					cout<<endl;
				}
		if(top==end){
		//	end=top;
			break;
		} 
		cout<<"  ↓"<<endl;
		for(int i=0;i<4;i++){
			if(ok(top,operatorx[i],operatory[i])){
				/*cout<<"father:"<<endl;
				for(int i=0;i<3;i++){
					for(int j=0;j<3;j++)
					cout<<top.s[i][j]<<" ";
					cout<<endl;
				}
				cout<<top.nowstep<<" "<<top.estep<<" "<<top.ox<<" "<<top.oy<<endl;
				cout<<"爸爸的爸爸"<<endl;
				for(int i=0;i<3;i++){
	            		for(int j=0;j<3;j++)
		         		cout<<father[top].s[i][j]<<" ";
	            		cout<<endl;
                 		}
                cout<<father[top].nowstep<<" "<<father[top].estep<<" "<<father[top].ox<<" "<<father[top].oy<<endl;*/
			//	cout<<"top.depth:"<<top.nowstep<<" "<<top.estep<<" "<<endl;
				state now=top;
				now.nowstep++;
				int t=now.s[now.ox+operatorx[i]][now.oy+operatory[i]];
				now.s[now.ox+operatorx[i]][now.oy+operatory[i]]=0;
				now.s[now.ox][now.oy]=t;
				now.estep=computeestep(now.s);
				now.ox=now.ox+operatorx[i];now.oy=now.oy+operatory[i];
			/*	cout<<"now:"<<endl;
				for(int i=0;i<3;i++){
					for(int j=0;j<3;j++)
					cout<<now.s[i][j]<<" ";
					cout<<endl;
				}
				cout<<now.nowstep<<" "<<now.estep<<" "<<now.ox<<" "<<now.oy<<endl;*/
			//	cout<<"now.depth:"<<now.nowstep<<" "<<now.estep<<" "<<endl;
			//	cout<<"isvisited:"<<isvisited(now)<<" isclosed:"<<isclosed(now)<<endl;
				if(isvisited(now)){
					if(isclosed(now)) continue;
					else{
						if(depth[now]>now.nowstep){ //更新 
							depth[now]=now.nowstep;
							father[now]=top;
						}
					}
				}
				else{ //还没被访问过 
					depth[now]=now.nowstep;
					father[now]=top;
					for(int i=0;i<3;i++){
	            		for(int j=0;j<3;j++)
		         		/*cout<<father[now].s[i][j]<<" ";
	            		cout<<endl;
                 		}
                 		cout<<father[now].nowstep<<" "<<father[now].estep<<" "<<father[now].ox<<" "<<father[now].oy<<endl;*/
					q.push(now);
					visited.push_back(now);
				}
			//	cout<<"closed表里的个数："<<closed.size()<<endl;
			//	cout<<"visited表里的个数："<<visited.size()<<endl;
			}
		}
	}
/*	cout<<"存储"<<endl;
	while(end!=sf){
		ans.push_back(end);
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++)
				cout<<end.s[i][j]<<" ";
			cout<<endl;
		}
		cout<<end.nowstep<<" "<<end.estep<<" "<<end.ox<<" "<<end.oy<<endl;
		end=father[end];
	}
	ans.push_back(sf);
	cout<<"A*算法解路径："<<endl;
	for(int k=0;k<ans.size();k++){
		for(int i=0;i<3;i++){
			for(int j=0;j<3;j++){
				cout<<ans[k].s[i][j]<<" ";
			}
			cout<<endl;
		}
		if(k!=ans.size()-1) cout<<" ↓"<<endl;*/
	}
}
