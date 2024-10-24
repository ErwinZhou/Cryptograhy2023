#include <iostream>
#include <cmath>
using namespace std;
long long p, a, b; // 全局变量曲线的参数p、a、b
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
int main()
{

    cin >> a >> b >> p;
    long long x, y, k;
    cin >> x >> y >> k;
    Point p(x, y);
    Point k_p = DoubleAndAdd_ScalarProductOfPoint(p, k);
    cout << k_p.x << " " << k_p.y;
    return 0;
}