#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
using namespace std;

int l, m, Nr;
int substituion[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
int permutation[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
vector<int> bitwiseXor(vector<int> vec1, vector<int> vec2)
{
    vector<int> result(vec1.size());
    transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(),
              [](int a, int b)
              { return a ^ b; });
    return result;
}
void reverseVector(vector<int> &v)
{
    reverse(v.begin(), v.end());
}

vector<int> Bin2Hex(vector<int> bin)
{
    vector<int> hex;
    while (!bin.empty())
    {
        int temp = 0;
        int i = 0;
        if (bin.size() < 4)
        {
            while (!bin.empty())
            {
                int s = bin.back();
                temp += s * pow(2, 3 - i);
                i++;
                bin.pop_back();
            }
            hex.push_back(temp);
        }
        else
        {
            while (i < 4)
            {
                int s = bin.back();
                temp += s * pow(2, 3 - i);
                i++;
                bin.pop_back();
            }
            hex.push_back(temp);
        }
    }
    return hex;
}
vector<int> Hex2Bin(vector<int> hex)
{
    vector<int> bin;
    while (!hex.empty())
    {
        int temp = hex.back();
        vector<int> temp_bin;
        while (temp)
        {
            temp_bin.push_back(temp % 2);
            temp = temp / 2;
        }
        if (temp_bin.size() < 4)
            while (temp_bin.size() < 4)
                temp_bin.push_back(0);
        hex.pop_back();
        reverseVector(temp_bin);
        bin.insert(bin.begin(), temp_bin.begin(), temp_bin.end());
    }
    return bin;
}
void SPN(vector<int> x, vector<int> *ks)
{
    vector<int> *w = new vector<int>[Nr];
    vector<int> *u = new vector<int>[Nr + 1];
    vector<int> *v = new vector<int>[Nr + 1];
    vector<int> y;
    w[0] = x;
    for (int r = 1; r <= Nr - 1; r++)
    {
        u[r] = bitwiseXor(w[r - 1], ks[r]);
        for (int i = 1; i <= m; i++)
        {
            vector<int> s_input(u[r].begin() + 4 * (i - 1), u[r].begin() + 4 * i);
            reverseVector(s_input);
            int s_input_hex = Bin2Hex(s_input).back();
            vector<int> s_output_hex(1, substituion[s_input_hex]);
            vector<int> s_output_bin = Hex2Bin(s_output_hex);
            vector<int> vr_i = s_output_bin;
            v[r].insert(v[r].end(), vr_i.begin(), vr_i.end());
        }
        for (int i = 1; i <= l * m; i++)
            w[r].push_back(v[r][permutation[i - 1] - 1]);
    }
    u[Nr] = bitwiseXor(w[Nr - 1], ks[Nr]);
    for (int i = 1; i <= m; i++)
    {
        vector<int> s_input(u[Nr].begin() + 4 * (i - 1), u[Nr].begin() + 4 * i);
        reverseVector(s_input);
        int s_input_hex = Bin2Hex(s_input).back();
        vector<int> s_output_hex(1, substituion[s_input_hex]);
        vector<int> s_output_bin = Hex2Bin(s_output_hex);
        vector<int> vNr_i = s_output_bin;
        v[Nr].insert(v[Nr].end(), vNr_i.begin(), vNr_i.end());
    }
    y = bitwiseXor(v[Nr], ks[Nr + 1]);
    for (int i = 0; i < y.size(); i++)
    {
        cout << y[i];
    }
    return;
}

int main()
{
    l = m = Nr = 4;
    char *plainstr = new char[17];
    char *keystr = new char[33];
    vector<int> plaintext;
    vector<int> key;
    vector<int> *keyscheme = new vector<int>[6];
    cin >> plainstr;
    cin >> keystr;
    for (int i = 0; i < 32; i++)
    {
        if (i < 16)
            plaintext.push_back(plainstr[i] - '0');
        key.push_back(keystr[i] - '0');
    }
    for (int i = 1; i <= 5; i++)
    {
        for (int j = 4 * i - 3; j <= 4 * i + 12; j++)
        {
            keyscheme[i].push_back(key[j - 1]);
        }
    }
    SPN(plaintext, keyscheme);
    return 0;
}

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <algorithm>
#include <set>
#include <chrono>
using namespace std;

int l, m, nr;
int t;
char unknownkey[33] = "00111010100101001101011000111111";
int substituion[16] = {14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7};
int rev_substituion[16];
int permutation[16] = {1, 5, 9, 13, 2, 6, 10, 14, 3, 7, 11, 15, 4, 8, 12, 16};
void getrevsubstitution()
{
    for (int i = 0; i < 16; i++)
    {
        int index;
        for (int j = 0; j < 16; j++)
            if (substituion[j] == i)
                index = j;
        rev_substituion[i] = index;
    }
    cout << "sbox reversed accomplished!" << endl;
    return;
}
vector<int> bitwisexor(vector<int> vec1, vector<int> vec2)
{
    vector<int> result(vec1.size());
    transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(),
              [](int a, int b)
              { return a ^ b; });
    return result;
}
void reversevector(vector<int> &v)
{
    reverse(v.begin(), v.end());
}

