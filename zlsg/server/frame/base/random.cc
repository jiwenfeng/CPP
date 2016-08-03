#include "random.h"

uint32 mtrandom::state[N] = {0};  
uint32* mtrandom::next = NULL;     
int mtrandom::left = 0;

void mtrandom::initialize( const uint32 seed )
{
    register uint32 *s = state;
    register uint32 *r = state;
    register int i = 1;
    *s++ = seed & 0xffffffffUL;
    for( ; i < N; ++i )
    {
        *s++ = ( 1812433253UL * ( *r ^ (*r >> 30) ) + i ) & 0xffffffffUL;
        r++;
    }
}

void mtrandom::_reload()
{
    static const int MmN = int(M) - int(N); 
	
    uint32 *p = state;	
    for( int i = N - M; i--; ++p ) *p = _twist( p[M], p[0], p[1] );
    for( int i = M; --i; ++p ) *p = _twist( p[MmN], p[0], p[1] );
    *p = _twist( p[MmN], p[0], state[0] );	
    left = N, next = state;
}

void mtrandom::_seed( const uint32 seed )
{
    initialize( seed );
    _reload();
}

void mtrandom::_seed( uint32 *const big_seed, const uint32 nLength )
{
    initialize( 19650218UL );
    int i = 1;
    uint32 j = 0;
    int k = ( N > nLength ? N : nLength );
    for( ; k; --k )
    {
        state[i] = state[i] ^ ( (state[i-1] ^ (state[i-1] >> 30)) * 1664525UL );
        state[i] += ( big_seed[j] & 0xffffffffUL ) + j;
        state[i] &= 0xffffffffUL;
        ++i;  ++j;
        if( i >= N ) { state[0] = state[N-1]; i = 1; }
        if( j >= nLength ) j = 0;
    }
    for( k = N - 1; k; --k )
    {
        state[i] =
		state[i] ^ ( (state[i-1] ^ (state[i-1] >> 30)) * 1566083941UL );
        state[i] -= i;
        state[i] &= 0xffffffffUL;
        ++i;
        if( i >= N ) { state[0] = state[N-1];  i = 1; }
    }
    state[0] = 0x80000000UL;  
    _reload();
}

uint32 mtrandom::rand_mid32(uint32 l, uint32 r)
{
    if (r < l ) {
        uint32 temp = r;
        r = l;
        l = temp;
    }

    return (l + mtrandom::rand_int32() % ( r - l + 1 ) );
}

void mtrandom::get_rand_array(uint32* result, uint32 count)
{
    uint32 k;
    for (uint32 i = 0; i < count; ++i){
        uint32 j = rand_int32()%count;
        if (i != j){
            k = result[i];
            result[i] = result[j];
            result[j] = k;
        }
    }
}

void mtrandom::rand_num32_list(uint32 l, uint32 r, uint32* result, uint32 count)
{
    if (l >= r || count == 0){
        return;
    }

    uint32 num = r - l + 1;
    if (num < count){
        count = num;
    } 
    
    if (num == count) {
        for (uint32 i = 0; i < count; ++i){
            result[i] = l + i;
        }
        return;
    }

    uint32* arr = new uint32[num];
    for (uint32 i = 0; i < num; ++i){
        arr[i] = l + i;
    }

    for (uint32 i = 0; i < count; ++i){
        uint32 index = rand_int32() % num;
        result[i] = arr[index];

        arr[index] = arr[num - 1 - i];
        --num;
    }

    delete []arr;
}

static int random_lock = 0;

uint32 mtrandom::rand_int32()
{
#ifdef __LIB_MINI_SERVER__
    return (uint32)rand();
#endif // __LIB_MINI_SERVER__

    while (__sync_lock_test_and_set(&random_lock,1)) {}
    if( left == 0 ){
        _reload();
    }
    --left;
    uint32 s1 = *next++;
    __sync_lock_release(&random_lock);

    s1 ^= (s1 >> 11);
    s1 ^= (s1 <<  7) & 0x9d2c5680UL;
    s1 ^= (s1 << 15) & 0xefc60000UL;
    return ( s1 ^ (s1 >> 18) );	
}

uint32 mtrandom::rand_int32( const uint32 n )
{
    uint32 used = n;
    used |= used >> 1;
    used |= used >> 2;
    used |= used >> 4;
    used |= used >> 8;
    used |= used >> 16;
	
    uint32 i;
    do
    {
        i = rand_int32() & used;  
    }
    while( i > n );
    return i;
}

double mtrandom::rand_realc0c1()
{
    return double(rand_int32()) * (1.0/4294967295.0); 
}

double mtrandom::rand_realc0cn( const double n )
{
    return rand_realc0c1() * n; 
}

double mtrandom::rand_realc0o1()
{
    return double(rand_int32()) * (1.0/4294967296.0); 
}

double mtrandom::rand_realc0on( const double n )
{
    return rand_realc0o1() * n; 
}

double mtrandom::rand_realo0o1()
{
    return ( double(rand_int32()) + 0.5 ) * (1.0/4294967296.0); 
}

double mtrandom::rand_realo0on( const double n )
{
    return rand_realo0o1() * n; 
}


void mtrandom::_save( uint32* saveArray )
{
    const uint32 *s = state;
    uint32 *sa = saveArray;	
    for( int i = N; i--; *sa++ = *s++ ) {}
    *sa = left;
}

void mtrandom::_load( uint32 *const loadArray )
{
    register uint32 *s = state;
    register uint32 *la = loadArray;	
    for( int i = N; i--; *s++ = *la++ ) {}
    left = *la;
    next = &state[N-left];
}
