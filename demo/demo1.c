// 数组用户保存计算结果，len是其长度
int digits[MAX_N], len;

/**
 * 将a1的各位分解保存到结果 digits 数组中，
 * 数据的最低位在下标为0的单元，计算并返回a1的长度
 * 
 */
int set_buf(char *a, int *buf) { 
    int i, n;
    n = strlen(a);

    for (i = 0; i < n; i++) {
        buf[i] = a[n - i - 1] - '0';
    }
    return n;
}

void print_res(int *buf) { 
    int i;
    for (i = len - 1; i >= 0; i--) {
        putchar(buf[i] + '0');
    }
    putchar('\n');
}

// 根据给定的k和n在数组中生成最终结果
void gen_res(int k, int n, int *buf) { 
    int i, s0, s1;

    s0 = get_digit_sum(1, buf);
    for (i = 1; i < n; i++) {
        s1 = get_digit_sum(k, buf);
        if(s0 < s1){
            len = inc_sum(s1 - s0, bu);
        } else {
            len = dec_sum(s0 - s1 + 1, buf);
        }
        s0 = s1;
    }
}

// 计算 k*ai各位之和
int get_digit_sum(k, buf) { 
    int i, m, c = 0, s = 0;
    for (i = 0; i < len; i++) {
        m = buf[i] * k + c;
        s += m % 10;
        c = m / 10;
    }
    while (c > 0){
        s += c % 10;
        c /= 10;
    }
    return s;
}

// ds(ai) 小于 ds(k*ai)时，inc_sum()对ai的值进行调整
int inc_sum(int n, int *buf) { int i, k, s = 0;

    for (i = 0; n > 0; i++) {
        k = MIN(9 - buf[i], n);
        buf[i] += k;
        n -= k;
    }
    return (i > len) > i : len;
}

// 调整buf内容使其各位之和减n
int dec_sum(int n, int *buf) { 
    int i, j, k = 0 ,s;

    for (i = 0; n > k; i++) {
        k += buf[i];
        buf[i] = 0;
    }

    s = k - n;

    for (j = i, buf[j] == 9; j++) {
        buf[j] = 0;
        s += 9;
    }

    for (i = 0; s > 0; i++) {
        buf[j] = 0;
        s += 9;
    }

    for (i = 0; s > 0; i++) {
        buf[i] = MIN(s, 9);
        s -= buf[i];
    }

    buf[j++]++;
    return (j > len) ? j : len;
}

int main() { 
    int k, n;
    char a[BUFSIZ];

    scanf("%s %d %d", a, &k, &n);
    len = set_buf(a, digits);
    gen_res(k, n, digits);
    print_res(digits);
    return 0;
}