vector<int> bin2hex(vector<int> bin)
{
    vector<int> hex;
    while (!bin.empty())
    {
        int temp = 0;
        int i = 0;
        if (bin.size() < 4)
        {
            while (!bin.empty())
            {
                int s = bin.back();
                temp += s * pow(2, 3 - i);
                i++;
                bin.pop_back();
            }
            hex.push_back(temp);
        }
        else
        {
            while (i < 4)
            { // 若小于4，特殊处理
                int s = bin.back();
                temp += s * pow(2, 3 - i);
                i++;
                bin.pop_back();
            }
            hex.push_back(temp);
        }
    }
    return hex;
}
vector<int> hex2bin(vector<int> hex)
{
    vector<int> bin;
    while (!hex.empty())
    {
        int temp = hex.back();
        vector<int> temp_bin;
        while (temp)
        {
            temp_bin.push_back(temp % 2);
            temp = temp / 2;
        }
        if (temp_bin.size() < 4) // 若长度小于4，进行补0
            while (temp_bin.size() < 4)
                temp_bin.push_back(0);
        hex.pop_back();
        reversevector(temp_bin);
        bin.insert(bin.begin(), temp_bin.begin(), temp_bin.end());
    }
    return bin;
}
void spn(vector<int> x, vector<int> *ks, vector<int> &y)
{
    vector<int> *w = new vector<int>[nr];
    vector<int> *u = new vector<int>[nr + 1];
    vector<int> *v = new vector<int>[nr + 1];
    w[0] = x;
    for (int r = 1; r <= nr - 1; r++)
    {
        u[r] = bitwisexor(w[r - 1], ks[r]);
        for (int i = 1; i <= m; i++)
        {
            vector<int> s_input(u[r].begin() + 4 * (i - 1), u[r].begin() + 4 * i); // 获取sbox的输入
            reversevector(s_input);                                                // 转16进制前必须先翻转
            int s_input_hex = bin2hex(s_input).back();                             // 转为16进制。并且由于默认l=m=nr=4，因此转换而来的16进制只有1位
            vector<int> s_output_hex(1, substituion[s_input_hex]);                 //
            vector<int> s_output_bin = hex2bin(s_output_hex);
            vector<int> vr_i = s_output_bin;                   // 得到vr
            v[r].insert(v[r].end(), vr_i.begin(), vr_i.end()); // 插入vr
        }
        for (int i = 1; i <= l * m; i++)
            w[r].push_back(v[r][permutation[i - 1] - 1]); // 置换
    }
    u[nr] = bitwisexor(w[nr - 1], ks[nr]); // 密钥混合
    for (int i = 1; i <= m; i++)
    { // 最后一轮
        vector<int> s_input(u[nr].begin() + 4 * (i - 1), u[nr].begin() + 4 * i);
        reversevector(s_input);
        int s_input_hex = bin2hex(s_input).back();
        vector<int> s_output_hex(1, substituion[s_input_hex]);
        vector<int> s_output_bin = hex2bin(s_output_hex);
        vector<int> vnr_i = s_output_bin;
        v[nr].insert(v[nr].end(), vnr_i.begin(), vnr_i.end());
    }
    // 这里不进行置换，密钥混合后直接输出
    y = bitwisexor(v[nr], ks[nr + 1]);
    return;
}

