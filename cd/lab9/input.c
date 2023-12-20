#include <stdio.h>
#include <stdlib.h>
void main()
{
    int a[100];
    int i, p, r, t, amt;

    amt = p * r;

    // if(p==1)
    // {
    //     for(i=1;i<=5;i=i+1)
    //     {
    //         r=r*2;
    //     }
    // } 

    // while(t<10)
    // {
    //     if(r==t+2)
    //     {
    //         p=p*1000;
    //     }
    // }

    switch (p)
    {
    case 1:
        t = 3;
        break;

    case 2:
        i = 5;
        break;

    case 3:
        r = r + 10;
        break;

    case 4:
        amt = amt - 2;
        break;

    default:
        p = 1;
    }
}