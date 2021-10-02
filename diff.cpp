#include <bits/stdc++.h>

using namespace std;
#define INITIO()    ios_base::sync_with_stdio(false);cin.tie(NULL);cout<<setprecision(numeric_limits<double>::max_digits10+1);
#define MEM(i,j)    memset(i,j,sizeof i);
#define X           first
#define Y           second
#define pb          push_back
#define mp          make_pair
#define ALL(a)      a.begin(),a.end()
typedef             unsigned long long ll;
typedef             pair<int ,int> pii;
const double Eps  = 1e-8;
const ll Inf      = 1e18+7;
const int Mod     = 1e9+7;

int sbox[4][16] = { 
    14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
    0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
    4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
    15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
};
int sboxRev[4][16] = {
    0,2,9,4,13,7,12,3,1,6,11,5,15,10,14,8,
    0,8,1,14,3,7,5,15,12,13,9,11,6,2,4,10,
    13,1,5,15,10,2,14,7,0,3,12,11,8,9,6,4,
    13,10,5,8,4,11,12,7,9,14,2,6,3,15,1,0
};

int chars[64][16];
vector<pair<int,int>> probdiff;

int s_row (int str){
    bitset<6> val(str);
    int row=0;
    row = val[5]*2+val[0];
    return row;
}

int s_col (int str){
    bitset<6> val(str);
    int col =0;
    col= 2*val[4]+4*val[3]+2*val[2]+val[1];
    return col;
}

int roundFunc(int input, int key)
{
    return sbox[s_row(key ^ input)][s_col(key ^ input)];    
}

int encrypt(int input, int k0, int k1)
{
    int x0 = roundFunc(input, k0);
    return x0 ^ k1;        
}

void findDiffs()
{
    printf("\nCreating XOR differential table:\n");
    
    int c, d, e, f;
    int MaxT=0;

    for(c = 0; c < 64; c++){
        for(d = 0; d < 64; d++){
            chars[c ^ d][sbox[s_row(c)][s_col(c)] ^ sbox[s_row(d)][s_col(d)]]++;   
        } 
    }     
    
    for(c = 0; c < 64; c++)
    {
        for(d = 0; d < 16; d++){
            printf("  %x ", chars[c][d]);
            if(!c && !d) {}
            else MaxT = max(MaxT,chars[c][d]);
        }
        printf("\n");
    }
    printf("\nDisplaying most probable differentials:\n");
    
    for(c = 0; c < 64; c++){
        for(d = 0; d < 16; d++){
            if (chars[c][d] == MaxT){
                cout << MaxT <<"/64:   "<<c<<" --> "<<d<<endl;
                probdiff.pb(pair<int,int>(c,d)); 
            }            
        } 
    }
}

ll knownP0[10000];
ll knownP1[10000];
ll knownC0[10000];
ll knownC1[10000];

ll goodP0, goodP1, goodC0, goodC1;

ll numPairs;

//int realk0, realk1;

ll chardat0[64];
int chardatmax = 0;

void genCharData(int indiff, int outdiff)
{
            printf("\nGenerating possible intermediate values based on differential(%i --> %i):\n", indiff, outdiff);
    
            chardatmax = 0;
            MEM(chardat0,0);

            int f;
            for(f = 0; f < 64; f++){
                int myComp = f ^ indiff;
                
                if ((sbox[s_row(f)][s_col(f)] ^ sbox[s_row(myComp)][s_col(myComp)]) == outdiff){
                    printf("  Possibles:   %i + %i --> %i + %i\n", f, myComp, sbox[s_row(f)][s_col(f)], sbox[s_row(myComp)][s_col(myComp)]);
                    chardat0[chardatmax] = f;
                    chardatmax++;
                }
            }    
}

void genPairs(int indiff)
{
    printf("\nGenerating %lld known pairs with input differential of %i.\n", numPairs, indiff);
    
    ll realk0 = fmod(rand(), 24);                                                       //Create random subkey0
    ll realk1 = fmod(rand(), 24);                                                          //Create random subkey0
    
    printf("  Real K0 = %lld\n", realk0);
    printf("  Real K1 = %lld\n", realk1);
    MEM(knownP0,0);
    MEM(knownP1,0);
    MEM(knownC0,0);
    MEM(knownC1,0);
    
    ll c;
    for(c = 0; c < numPairs; c++){
        knownP0[c] = fmod(rand() , 32);
        knownP1[c] = knownP0[c] ^ indiff;
        knownC0[c] = encrypt(knownP0[c], realk0, realk1);
        knownC1[c] = encrypt(knownP1[c], realk0, realk1);
    }  
}

void findGoodPair(int outdiff)
{
    printf("\nSearching for good pair:\n");
    ll c;
    for(c = 0; c < numPairs; c++)
        if ((knownC0[c] ^ knownC1[c]) == outdiff){   
            goodC0 = knownC0[c];
            goodC1 = knownC1[c];
            goodP0 = knownP0[c];
            goodP1 = knownP1[c];   
            printf("  FOUND GOOD PAIR: (P0 = %lld, P1 = %lld) --> (C0 = %lld, C1 = %lld)\n", goodP0, goodP1, goodC0, goodC1);        
            return;    
        }     
    printf("NO GOOD PAIR FOUND!\n");
}

int testKey(int testK0, int testK1)
{
    ll c;
    int crap = 0;
    for(c = 0; c < numPairs; c++){
        if ((encrypt(knownP0[c], testK0, testK1) != knownC0[c]) || (encrypt(knownP1[c], testK0, testK1) != knownC1[c])){
            crap = 1;
            break;
        }        
    }
    
    if (crap == 0)
        return 1;     
    else   
        return 0;   
}

void crack()
{
    printf("\nBrute forcing reduced keyspace:\n");
    
    ll f;
    for(f = 0; f < chardatmax; f++){
        int testK0 = chardat0[f] ^ goodP0;
        int testK1 = sbox[s_row(chardat0[f])][s_col(chardat0[f])] ^ goodC0;

        if (testKey(testK0, testK1) == 1)
            printf("  KEY! (%i, %i)\n", testK0, testK1);
        else
            printf("       (%i, %i)\n", testK0, testK1);
    }  
}

int main()
{
    srand(time(NULL));                                                          //Randomize values per run
    
    findDiffs();                                                                //Find some good differentials in the S-Boxes
    
    numPairs = 64;                                                               //Define number of known pairs
    for(auto &it : probdiff){
        int x=it.X,y=it.Y;
        genCharData(x, y);                                                          //Find inputs that lead a certain characteristic
        genPairs(x);                                                        //Generate chosen-plaintext pairs
        findGoodPair(y);                                                            //Choose a known pair that satisfies the characteristic
        crack();       
    }                                                             //Use charData and "good pair" in find key                                                               
    //while(1){}
    return 0;    
}