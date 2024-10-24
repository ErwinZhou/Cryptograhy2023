
// Lab06-Simplified ECDSA
#include <iostream>
#include <cmath>
using namespace std;

long long x0, y0, m, x, k, r, s;
long long a = 1, b = 6, p = 11, q = 13;

class Point
{ // 定义所需的点类
public:
    long long x, y;
    Point(long long x1, long long y1) : x(x1), y(y1) {};
    Point() {};
};

long long mod(long long a, long long p)
{ // 重载取模运算mod
    if (a % p >= 0)
        return a % p;
    else
        return a % p + p;
}

long long mulmod(long long a, long long b, long long p)
{
    /*
     * Calculate (a * b)(mod p) in log(b)
     */
    long long res = 0;
    a = mod(a, p); // 一上来先给他一个下马威
    if (b < 0)
    {
        b = -b;
        a = p - a;
    }
    while (b > 0)
    {
        if (b & 1)
            res = mod(res + a, p);
        if (a > p / 2)
        {
            a = mod(a * 2, p);
        }
        else
        {
            a = a * 2;
        }
        b >>= 1;
    }
    return mod(res, p);
}

long long fast_power(long long base, long long exponent, long long mod)
{
    /*
     * Calculate base^exponent(mod mod) in O(log exponent)
     */
    long long result = 1;
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

long long ExtendedEuclideanAlgorithm(long long a, long long b, long long &x, long long &y)
{ // 扩展的欧几里得算法求逆元，用于求逆元
    if (!b)
    {
        x = 1;
        y = 0;
        return a;
    }
    long long ret = ExtendedEuclideanAlgorithm(b, a % b, x, y); // 递归结束返回时候得到的x和y是下一个状态的，由推导的公式决定此时的x和y
    long long temp = x;
    x = y;
    y = temp - a / b * (y);
    return ret;
}

bool IfIsInfinitePoint_O(Point P)
{ // 判断是否为无穷远点，设置无穷远点O的x和y均为特殊值-1
    return P.x == -1 && P.y == -1 ? true : false;
}

Point AddTwoPoints(Point P, Point Q)
{ // 对在椭圆曲线E_p(a,b)上的两点P,Q，计算P+Q
    Point RP;
    if (IfIsInfinitePoint_O(P))
    { // 若其中P为无穷远点O
        if (IfIsInfinitePoint_O(Q))
        { // 若Q也为无穷远点O
            RP.x = RP.y = -1;
            return RP;
        }
        else
        { // 若Q不为无穷远点O，则结果为Q
            RP.x = Q.x, RP.y = Q.y;
            return RP;
        }
    }
    if (IfIsInfinitePoint_O(Q))
    { // 若其中Q为无穷远点O
        if (IfIsInfinitePoint_O(Q))
        { // 若P也为无穷远点O,则结果为O
            RP.x = RP.y = -1;
            return RP;
        }
        else
        { // 若P不为无穷远点P，则结果为P
            RP.x = P.x, RP.y = P.y;
            return RP;
        }
    }
    if (P.x == Q.x && P.y != Q.y)
    { // 若两个点x坐标相同，但y坐标不同，即为不同点，二者相加结果为与椭圆曲线相交的第三个点——无穷远点O
        // 此时对返回的结果点x和y设为最大值即特殊值-1
        RP.x = RP.y = -1;
        return RP;
    }
    if (P.x == Q.x && P.y == Q.y)
    { // 若两点相同，为同一点，相加的结果为切线
        long long y0 = mulmod(2, P.y, p);
        long long x0 = mod(mulmod(3, mulmod(P.x, P.x, p), p) + a, p);
        long long a, b;
        ExtendedEuclideanAlgorithm(y0, p, a, b); // 调用扩展欧几里得算法求逆元
        long long y0ni = a;
        long long k = mulmod(x0, y0ni, p);
        long long x3 = mod(mulmod(k, k, p) - mulmod(2, P.x, p), p);
        long long y3 = mod(mulmod(k, (P.x - x3), p) - P.y, p);
        RP.x = x3, RP.y = y3;
        return RP;
    }
    else
    { // 若两点不相同
        long long y0 = mod(Q.y - P.y, p);
        long long x0 = mod(Q.x - P.x, p);
        long long a, b;
        ExtendedEuclideanAlgorithm(x0, p, a, b); // 调用扩展欧几里得算法求逆元
        long long x0ni = a;
        long long k = mulmod(y0, x0ni, p);
        long long x3 = mod(mulmod(k, k, p) - P.x - Q.x, p);
        long long y3 = mod(mulmod(k, (P.x - x3), p) - P.y, p);
        RP.x = x3, RP.y = y3;
        return RP;
    }
}
Point DoubleAndAdd_ScalarProductOfPoint(Point P, long long m)
{ // 对在椭圆曲线E_p(a,b)上的点P，使用倍加-和算法计算mP

    if (m == 0)
    {
        Point O(-1, -1);
        return O;
    }
    Point T(-1, -1);
    Point Q = P;
    while (m)
    {
        if (m & 1)
            T = AddTwoPoints(T, Q);
        Q = AddTwoPoints(Q, Q);
        m >>= 1;
    }
    return T;
}
void Simplified_ECDSA()
{
    // 椭圆曲线已知，即y^2=x^3+x+6(mod 11)
    Point A(x0, y0);
    // 点乘计算kA
    Point B = DoubleAndAdd_ScalarProductOfPoint(A, k);
    // 计算r
    r = mod(B.x, q);
    /* 接下来计算s */
    // 计算k^-1
    long long temp1, temp2;
    ExtendedEuclideanAlgorithm(k, q, temp1, temp2);
    long long k_ni = temp1;
    // 计算Hash(x）= 2^x(mod q)
    long long hash_x = fast_power(2, x, q);
    // 计算mr
    long long mr = mulmod(m, r, q);
    // 计算最终结果
    s = mod(mulmod(k_ni, mod(hash_x + mr, q), q), q);
    return;
}
int main()
{
    cin >> x0 >> y0 >> m >> x >> k;
    Simplified_ECDSA();
    cout << r << " " << s;
    return 0;
}