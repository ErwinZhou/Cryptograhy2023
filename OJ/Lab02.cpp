// Homework OJ Lab02:AES
#include <iostream>
#include <vector>
#include <math.h>
#include <algorithm>
using namespace std;

int Nr = 10;
vector<int> key;
vector<int> Expandedkey;
vector<int> Plaintext;
vector<int> *Rcon = new vector<int>[11];
vector<int> *w = new vector<int>[44];
string rcon[11] = {"00", "01", "02", "04", "08", "10", "20", "40", "80", "1B", "36"};
vector<int> MixMat[4][4] = {
    {vector<int>{0, 2}, vector<int>{0, 3}, vector<int>{0, 1}, vector<int>{0, 1}},
    {vector<int>{0, 1}, vector<int>{0, 2}, vector<int>{0, 3}, vector<int>{0, 1}},
    {vector<int>{0, 1}, vector<int>{0, 1}, vector<int>{0, 2}, vector<int>{0, 3}},
    {vector<int>{0, 3}, vector<int>{0, 1}, vector<int>{0, 1}, vector<int>{0, 2}}};

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
vector<int> GaloisMultiplication_128(vector<int> a, vector<int> b)
{
    /*
     * GF(2^8)
     * Inreduceable polynomial: x^8+x^4+x^3+x^1+1;
     */
    vector<int> result(8, 0);
    vector<int> temp(15, 0);
    vector<int> Irreduciblepolynomial = {1, 0, 0, 0, 1, 1, 0, 1, 1};
    for (int i = 0; i < 8; i++)
    {
        if (a[i])
        {
            for (int j = 0; j < 8; j++)
            {
                if (b[j])
                    temp[i + j] ^= a[i] * b[j];
            }
        }
    }
    for (int i = 0; i < 6; i++)
        Irreduciblepolynomial.insert(Irreduciblepolynomial.begin(), 0); // 填0

    int ir_first_index = 6;
    int first_index;
    while (true)
    {
        auto it = find(temp.begin(), temp.end(), 1);
        if (it != temp.end())
        {
            first_index = distance(temp.begin(), it);
            if (first_index > ir_first_index)
            {
                result.clear();
                result.insert(result.end(), temp.end() - 8, temp.end());
                break;
            }
            else
            {
                // 不可约多项式左移
                vector<int> temp_ir_ploy = Irreduciblepolynomial;
                temp_ir_ploy.erase(temp_ir_ploy.begin(), temp_ir_ploy.begin() + ir_first_index - first_index);
                temp_ir_ploy.insert(temp_ir_ploy.end(), ir_first_index - first_index, 0);
                temp = bitwisexor(temp, temp_ir_ploy);
            }
        }
        else
            break;
    }
    return result;
}

vector<int> FieldInv(vector<int> a)
{
    vector<int> hex(2, 0);
    for (int i = 0; i <= 15; i++)
    {
        hex[1] = 0;
        for (int j = 0; j <= 15; j++)
        {
            vector<int> temp = hex2bin(hex);
            vector<int> result = GaloisMultiplication_128(temp, a);
            if (result.back() == 1 && all_of(result.begin(), result.end() - 1, [](int i)
                                             { return i == 0; }))
                return temp;
            hex[1]++;
        }
        hex[0]++;
    }
}

vector<int> SubBytes(vector<int> a)
{
    /*
     * SubBytes based on binary
     * 8bit binary
     * return: b on binary 8 bit
     * Correct
     */
    if (!all_of(a.begin(), a.end(), [](int i)
                { return i == 0; }))
        a = FieldInv(a);
    vector<int> c = {0, 1, 1, 0, 0, 0, 1, 1};
    reversevector(c);
    reversevector(a);
    vector<int> b(8, 0);
    for (int i = 0; i < 8; i++)
        b[i] = (a[i % 8] + a[(i + 4) % 8] + a[(i + 5) % 8] + a[(i + 6) % 8] + a[(i + 7) % 8] + c[i]) % 2;
    reversevector(b);
    return b;
}

