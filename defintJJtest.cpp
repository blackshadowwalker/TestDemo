#include <iostream>

class MemoryPool  
{  
};

#define REGISTER_MEMORYPOOL(PoolNameSpace, ClassName, IncreaseNum) \
    namespace PoolNameSpace\
{\
	MemoryPool g_##ClassName##_mem_pool(sizeof(ClassName), IncreaseNum);\
}\
    void *ClassName::operator new(size_t size)\
{\
    void *mem = PoolNameSpace::g_##ClassName##_mem_pool.Alloc();\
    return mem;\
}\
    void ClassName::operator delete(void *m)\
{\
    PoolNameSpace::g_##ClassName##_mem_pool.Free(m);\
}

class Test
{

};

void main()
{
	REGISTER_MEMORYPOOL(gamememorypool, Test, 64)  ;

}