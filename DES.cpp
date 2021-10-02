#include <bits/stdc++.h>

using namespace std;
#define INITIO()    ios_base::sync_with_stdio(false);cin.tie(NULL);cout<<setprecision(numeric_limits<double>::max_digits10+1);
#define MEM(i,j)    memset(i,j,sizeof i);
#define X           first
#define Y           second
#define pb          push_back
#define mp          make_pair
#define ALL(a)      a.begin(),a.end()
typedef             long long ll;
typedef             pair<int ,int> pii;
const double Eps  = 1e-8;
const ll Inf      = 1e18+7;
const int Mod     = 1e9+7;

bitset<64> key;
bitset<48> subkey[16];

int ip [64] = {
    58,50,42,34,26,18,10,2, 
	60,52,44,36,28,20,12,4, 
	62,54,46,38,30,22,14,6, 
	64,56,48,40,32,24,16,8, 
	57,49,41,33,25,17,9,1, 
	59,51,43,35,27,19,11,3, 
	61,53,45,37,29,21,13,5, 
	63,55,47,39,31,23,15,7 
};

int ep [64] = {
    40, 8, 48, 16, 56, 24, 64, 32,
    39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30,
    37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28,
    35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26,
    33, 1, 41, 9, 49, 17, 57, 25
};

int E[48] = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

int s_box[8][4][16] =  {
    { 
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
    }, 
    { 
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10, 
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5, 
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15, 
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 
    }, 
    { 
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8, 
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1, 
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7, 
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 
    }, 
    { 
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15, 
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9, 
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4, 
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 
    }, 
    { 
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9, 
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, 
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14, 
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 
    }, 
    { 
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11, 
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8, 
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6, 
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 
    }, 
    { 
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1, 
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6, 
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2, 
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 
    }, 
    { 
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7, 
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2, 
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8, 
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 
    }
};

int pc1[56] = {
	57,49,41,33,25,17,9, 
	1,58,50,42,34,26,18, 
	10,2,59,51,43,35,27, 
	19,11,3,60,52,44,36,		 
	63,55,47,39,31,23,15, 
	7,62,54,46,38,30,22, 
	14,6,61,53,45,37,29, 
	21,13,5,28,20,12,4 
};

int pc2[48] = { 
	14,17,11,24,1,5, 
	3,28,15,6,21,10, 
	23,19,12,4,26,8, 
	16,7,27,20,13,2, 
	41,52,31,37,47,55, 
	30,40,51,45,33,48, 
	44,49,39,56,34,53, 
	46,42,50,36,29,32 
};

int P[32] ={
    16, 7, 20, 21,
    29, 12, 28, 17,
    1, 15, 23, 26,
    5, 18, 31, 10,
    2, 8, 24, 14,
    32, 27, 3, 9,
    19, 13, 30, 6,
    22, 11, 4, 25
};

int shift[16] ={1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

bitset<28> LS(bitset<28> in,int m){
    bitset<28> ans = in;
    if(m>2||m<1) return ans;
    for(int i=0 ; i<28-m ; i++){
        if(i-m<0) ans[i+28-m] = in[i];
        else ans[i] = in[i+m];
    }
    return ans;
}

void CreateSubkey(){
    bitset<56> reduce;
    bitset<48> sk;
    bitset<28> C,D;

    for (int i = 0; i < 56; i++) {
        reduce[i] = key[pc1[i] - 1];
    }

    for (int times = 0; times < 16; times++) {

        for (int i = 0 ; i < 28 ; i++)    C[i] = reduce[i];
        for (int j = 28 ; j < 56 ; j++)   D[j-28] = reduce[j];

        C = LS(C, shift[times]);
        D = LS(D, shift[times]);
        
        for (int q = 0; q < 56; q++) {
            if (q < 28) reduce[q] = C[q];
            else reduce[q] = D[q - 28];
        }

        for (int g = 0; g < 48; g++) sk[g] = reduce[pc2[g] - 1];

        subkey[times] = sk;
  }
}

bitset<64> StrToBit(const char s[8]){
    bitset<64> ans;
    for(int i = 0 ; i < 8 ; i++){
        for(int j = 0 ; j < 8 ; j++){
            ans[i*8+j] = (s[i]>>(7-j))&1;
        }
    }
    return ans;
}

string bitToStr(bitset<64> s) {
  string ans;
  for (int i = 0; i < 8; i++) {
    char c = '\0';
    for (int j = 0; j < 8; j++) {
      if (s[i*8 + j] == 1) {
        c = (c << 1) | 1;
      }
      else {
        c = c << 1;
      }
    }
    ans += (unsigned char)c;
  }
  return ans;
}

bitset<32> F(bitset<32> R,bitset<48> K){
    bitset<48> ex_box;
    bitset<32> output;
    for(int i = 0 ; i < 48 ; i++){
        ex_box[i] = R[E[i]-1];
    }
    for(int j = 0 ; j < 48 ; j++){
        ex_box[j] = ex_box[j]^K[j];
    }
    int idx = 0;
    for(int k =0 ;k <48 ;k+=6){
        int n = ex_box[k]+ex_box[k+5];
        int m = ex_box[k+1]*8+ex_box[k+2]*4+2*ex_box[k+3]+ex_box[k+4];
        int val = s_box[k/6][n][m];

        bitset<4> newval(val);

        for(int q=0;q<4;q++) output[idx+q] = newval[q];

        idx+=4;
    }
    bitset<32> ans;

    for(int m=0;m<32;m++) ans[m] = output[P[m]-1];

    return ans;
}

bitset<64> encrypt(bitset<64> in){
    bitset<64> map,ans;
    bitset<32> L,R,temp;
  
    for(int i =0 ; i <64 ; i++){
        map[i] = in[ip[i]-1];
        if(i<32) L[i]=map[i];
        else  R[i-32] = map[i];
    }

    for(int t=0 ; t<16 ; t++){
        temp = R;
        R = L^F(R, subkey[t]);
        L = temp;
    }

    for(int j=0 ; j<64 ; j++){
        if(j<32) ans[j] = R[j];
        else ans[j] = L[j-32];
    }

    map = ans;

    for(int q=0;q<64;q++){
        ans[q] = map[ep[q]-1];
    }
    return ans;
}

bitset<64> decrypt(bitset<64> in) {
    bitset<64> map,ans;
    bitset<32> L,R,temp;

    for(int i =0 ; i <64 ; i++){
        map[i] = in[ip[i]-1];
        if(i<32) L[i]=map[i];
        else  R[i-32] = map[i];
    }

    for(int t=15 ; t>=0 ; t--){
        temp = R;
        R = L^F(R, subkey[t]);
        L = temp;
    }

    for(int j=0 ; j<64 ; j++){
        if(j<32) ans[j] = R[j];
        else ans[j] = L[j-32];
    }

    map = ans;

    for(int q=0;q<64;q++){
        ans[q] = map[ep[q]-1];
    }
    
    return ans;
}

int main (){
    INITIO();
    string s,k;
    cin >> s;
    k = "NCKUCSIE";
    bitset<64> in,out;

    in = StrToBit(s.c_str());
    key = StrToBit(k.c_str());
    CreateSubkey();

    out = decrypt(encrypt(in));
    string ans = bitToStr(out);
    cout << ans<<endl;
    return 0;
}