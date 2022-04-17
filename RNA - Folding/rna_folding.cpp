#include<bits/stdc++.h>
#include <fstream>


using namespace std;

vector<pair<int,int>>base_pairs;

int match(char c1,char c2){
	return (c1=='A'&&c2=='U')||(c1=='U'&&c2=='A')||(c1=='C'&&c2=='G')||(c1=='G'&&c2=='C');
}

void retrieve_structure(string input_seq,vector<vector<int>>&dp,int x,int y,int n){
	if(x > n || y < 1){
		return;
	}
	if(dp[x][y] == dp[x][y-1]){
		retrieve_structure(input_seq,dp,x,y-1,n);
	}
	else{
		for(int t = x; t<y-4;t++){

				if(1+dp[x][t-1]+dp[t+1][y-1] == dp[x][y]){
					if(match(input_seq[t],input_seq[y])){
						base_pairs.push_back({t,y});
						retrieve_structure(input_seq,dp,x,t-1,n);
						retrieve_structure(input_seq,dp,t+1,y-1,n);
						break;
					}
				}
		}
	}


}


int main(){
	string input_seq;
	cin>>input_seq;
	input_seq.insert(0,1,'$');
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
    ofstream fout; 
    fout.open("C:/Users/BITS-PC/Desktop/DAA Assignment-1 final/RNA secondary struct prediction/RNA_secondary_structures.txt",std::ios_base::app);
    fout<<"Input sequence : "<<input_seq<<endl;
	cout<<"Max base pairs possible = "<<dp[1][n]<<endl;
	fout<<"Max base pairs possible = "<<dp[1][n]<<endl;
    cout<<"Base Pairings"<<endl;
    fout<<"Base Pairings"<<endl;

	retrieve_structure(input_seq,dp,1,n,n);
	for(auto i:base_pairs){
		cout<<input_seq[i.first]<<"("<<i.first<<")"<<"===="<<input_seq[i.second]<<"("<<i.second<<")"<<endl;
		fout<<input_seq[i.first]<<"("<<i.first<<")"<<"===="<<input_seq[i.second]<<"("<<i.second<<")"<<endl;

	}
	fout<<"------------------------------------------------------------------------------------------------"<<endl;
	fout.close();

	return 0;
}