void AddRoundKey(vector<int> (&s)[4][4], vector<int> k[4][4])
{
    /*
     * XOR based on hex
     */
    vector<int> bin_s, bin_k, temp;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            bin_s = hex2bin(s[i][j]);
            bin_k = hex2bin(k[i][j]);
            temp = bitwisexor(bin_s, bin_k);
            reversevector(temp);
            s[i][j] = bin2hex(temp);
        }
    }
    return;
}
void ShiftRows(vector<int> (&s)[4][4])
{
    // Row 0 remains the same
    // Row 1
    vector<int> temp = s[1][0];
    s[1][0] = s[1][1];
    s[1][1] = s[1][2];
    s[1][2] = s[1][3];
    s[1][3] = temp;
    // Row 2
    temp = s[2][0];
    vector<int> temp1 = s[2][1];
    s[2][0] = s[2][2];
    s[2][1] = s[2][3];
    s[2][2] = temp;
    s[2][3] = temp1;
    // Row 3
    temp = s[3][0];
    temp1 = s[3][1];
    vector<int> temp2 = s[3][2];
    s[3][0] = s[3][3];
    s[3][1] = temp;
    s[3][2] = temp1;
    s[3][3] = temp2;
    return;
}
void MixColumns(vector<int> (&s)[4][4])
{
    /*
     * MixColumn based on Hex
     */

    vector<int> bin_s[4][4];
    vector<int> bin_mm[4][4];
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            bin_mm[i][j] = hex2bin(MixMat[i][j]);
            bin_s[i][j] = hex2bin(s[i][j]);
        }
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            s[i][j] = bitwisexor(
                bitwisexor(
                    GaloisMultiplication_128(bin_mm[i][0], bin_s[0][j]),
                    GaloisMultiplication_128(bin_mm[i][1], bin_s[1][j])),
                bitwisexor(
                    GaloisMultiplication_128(bin_mm[i][2], bin_s[2][j]),
                    GaloisMultiplication_128(bin_mm[i][3], bin_s[3][j])));
            reversevector(s[i][j]);
            s[i][j] = bin2hex(s[i][j]);
        }
    }
    return;
}

