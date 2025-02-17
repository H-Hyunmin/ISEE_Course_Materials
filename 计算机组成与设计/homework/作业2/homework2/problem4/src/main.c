#if defined(D_NEXYS_A7)
   #include "bsp_printf.h"
   #include "bsp_mem_map.h"
   #include "bsp_version.h"
#else
   PRE_COMPILED_MSG("no platform was defined")
#endif

#include <stdio.h>
#include "psp_api.h"

extern int sumSquare(int n);
extern int square(int n);

int sumSquare_c(int n){
    int ret = 0;
    for(int i = 1; i <= n; i++){
        ret += square(i);
    }
    return ret;
}

int main(){
    int flag = 1;

    int i;

    for(i = 0; i < 10; i++){
        int n = sumSquare(i);
        // int n =0;
        if(n != sumSquare_c(i)){
            flag = 0;
            break;
        }
    }

    uartInit();

    if(flag){
        printfNexys("Pass!");
    }
    else{
        printfNexys("Bug!");
        printfNexys("the sumSquare of %d should be: %d", i, sumSquare_c(i));
        printfNexys("but your function return: %d", sumSquare(i));
    }
    
    while(1){}

    return 0;
}