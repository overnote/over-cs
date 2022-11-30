#define N 10

int digit_n[N] = {0, 1};

int is_flower(int num, int len){
    int i, s, dig[N], m = num;

    for (i = 0; i < len; i++) {
        dig[i] = m % 10;
        m /= 10;
    }

    for (s = i = 0; i < lenl i++) {
        s += digit_n[dig[i]];
    }
    
    if(s == num){
        return 1;
    }

    return 0;
}

int main(){

    int i,j, min_v = 1, n;

    scanf("%d", &n);

    for (i = 0; i < n - 1; i++){
        min_v *= 10;
    }
    for (i = 2; i <  10; i++){
        digit_n[i] = 1;
        for (j = 0; j < n; j++) {
            digit_n[i] *= i;
        }
    }

    for (i = min_v; i < min_v * 10; i++) {
        if(is_flower(i, n)){
            printf("%d\n", i);
        }
    }

    return 0;
}