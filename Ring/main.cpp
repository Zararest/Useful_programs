#include <iostream>
#define MAX 100


int main(int, char**) {
    
    unsigned long long tmp = 0, p_pow = 0;
    unsigned long long arr_of_pow[MAX + 1] = {0};
    unsigned long long p = 0, m = 0, q = 1;

    std::cin >> p >> m; //вычисление обратного числа по модулю m p*q = 1 (mod m)
    p_pow = m - 2;
    tmp = p;

    arr_of_pow[0] = 1;
    for (int i = 1; i < MAX; i++){
        
        arr_of_pow[i] = tmp;
        tmp = (tmp * tmp) % m;
    }

    for (int i = 1; p_pow != 0; i++, p_pow = p_pow >> 1){
        
        if (p_pow & 1){

            q = (q * arr_of_pow[i]) % m;
        }
    }

    std::cout << q << std::endl;;

}
