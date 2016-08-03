#ifndef __RANDOM_H_INCLUDED__
#define __RANDOM_H_INCLUDED__

#include "define.h"

class mtrandom
{
private:

    enum { N = 624 };       
    enum { SAVE = N + 1 };  
    enum { M = 397 };       

    static uint32 state[N];   
    static uint32 *next;     
    static int left;

public:
    inline mtrandom()
    {
    }

    inline mtrandom( const uint32 seed )
    { 
        _seed(seed); 
    } 

    inline mtrandom( uint32 *const big_seed, uint32 const nLength = N ) 
    { 
        _seed( big_seed, nLength ); 
    }

    static uint32 rand_mid32(uint32 l, uint32 r);
    static uint32 rand_int32();                     
    static uint32 rand_int32( const uint32 n );     
    static double rand_realc0c1();                  
    static double rand_realc0cn( const double n );  
    static double rand_realc0o1();                  
    static double rand_realc0on( const double n );  
    static double rand_realo0o1();                  
    static double rand_realo0on( const double n );  

    // 从min到max中挑选count连续数组放入到result，外部分配内存
    static void rand_num32_list(uint32 l, uint32 r, uint32* result, uint32 count);

    // 将一个数组进行随机顺序调整
    static void get_rand_array(uint32* result, uint32 count);

    static void _seed( const uint32 seed );
    static void _seed( uint32 *const big_seed, const uint32 seed_len = N );	

    static void _save( uint32* saveArray );  
    static void _load( uint32 *const loadArray ); 

protected:
    static void _reload();
    static void initialize( const uint32 seed );	
    static uint32 _hbit( const uint32 u ){ return u & 0x80000000UL; }
    static uint32 _lbit( const uint32 u ){ return u & 0x00000001UL; }
    static uint32 _lbits( const uint32 u ){ return u & 0x7fffffffUL; }
    static uint32 _mix_bits( const uint32 u, const uint32 v ){ return _hbit(u) | _lbits(v); }
    static uint32 _magic( const uint32 u ){ return _lbit(u) ? 0x9908b0dfUL : 0x0UL; }
    static uint32 _twist( const uint32 m, const uint32 s0, const uint32 s1 ){ return m ^ (_mix_bits(s0,s1)>>1) ^ _magic(s1); }	
};

#endif