void generatepairs(vector<vector<int>> &plaintext, vector<vector<int>> &ciphertext, vector<int> *keyscheme)
{
    random_device rd;                     // 用于获取种子
    mt19937 gen(rd());                    // 使用mersenne twister随机数生成器
    uniform_int_distribution<> dis(0, 1); // 二进制，所以只生成0和1

    set<vector<int>> generatedplaintexts; // 用于跟踪已生成的明文

    for (int i = 0; i < t; i++)
    {
        vector<int> plain(16); // 每个明文都有16位

        do
        {
            for (int j = 0; j < 16; j++)
            {
                plain[j] = dis(gen); // 随机生成0或1
            }
        } while (generatedplaintexts.find(plain) != generatedplaintexts.end()); // 保证明文是唯一的

        generatedplaintexts.insert(plain);
        plaintext.push_back(plain);

        // 使用spn函数得到对应的密文
        vector<int> cipher;
        spn(plain, keyscheme, cipher);
        ciphertext.push_back(cipher);
    }

    cout << "generation accomplished!" << endl;
    return;
}

void linearcryptanalysis(vector<vector<int>> x, vector<vector<int>> y, vector<int> k5, int *rev_substition)
{
    int candidate_key_pairs_count24[16][16];   // z1，先计算k2和k4
    int candidate_key_pairs_count13_1[16][16]; // z2
    int candidate_key_pairs_count13_2[16][16]; // z3
    int candidate_key_pairs_count13[16][16];   // 再计算k2和k3

    vector<int> *maxkey = new vector<int>[4]; // 存储最终结果
    for (int l1 = 0; l1 < 16; l1++)
        for (int l2 = 0; l2 < 16; l2++)
            candidate_key_pairs_count24[l1][l2] =
                candidate_key_pairs_count13_1[l1][l2] =
                    candidate_key_pairs_count13_2[l1][l2] =
                        candidate_key_pairs_count13[l1][l2] = 0;
    cout << "preparations finished!" << endl;
    cout << "-----------k2 & k4 analysis-----------" << endl;
    cout << "begin analyzing k2 and k4:" << endl;
    int t = 0;
    while (t < t)
    {
        vector<int> xt = x[t];
        vector<int> yt = y[t];
        vector<int> yt_1(yt.begin(), yt.begin() + 4);
        vector<int> yt_2(yt.begin() + 4, yt.begin() + 8);
        vector<int> yt_3(yt.begin() + 8, yt.begin() + 12);
        vector<int> yt_4(yt.begin() + 12, yt.begin() + 16);
        for (int l1 = 0; l1 < 16; l1++)
        {
            for (int l2 = 0; l2 < 16; l2++)
            {
                vector<int> l1_v_hex(1, l1);
                vector<int> l1_v_bin = hex2bin(l1_v_hex);
                vector<int> l2_v_hex(1, l2);
                vector<int> l2_v_bin = hex2bin(l2_v_hex);
                vector<int> v4_2 = bitwisexor(l1_v_bin, yt_2);
                vector<int> v4_4 = bitwisexor(l2_v_bin, yt_4);
                // 对v4_2进行逆代换
                reversevector(v4_2);
                int rev_s_input_hex1 = bin2hex(v4_2).back();
                vector<int> s_output_hex1(1, rev_substituion[rev_s_input_hex1]);
                vector<int> s_output_bin1 = hex2bin(s_output_hex1);
                vector<int> u4_2 = s_output_bin1;
                // 对v4_4进行逆代换
                reversevector(v4_4);
                int rev_s_input_hex2 = bin2hex(v4_4).back();
                vector<int> s_output_hex2(1, rev_substituion[rev_s_input_hex2]);
                vector<int> s_output_bin2 = hex2bin(s_output_hex2);
                vector<int> u4_4 = s_output_bin2;
                // 找出等于0的位置
                int z1 = xt[4] ^ xt[6] ^ xt[7] ^ u4_2[1] ^ u4_2[3] ^ u4_4[1] ^ u4_4[3];
                if (z1 == 0)
                    candidate_key_pairs_count24[l1][l2] += 1;
            }
        }
        t++;
        cout << t << "/" << t << " rounds finished" << endl;
        ;
    }
    // 找出最大值的位置，遍历
    int max = -1;
    for (int l1 = 0; l1 < 16; l1++)
    {
        for (int l2 = 0; l2 < 16; l2++)
        {
            candidate_key_pairs_count24[l1][l2] = abs(candidate_key_pairs_count24[l1][l2] - t / 2);
            if (candidate_key_pairs_count24[l1][l2] > max)
            {
                max = candidate_key_pairs_count24[l1][l2];
                vector<int> l1_v_hex(1, l1);
                vector<int> l2_v_hex(1, l2);
                vector<int> k2 = hex2bin(l1_v_hex);
                vector<int> k4 = hex2bin(l2_v_hex);
                maxkey[1] = k2;
                maxkey[3] = k4;
            }
        }
    }
    cout << "k2 and k4 analysis finished!" << endl;

    // 计算k1和k3：
    cout << "-----------k1 & k3 analysis-----------" << endl;
    cout << "begin analyzing k1 and k3:" << endl;
    t = 0;
    while (t < t)
    {
        vector<int> xt = x[t];
        vector<int> yt = y[t];
        vector<int> yt_1(yt.begin(), yt.begin() + 4);
        vector<int> yt_2(yt.begin() + 4, yt.begin() + 8);
        vector<int> yt_3(yt.begin() + 8, yt.begin() + 12);
        vector<int> yt_4(yt.begin() + 12, yt.begin() + 16);

        // v4_2和v4_4可以提前使用之前已经破解的k2和k4异或提前算出，避免重复计算

        // 异或计算v4_2
        vector<int> v4_2 = bitwisexor(maxkey[1], yt_2);
        // 对v4_2进行逆代换
        reversevector(v4_2);
        int rev_s_input_hex2 = bin2hex(v4_2).back();
        vector<int> s_output_hex2(1, rev_substituion[rev_s_input_hex2]);
        vector<int> s_output_bin2 = hex2bin(s_output_hex2);
        vector<int> u4_2 = s_output_bin2;

        // 异或计算v4_4
        vector<int> v4_4 = bitwisexor(maxkey[3], yt_4);
        // 对v4_4进行逆代换
        reversevector(v4_4);
        int rev_s_input_hex4 = bin2hex(v4_4).back();
        vector<int> s_output_hex4(1, rev_substituion[rev_s_input_hex4]);
        vector<int> s_output_bin4 = hex2bin(s_output_hex4);
        vector<int> u4_4 = s_output_bin4;

        for (int l1 = 0; l1 < 16; l1++)
        {
            for (int l2 = 0; l2 < 16; l2++)
            {
                vector<int> l1_v_hex(1, l1);
                vector<int> l1_v_bin = hex2bin(l1_v_hex);
                vector<int> l2_v_hex(1, l2);
                vector<int> l2_v_bin = hex2bin(l2_v_hex);

                vector<int> v4_1 = bitwisexor(l1_v_bin, yt_1);
                vector<int> v4_3 = bitwisexor(l2_v_bin, yt_3);

                // 对v4_1进行逆代换
                reversevector(v4_1);
                int rev_s_input_hex1 = bin2hex(v4_1).back();
                vector<int> s_output_hex1(1, rev_substituion[rev_s_input_hex1]);
                vector<int> s_output_bin1 = hex2bin(s_output_hex1);
                vector<int> u4_1 = s_output_bin1;

                // 对v4_3进行逆代换
                reversevector(v4_3);
                int rev_s_input_hex3 = bin2hex(v4_3).back();
                vector<int> s_output_hex3(1, rev_substituion[rev_s_input_hex3]);
                vector<int> s_output_bin3 = hex2bin(s_output_hex3);
                vector<int> u4_3 = s_output_bin3;

                // 找出等于0的位置
                int z2 = xt[0] ^ xt[1] ^ xt[3] ^ u4_1[0] ^ u4_2[0] ^ u4_3[0] ^ u4_4[0];
                if (z2 == 0)
                    candidate_key_pairs_count13_1[l1][l2] += 1;

                int z3 = xt[8] ^ xt[9] ^ xt[11] ^ u4_1[2] ^ u4_2[2] ^ u4_3[2] ^ u4_4[2];
                if (z3 == 0)
                    candidate_key_pairs_count13_2[l1][l2] += 1;
            }
        }
        t++;
        cout << t << "/" << t << " rounds finished" << endl;
        ;
    }
    // 找出最大值的位置，遍历
    max = -1;
    for (int l1 = 0; l1 < 16; l1++)
    {
        for (int l2 = 0; l2 < 16; l2++)
        {
            candidate_key_pairs_count13_1[l1][l2] = abs(candidate_key_pairs_count13_1[l1][l2] - t / 2);
            candidate_key_pairs_count13_2[l1][l2] = abs(candidate_key_pairs_count13_2[l1][l2] - t / 2);
            candidate_key_pairs_count13[l1][l2] = candidate_key_pairs_count13_1[l1][l2] + candidate_key_pairs_count13_2[l1][l2];
            if (candidate_key_pairs_count13[l1][l2] > max)
            {
                max = candidate_key_pairs_count13[l1][l2];
                vector<int> l1_v_hex(1, l1);
                vector<int> l2_v_hex(1, l2);
                vector<int> k1 = hex2bin(l1_v_hex);
                vector<int> k3 = hex2bin(l2_v_hex);
                maxkey[0] = k1;
                maxkey[2] = k3;
            }
        }
    }
    cout << "k1 and k3 analysis finished!" << endl;
    cout << "-----------results-----------" << endl;
    // 输出结果：
    cout << "k5_1:";
    for (int i = 0; i < 4; i++)
    {
        cout << maxkey[0][i];
    }
    cout << endl;
    cout << "k5_2:";
    for (int i = 0; i < 4; i++)
    {
        cout << maxkey[1][i];
    }
    cout << endl;
    cout << "k5_3:";
    for (int i = 0; i < 4; i++)
    {
        cout << maxkey[2][i];
    }
    cout << endl;
    cout << "k5_4:";
    for (int i = 0; i < 4; i++)
    {
        cout << maxkey[3][i];
    }
    cout << endl;
    // 将结果与实际的密钥对比
    vector<int> expected_key;
    for (int i = 0; i < 4; i++)
        expected_key.insert(expected_key.end(), maxkey[i].begin(), maxkey[i].end());

    if (equal(expected_key.begin(), expected_key.end(), k5.begin()))
        cout << "success!" << endl;
    else
        cout << "failure!" << endl;
    cout << "linearcryptanalysis accomplished!" << endl;
    return;
}

