#include <bits/stdc++.h>
#define pb  push_back
using namespace std;

int main (){
    string p;
    string c;
    string k = "TEAM3rc4";
    string table;
    int s[256];
    int j = 0;
    int t = 0;

    cin >> p;
    c = p;

    for(int i = 0;i<256;i++)
        s[i] = i;
    for(int i = 0;i<256;i++){
        j = (j + s[i]+k[i%k.length()])%256;
        swap(s[i],s[j]);
    }
    j = 0;
    for(int i=0;i<p.length();i++){
        j = (s[i]+j)%256;
        swap(s[i],s[j]);
        t = (s[i]+s[j])%256;
        c[i] ^= s[t];
        table.pb(s[t]);
    }
    for(int i=0;i<p.length();i++) 
        p[i]=table[i]^c[i];
    
    cout <<"sbox :"<<table<<endl;
    cout <<"ciphers : "<<c<<endl;
    cout << "plaintext : "<<p<<endl;
    return 0;
}