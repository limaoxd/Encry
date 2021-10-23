#include <bits/stdc++.h>
#define pb  push_back
using namespace std;

int main (){
    string p,c,table;
    string k = "TEAM3rc4";
    int s[256];
    int i=0 ,j = 0,t = 0;

    getline(cin,p);
    c = p;

    for(i = 0;i<256;i++) s[i] = i;
    for(i = 0;i<256;i++){
        j = (j + s[i] + k[i%k.length()])%256;
        swap(s[i],s[j]);
    }
    for(i=0,j=0;i<p.length();i++){
        j = (s[i]+j)%256;
        swap(s[i],s[j]);
        t = (s[i]+s[j])%256;
        c[i] ^= s[t];
        table.pb(s[t]);
    }
    for(i=0;i<p.length();i++) p[i]=table[i]^c[i];

    cout <<"keystring :"<<table<<endl;
    cout <<"ciphers : "<<c<<endl;
    cout << "plaintext : "<<p<<endl;
    return 0;
}