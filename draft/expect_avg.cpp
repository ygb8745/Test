/*
0
1
2
..
N  : (N) // 2
N+1
*/
#include <iostream>
#include <vector>
using namespace std;

int main(){
    // vector<int> v={0,1,2,3,4,5,6};
    int n=10;
    vector<vector<int>> profit_matrix(1);

    for(int i=1; i<= n; i++){
        profit_matrix.push_back(vector<int>(n+1));
        // cout << i <<"\t"<< (i)/2 << endl;
        int t= (i-1)/2;
        // cout << t << endl;
        // cout << i << "\t" << "-1\t";
        profit_matrix[i][1] = -1;
        for(int j=2;j<=n;j++){
            if((j)/2<=t){
                // cout << -(j/2*2+1) << "\t";
                profit_matrix[i][j] = -(j/2*2+1);
            }else{
                // cout << (2*t+1) << "\t";
                profit_matrix[i][j] = 2*t+1;
            }
        }
        // cout << endl;
    }
    
    for(int i=1; i<= n; i++){
        cout << i << "\t";
        for(int j=1;j<=n;j++){
            cout << profit_matrix[i][j] << "\t";
        }
        cout << endl;
    }
}