int main()
{
    l = m = nr = 4;
    cout << "based on a spn network(l=m=nr=4)," << endl;
    // 开始输入参数t，即明文密文对数
    cout << "first put in t for pairs of plaintext-cipher text:" << endl;
    cout << "t=";
    cin >> t;
    vector<vector<int>> plaintext;
    vector<vector<int>> ciphertext;
    vector<int> key;
    vector<int> *keyscheme = new vector<int>[6];
    for (int i = 0; i < 32; i++) // 生成密钥
        key.push_back(unknownkey[i] - '0');

    for (int i = 1; i <= 5; i++)
    { // 生成对应密钥编排方案
        for (int j = 4 * i - 3; j <= 4 * i + 12; j++)
        {
            keyscheme[i].push_back(key[j - 1]);
        }
    }
    cout << "-----------start timing-----------" << endl;
    // 获取开始时间点
    auto start = chrono::high_resolution_clock::now();
    cout << "timing started!" << endl;
    // 开始生成明文密文对
    cout << "-----------generate pairs-----------" << endl;
    generatepairs(plaintext, ciphertext, keyscheme);
    // 进行sbox的翻转
    cout << "-----------reverse sbox-----------" << endl;
    getrevsubstitution();
    cout << "reverse sbox:" << endl;
    for (int i = 0; i < 16; i++)
    {
        cout << rev_substituion[i] << " ";
    }
    cout << endl;
    // 正式开始线性攻击
    cout << "-----------linearcryptanalysis-----------" << endl;
    linearcryptanalysis(plaintext, ciphertext, keyscheme[5], rev_substituion);
    // 获取结束时间点
    auto stop = chrono::high_resolution_clock::now();

    // 计算花费的时间
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    int hours = duration.count() / 3600;
    int minutes = (duration.count() % 3600) / 60;
    int seconds = duration.count() % 60;

    cout << "time taken: " << hours << " hours " << minutes << " minutes " << seconds << " seconds." << endl;
    return 0;
}