vector<int> SubWord(vector<int> *B)
{
    /*
     * SubWords based on bin
     * return:w1 32bit binary
     */
    vector<int> *S_B = new vector<int>[5];
    for (int i = 1; i <= 4; i++)
        S_B[i] = SubBytes(B[i]);

    vector<int> w1;
    for (int i = 1; i <= 4; i++)
        w1.insert(w1.end(), S_B[i].begin(), S_B[i].end());

    return w1;
}
vector<int> *RotWord(vector<int> w)
{
    /*
     * w 32bit=4B B1 B2 B3 B4
     * Based on binary
     */
    vector<int> *B = new vector<int>[5];
    for (int i = 1; i <= 4; i++)
        B[i].insert(B[i].begin(), w.begin() + 8 * i - 8, w.begin() + 8 * i);
    vector<int> temp = B[1];
    B[1] = B[2];
    B[2] = B[3];
    B[3] = B[4];
    B[4] = temp;

    return B;
}
void KeyExpansion()
{
    /*
     * return: w[44] on hex
     */
    // Initializing RCon
    for (int i = 1; i <= 10; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            if (rcon[i][j] <= 'Z' && rcon[i][j] >= 'A')
                Rcon[i].push_back(10 + rcon[i][j] - 'A');
            else
                Rcon[i].push_back(rcon[i][j] - '0');
        }
        Rcon[i].insert(Rcon[i].end(), 6, 0);
        Rcon[i] = hex2bin(Rcon[i]); // Transfer Rcon to binary,Rcon-32bit
        // Correct
    }

    for (int i = 0; i < 4; i++)
    {
        w[i].assign(key.begin() + 8 * i, key.begin() + 8 * i + 8); // Hex
        w[i] = hex2bin(w[i]);                                      // w-binary,32bit
        // Correct
    }

    for (int i = 4; i <= 43; i++)
    {
        vector<int> temp = w[i - 1]; // temp-binary,32bit
        // Correct
        if (!(i % 4))
            temp = bitwisexor(SubWord(RotWord(temp)), Rcon[i / 4]);
        w[i] = bitwisexor(w[i - 4], temp);
    }
    // bin2hex
    for (int i = 0; i < 44; i++)
    {
        reversevector(w[i]);
        w[i] = bin2hex(w[i]);
    }
    // Correct
    return;
}
void AES()
{
    KeyExpansion(); // w[44] hex,Correct
    // Correct
    vector<int> s[4][4]; // Stored in hex
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            s[i][j].insert(s[i][j].begin(), Plaintext.begin() + 8 * j + 2 * i, Plaintext.begin() + 8 * j + 2 * i + 2);

    vector<int> K[4][4];
    // AddRoundKey-1
    for (int j = 0; j < 4; j++)
        for (int i = 0; i < 4; i++)
            K[i][j].insert(K[i][j].begin(), w[j].begin() + 2 * i, w[j].begin() + 2 * i + 2);

    AddRoundKey(s, K);
    // Rounds
    for (int r = 1; r <= Nr; r++)
    {
        if (r == Nr)
        {
            // SubBytes
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    s[i][j] = hex2bin(s[i][j]); // SubBytes receive argument on bin
                    s[i][j] = SubBytes(s[i][j]);
                    reversevector(s[i][j]);
                    s[i][j] = bin2hex(s[i][j]);
                }
            }

            // ShiftRows
            ShiftRows(s);
            // AddRoundKey
            for (int j = 0; j < 4; j++)
                for (int i = 0; i < 4; i++)
                    K[i][j].assign(w[4 * r + j].begin() + 2 * i, w[4 * r + j].begin() + 2 * i + 2);
            AddRoundKey(s, K);
        }
        else
        { // First Nr-1 Round

            // SubBytes
            for (int j = 0; j < 4; j++)
            {
                for (int i = 0; i < 4; i++)
                {
                    s[i][j] = hex2bin(s[i][j]); // SubBytes receive argument on bin
                    s[i][j] = SubBytes(s[i][j]);
                    reversevector(s[i][j]);
                    s[i][j] = bin2hex(s[i][j]);
                }
            }
            // ShiftRows
            ShiftRows(s);
            // MixColumns
            MixColumns(s);
            // AddRoundKey
            for (int j = 0; j < 4; j++)
                for (int i = 0; i < 4; i++)
                    K[i][j].assign(w[4 * r + j].begin() + 2 * i, w[4 * r + j].begin() + 2 * i + 2);
            AddRoundKey(s, K);
        }
    }

    // Output ciphertext
    for (int j = 0; j < 4; j++)
    {
        for (int i = 0; i < 4; i++)
        {
            for (int k = 0; k < 2; k++)
            {
                if (s[i][j][k] >= 10 && s[i][j][k] <= 15)
                    cout << char('A' + s[i][j][k] - 10);
                else
                    cout << char('0' + s[i][j][k]);
            }
        }
    }

    return;
}
int main()
{
    char k, p;
    // Input key and Plaintext
    for (int i = 0; i < 32; i++)
    {
        cin >> k;
        if (k <= 'f' && k >= 'a')
            key.push_back(10 + k - 'a');
        else
            key.push_back(k - '0');
    }
    for (int i = 0; i < 32; i++)
    {
        cin >> p;
        if (p <= 'f' && p >= 'a')
            Plaintext.push_back(10 + p - 'a');
        else
            Plaintext.push_back(p - '0');
    }
    AES();
    return 0;
}