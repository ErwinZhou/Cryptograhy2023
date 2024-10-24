// OJ Lab4 - Prime Test
#include <iostream>
#include <random>
using namespace std;

unsigned long long mulmod(unsigned long long a, unsigned long long b, unsigned long long mod)
{
    /*
     * Calculate (a * b)(mod mod) in log(b)
     */
    unsigned long long res = 0;
    a = a % mod; // 一上来先给他一个下马威
    while (b > 0)
    {
        if (b & 1)
            res = (res + a) % mod;
        if (a > mod / 2)
        {
            a = (a * 2) % mod;
        }
        else
        {
            a = a * 2;
        }
        b >>= 1;
    }
    return res % mod;
}

unsigned long long fast_power(unsigned long long base, unsigned long long exponent, unsigned long long mod)
{
    /*
     * Calculate base^exponent(mod mod) in O(log exponent)
     */
    unsigned long long result = 1;
    while (exponent > 0)
    {
        if (exponent & 1)
        {
            // 这里用result * base也可能溢出，把这个乘法也封装了
            result = mulmod(result, base, mod);
        }
        base = mulmod(base, base, mod);
        exponent >>= 1;
    }
    return result;
}

// Miller Rabin
void MillerRabin(unsigned long long n)
{
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<unsigned long long> dis(1, n - 1); // 生成0到n-1范围内的随机数

    unsigned long long a = dis(gen);
    unsigned long long k = 0;
    unsigned long long temp = n - 1;

    while (fast_power(temp, (unsigned long long)1, (unsigned long long)2) == 0)
    {
        k++;
        temp /= 2;
    }
    unsigned long long m = temp;
    unsigned long long b = fast_power(a, m, n);
    if (fast_power(b, (unsigned long long)1, n) % n == 1)
    {
        cout << "Yes";
        return;
    }
    for (unsigned long long i = 0; i < k; i++)
    {
        if (fast_power(b, (unsigned long long)1, n) == n - 1)
        {
            cout << "Yes";
            return;
        }
        else
            b = fast_power(b, (unsigned long long)2, n);
    }
    cout << "No";
    return;
}

int main()
{
    unsigned long long n;
    cin >> n;
    // Miller Rabin
    MillerRabin(n);
    return 0;
}