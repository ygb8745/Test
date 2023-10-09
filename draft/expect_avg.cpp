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
    for(int i=1; i<= n; i++){
        // cout << i <<"\t"<< (i)/2 << endl;
        int t= (i-1)/2;
        // cout << t << endl;
        cout << i << "\t" << "-1\t";
        for(int j=2;j<=n;j++){
            if((j)/2<=t){
                cout << -(j/2*2+1) << "\t";
            }else{
                cout << (2*t+1) << "\t";
            }
        }
        cout << endl;
    }
}
