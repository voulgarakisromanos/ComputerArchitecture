#include <stdio.h>
int main() {
    int num;
    
    num = 13;
    
    // True if num is perfectly divisible by 2
    if(num % 2 == 0)
        printf("%d is even.", num);
    else
        printf("%d is odd.", num);
    
    return 0;
}
