#include<bits/stdc++.h>

using namespace std;

int match(char c1,char c2){
	return (c1=='A'&&c2=='U')||(c1=='U'&&c2=='A')||(c1=='C'&&c2=='G')||(c1=='G'&&c2=='C');
}


int main(){
	string input_seq;
	cin>>input_seq;
	input_seq.insert(0,1,'$');
	cout<<input_seq<<endl;
	int n = input_seq.size();
	vector<vector<int>> dp(n+1,vector<int>(n+1,0));
	for (int i = 0; i <= n; ++i)
	{
		for (int j=0;j<=n; ++j)
		{
			if(i>=j-4){
				dp[i][j]=0;
			}
		}
	}
	set<pair<int,int>>base_pairs;
	for(int k=5;k<=n-1;k++){
		for(int i = 1;i<=n-k;i++){
			int j = i+k;
			int mx_t = INT_MIN;
			int t_idx = i;
			for(int t=i;t<j-4;t++){
				if(match(input_seq[t],input_seq[j])){
						mx_t = max(mx_t,1+dp[i][t-1]+dp[t+1][j-1]); 
                     
				}
			}
			
			dp[i][j] = max(dp[i][j-1],mx_t);
		}

	}
	/*for(int k=4;k>=1;k--){
		for(int i = 6;i<=9;i++){
			cout<<dp[k][i]<<" ";
		}
		cout<<endl;
	}*/
	for(auto i:base_pairs){
		cout<<i.first<<"===="<<i.second<<endl;
	}
	cout<<dp[1][n]<<endl;

	return 0;
}
