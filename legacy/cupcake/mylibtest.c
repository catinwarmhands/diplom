//+---------------------------------------------------------------------------
//
//  HELLO_DLL.C - Windows DLL example - main application part
//

#include <stdio.h>
#include <inttypes.h>

typedef  int32_t s32;

void my_hello(void);
s32 my_sum(s32 a, s32 b);

int main()
{
	s32 x = 5;
	s32 y = 7;
    my_hello();
    printf("my_sum test: %d", my_sum(x, y));
    return 0;
}

