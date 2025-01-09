//5:17
#include<bits/stdc++.h>
using namespace std;
const int maxn = 1e6+10;

using u32=uint32_t;using u64=uint64_t;using u128=unsigned __int128;
inline u32 ctz(u128 z){
	return(u64)z?__builtin_ctzll(z):64|__builtin_ctzll(z>>64);
}
struct iset{
	struct{int son[4];}t[maxn*12];
	int tot;
	void insert(u32 x){
		int u=0;
		for(int i=29;i>6;i-=2){
			if(!t[u].son[x>>i&3])t[u].son[x>>i&3]=++tot;
			u=t[u].son[x>>i&3];
		}
		(u128&)t[u] |= (u128)1<<(x&127);
	}
	bool count(u32 x){
		int u=0;
		for(int i=29;i>6;i-=2){
			u=t[u].son[x>>i&3];
			if(!u)return 0;
		}
		return (u128&)t[u]>>(x&127)&1;
	}
	u32 lower_bound_slw(int u,u32 x,int i){
		if(i>6){
			if(t[u].son[x>>i&3]){
				u32 res=lower_bound_slw(t[u].son[x>>i&3],x,i-2);
				if(~res)return (x&(3<<i))|res;
			}
			for(int z=(x>>i&3)+1;z<4;++z)if(t[u].son[z]){
				return z<<i|lower_bound_slw(t[u].son[z],0,i-2);
			}
			return -1;
		}
		u128 msk=(u128&)t[u];
		return(msk>>(x&127))?(x&127)+ctz(msk>>(x&127)):-1;
	}
	u32 lower_bound(u32 x){
		return lower_bound_slw(0,x,29);
	}
	void clear(){memset(t,0,(tot+1)*sizeof t[0]);tot=0;}
}is;

#define test_clock(name,a...) {cerr<<"   "<<name<<": ";auto tim=clock();uint32_t hash_val=0;a cerr<<double(clock()-tim)/CLOCKS_PER_SEC<<"s, hash_val:"<<hash_val<<"\n";}

template<class T,class F,class G>
void test(T&&t,F const&f,G const&g,int mxrnd=9){
	cerr<<__PRETTY_FUNCTION__<<":\n";
	mt19937 rng;
	test_clock("insert 1e6 rand",
		for(int i=0;i<(int)1e6;++i)t.insert(f(i,rng()%(1u<<31)));
	);
	test_clock("count 1e6 rand",
		for(int rnd=0;rnd<mxrnd;++rnd){
			for(int i=0;i<(int)1e6;++i){
				if(t.count(g(i,rng()%(1u<<31))))hash_val^=i;
			}
		}
	);
}
template<class T,class F,class G,class GET_VAL>
void test_lower_bound(T&&t,F const&f,G const&g,GET_VAL const&get_val,int mxrnd=9){
	cerr<<__PRETTY_FUNCTION__<<":\n";
	mt19937 rng;
	test_clock("insert 1e6 rand",
		for(int i=0;i<(int)1e6;++i)t.insert(f(i,rng()%(1u<<31)));
	);

	t.insert((1u<<31)-1);

	test_clock("count 1e6 rand",
		for(int rnd=0;rnd<mxrnd;++rnd){
			for(int i=0;i<(int)1e6;++i){
				hash_val^=get_val(t.lower_bound(g(i,rng()%(1u<<31))));
			}
		}
	);
}
auto tak_fir=[](int a,int){return a;};
auto tak_sec=[](int,int b){return b;};
auto tak_num=[](int a){return a;};
auto tak_itr=[](set<int>::iterator a){return*a;};
signed main() {
	ios::sync_with_stdio(0),cin.tie(0);

	is.clear();
	test_lower_bound(is,tak_sec,tak_sec,tak_num,3);
	test_lower_bound(set<int>(),tak_sec,tak_sec,tak_itr,3);

	is.clear();
	test_lower_bound(is,tak_sec,tak_fir,tak_num,39);
	test_lower_bound(set<int>(),tak_sec,tak_fir,tak_itr,39);

	is.clear();
	test(is,tak_sec,tak_sec);
	// test(set<int>(),tak_sec,tak_sec); //too slow
	test(unordered_set<int>(),tak_sec,tak_sec);

	is.clear();
	test(is,tak_sec,tak_fir);
	// test(set<int>(),tak_sec,tak_fir); //too slow
	test(unordered_set<int>(),tak_sec,tak_fir);
}
