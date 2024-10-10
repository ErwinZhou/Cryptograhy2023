/*
#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
using namespace std;

int l, m, Nr;
int substituion[16] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
int permutation[16] = { 1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16 };
vector<int> bitwiseXor(vector<int> vec1, vector<int> vec2) {
    vector<int> result(vec1.size());
    transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(),
        [](int a, int b) { return a ^ b; });
    return result;
}
void reverseVector(vector<int>& v) {
    reverse(v.begin(), v.end());
}

vector<int> Bin2Hex(vector<int> bin) {
    vector<int> hex;
    while (!bin.empty()) {
        int temp = 0;
        int i = 0;
        if (bin.size() < 4) {
            while (!bin.empty()) {
                int s = bin.back();
                temp += s * pow(2, 3 - i);
                i++;
                bin.pop_back();
            }
            hex.push_back(temp);
        }
        else {
            while (i < 4) {
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
vector<int> Hex2Bin(vector<int> hex) {
    vector<int> bin;
    while (!hex.empty()){
        int temp = hex.back();
        vector<int> temp_bin;
        while (temp){
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
void SPN(vector<int> x,vector<int>* ks) {
    vector<int>* w = new vector<int>[Nr];
    vector<int>* u = new vector<int>[Nr+1];
    vector<int>* v = new vector<int>[Nr+1];
    vector<int> y;
    w[0] = x;
    for (int r = 1; r <= Nr - 1; r++) {
        u[r] = bitwiseXor(w[r - 1], ks[r]);
        for (int i = 1; i <= m; i++) {
            vector<int> s_input(u[r].begin() + 4 * (i - 1), u[r].begin() + 4 * i);
            reverseVector(s_input);
            int s_input_hex = Bin2Hex(s_input).back();
            vector<int> s_output_hex(1,substituion[s_input_hex]);
            vector<int> s_output_bin = Hex2Bin(s_output_hex);
            vector<int> vr_i = s_output_bin;
            v[r].insert(v[r].end(), vr_i.begin(), vr_i.end());
        }
        for (int i = 1; i <= l * m; i++) 
            w[r].push_back(v[r][permutation[i-1]-1]);
    }
    u[Nr] = bitwiseXor(w[Nr - 1], ks[Nr]);
    for (int i = 1; i <= m; i++) {
        vector<int> s_input(u[Nr].begin() + 4 * (i - 1), u[Nr].begin() + 4 * i);
        reverseVector(s_input);
        int s_input_hex = Bin2Hex(s_input).back();
        vector<int> s_output_hex(1, substituion[s_input_hex]);
        vector<int> s_output_bin = Hex2Bin(s_output_hex);
        vector<int> vNr_i = s_output_bin;
        v[Nr].insert(v[Nr].end(), vNr_i.begin(), vNr_i.end());
    }
    y = bitwiseXor(v[Nr], ks[Nr + 1]);
    for (int i = 0; i < y.size(); i++) {
        cout << y[i];
    }
    return;
}




int main() {
    l = m = Nr = 4;
    char* plainstr = new char[17];
    char* keystr = new char[33];
    vector<int> plaintext;
    vector<int> key;
    vector<int>* keyscheme = new vector<int>[6];
    cin >> plainstr;
    cin >> keystr;
    for (int i = 0; i < 32; i++) {
        if (i < 16)
            plaintext.push_back(plainstr[i] - '0');
        key.push_back(keystr[i] - '0');
    }
    for (int i = 1; i <= 5; i++) {
        for (int j = 4 * i - 3; j <= 4 * i + 12; j++) {
            keyscheme[i].push_back(key[j - 1]);
        }
    }
    SPN(plaintext, keyscheme);
    return 0;

}
*/



#include<iostream>
#include<vector>
#include<cmath>
#include<random>
#include<algorithm>
#include<set>
#include <chrono>
using namespace std;

int l, m, Nr;
int T;
char unknownkey[33] = "00111010100101001101011000111111";
int substituion[16] = { 14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7 };
int rev_substituion[16];
int permutation[16] = { 1,5,9,13,2,6,10,14,3,7,11,15,4,8,12,16 };
void getRevsubstitution() {
    for (int i = 0; i < 16; i++) {
        int index;
        for (int j = 0; j < 16; j++) 
            if (substituion[j] == i)
                index = j;  
        rev_substituion[i] = index;
    }
    cout << "SBox reversed accomplished!" << endl;
    return;
}
vector<int> bitwiseXor(vector<int> vec1, vector<int> vec2) {
    vector<int> result(vec1.size());
    transform(vec1.begin(), vec1.end(), vec2.begin(), result.begin(),
        [](int a, int b) { return a ^ b; });
    return result;
}
void reverseVector(vector<int>& v) {
    reverse(v.begin(), v.end());
}

vector<int> Bin2Hex(vector<int> bin) {
    vector<int> hex;
    while (!bin.empty()) {
        int temp = 0;
        int i = 0;
        if (bin.size() < 4) {
            while (!bin.empty()) {
                int s = bin.back();
                temp += s * pow(2, 3 - i);
                i++;
                bin.pop_back();
            }
            hex.push_back(temp);
        }
        else {
            while (i < 4) {//若小于4，特殊处理
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
vector<int> Hex2Bin(vector<int> hex) {
    vector<int> bin;
    while (!hex.empty()) {
        int temp = hex.back();
        vector<int> temp_bin;
        while (temp) {
            temp_bin.push_back(temp % 2);
            temp = temp / 2;
        }
        if (temp_bin.size() < 4)//若长度小于4，进行补0
            while (temp_bin.size() < 4)
                temp_bin.push_back(0);
        hex.pop_back();
        reverseVector(temp_bin);
        bin.insert(bin.begin(), temp_bin.begin(), temp_bin.end());
    }
    return bin;
}
void SPN(vector<int> x, vector<int>* ks, vector<int>& y) {
    vector<int>* w = new vector<int>[Nr];
    vector<int>* u = new vector<int>[Nr + 1];
    vector<int>* v = new vector<int>[Nr + 1];
    w[0] = x;
    for (int r = 1; r <= Nr - 1; r++) {
        u[r] = bitwiseXor(w[r - 1], ks[r]);
        for (int i = 1; i <= m; i++) {
            vector<int> s_input(u[r].begin() + 4 * (i - 1), u[r].begin() + 4 * i);//获取SBox的输入
            reverseVector(s_input);//转16进制前必须先翻转
            int s_input_hex = Bin2Hex(s_input).back();//转为16进制。并且由于默认l=m=Nr=4，因此转换而来的16进制只有1位
            vector<int> s_output_hex(1, substituion[s_input_hex]);//
            vector<int> s_output_bin = Hex2Bin(s_output_hex);
            vector<int> vr_i = s_output_bin;//得到vr
            v[r].insert(v[r].end(), vr_i.begin(), vr_i.end());//插入vr
        }
        for (int i = 1; i <= l * m; i++)
            w[r].push_back(v[r][permutation[i - 1] - 1]);//置换
    }
    u[Nr] = bitwiseXor(w[Nr - 1], ks[Nr]);//密钥混合
    for (int i = 1; i <= m; i++) {//最后一轮
        vector<int> s_input(u[Nr].begin() + 4 * (i - 1), u[Nr].begin() + 4 * i);
        reverseVector(s_input);
        int s_input_hex = Bin2Hex(s_input).back();
        vector<int> s_output_hex(1, substituion[s_input_hex]);
        vector<int> s_output_bin = Hex2Bin(s_output_hex);
        vector<int> vNr_i = s_output_bin;
        v[Nr].insert(v[Nr].end(), vNr_i.begin(), vNr_i.end());
    }
    //这里不进行置换，密钥混合后直接输出
    y = bitwiseXor(v[Nr], ks[Nr + 1]);
    return;
}

void generatePairs(vector<vector<int>>& plaintext, vector<vector<int>>& ciphertext, vector<int>* keyscheme) {
    random_device rd;  //用于获取种子
    mt19937 gen(rd()); //使用Mersenne Twister随机数生成器
    uniform_int_distribution<> dis(0, 1); //二进制，所以只生成0和1

    set<vector<int>> generatedPlaintexts; // 用于跟踪已生成的明文

    for (int i = 0; i < T; i++) {
        vector<int> plain(16); //每个明文都有16位

        do {
            for (int j = 0; j < 16; j++) {
                plain[j] = dis(gen); //随机生成0或1
            }
        } while (generatedPlaintexts.find(plain) != generatedPlaintexts.end()); // 保证明文是唯一的

        generatedPlaintexts.insert(plain);
        plaintext.push_back(plain);

        // 使用SPN函数得到对应的密文
        vector<int> cipher;
        SPN(plain, keyscheme, cipher);
        ciphertext.push_back(cipher);
    }

    cout << "Generation accomplished!" << endl;
    return;
}

void LinearCryptanalysis(vector<vector<int>> x, vector<vector<int>> y, vector<int> K5, int* rev_substition) {
    int candidate_key_pairs_count24[16][16];//z1，先计算K2和K4
    int candidate_key_pairs_count13_1[16][16];//z2
    int candidate_key_pairs_count13_2[16][16];//z3
    int candidate_key_pairs_count13[16][16];//再计算K2和K3

    vector<int>* maxkey = new vector<int>[4];//存储最终结果
    for (int L1 = 0; L1 < 16; L1++)
        for (int L2 = 0; L2 < 16; L2++)
            candidate_key_pairs_count24[L1][L2] =
            candidate_key_pairs_count13_1[L1][L2] =
            candidate_key_pairs_count13_2[L1][L2] = 
            candidate_key_pairs_count13[L1][L2] = 0;
    cout << "Preparations finished!" << endl;
    cout << "-----------K2 & K4 Analysis-----------" << endl;
    cout << "Begin analyzing K2 and K4:" << endl;
    int t = 0;
    while (t < T) {
        vector<int> xt = x[t];
        vector<int> yt = y[t];
        vector<int> yt_1(yt.begin(), yt.begin() + 4);
        vector<int> yt_2(yt.begin() + 4, yt.begin() + 8);
        vector<int> yt_3(yt.begin() + 8, yt.begin() + 12);
        vector<int> yt_4(yt.begin() + 12, yt.begin() + 16);
        for (int L1 = 0; L1 < 16; L1++) {
            for (int L2 = 0; L2 < 16; L2++) {
                vector<int> L1_v_hex(1, L1);
                vector<int> L1_v_bin = Hex2Bin(L1_v_hex);
                vector<int> L2_v_hex(1, L2);
                vector<int> L2_v_bin = Hex2Bin(L2_v_hex);
                vector<int> v4_2 = bitwiseXor(L1_v_bin, yt_2);
                vector<int> v4_4 = bitwiseXor(L2_v_bin, yt_4);
                //对v4_2进行逆代换
                reverseVector(v4_2);
                int rev_s_input_hex1 = Bin2Hex(v4_2).back();
                vector<int> s_output_hex1(1, rev_substituion[rev_s_input_hex1]);
                vector<int> s_output_bin1 = Hex2Bin(s_output_hex1);
                vector<int> u4_2 = s_output_bin1;
                //对v4_4进行逆代换
                reverseVector(v4_4);
                int rev_s_input_hex2 = Bin2Hex(v4_4).back();
                vector<int> s_output_hex2(1, rev_substituion[rev_s_input_hex2]);
                vector<int> s_output_bin2 = Hex2Bin(s_output_hex2);
                vector<int> u4_4 = s_output_bin2;
                //找出等于0的位置
                int z1 = xt[4] ^ xt[6] ^ xt[7] ^ u4_2[1] ^ u4_2[3] ^ u4_4[1] ^ u4_4[3];
                if (z1 == 0) 
                    candidate_key_pairs_count24[L1][L2] += 1;
            }
        }
        t++;
        cout << t << "/" << T << " rounds finished" << endl;;
    }
    //找出最大值的位置，遍历
    int max = -1;
    for (int L1 = 0; L1 < 16; L1++) {
        for (int L2 = 0; L2 < 16; L2++) {
            candidate_key_pairs_count24[L1][L2] = abs(candidate_key_pairs_count24[L1][L2] - T / 2);
            if (candidate_key_pairs_count24[L1][L2] > max) {
                max = candidate_key_pairs_count24[L1][L2];
                vector<int> L1_v_hex(1, L1);
                vector<int> L2_v_hex(1, L2);
                vector<int> K2 = Hex2Bin(L1_v_hex);
                vector<int> K4 = Hex2Bin(L2_v_hex);
                maxkey[1] = K2;
                maxkey[3] = K4;
            }
        }
    }
    cout << "K2 and K4 analysis finished!" << endl;

    //计算K1和K3：
    cout << "-----------K1 & K3 Analysis-----------" << endl;
    cout << "Begin analyzing K1 and K3:" << endl;
    t = 0;
    while (t < T) {
        vector<int> xt = x[t];
        vector<int> yt = y[t];
        vector<int> yt_1(yt.begin(), yt.begin() + 4);
        vector<int> yt_2(yt.begin() + 4, yt.begin() + 8);
        vector<int> yt_3(yt.begin() + 8, yt.begin() + 12);
        vector<int> yt_4(yt.begin() + 12, yt.begin() + 16);

        //v4_2和v4_4可以提前使用之前已经破解的K2和K4异或提前算出，避免重复计算

        //异或计算v4_2
        vector<int> v4_2 = bitwiseXor(maxkey[1], yt_2);
        //对v4_2进行逆代换
        reverseVector(v4_2);
        int rev_s_input_hex2 = Bin2Hex(v4_2).back();
        vector<int> s_output_hex2(1, rev_substituion[rev_s_input_hex2]);
        vector<int> s_output_bin2 = Hex2Bin(s_output_hex2);
        vector<int> u4_2 = s_output_bin2;

        //异或计算v4_4
        vector<int> v4_4 = bitwiseXor(maxkey[3], yt_4);
        //对v4_4进行逆代换
        reverseVector(v4_4);
        int rev_s_input_hex4 = Bin2Hex(v4_4).back();
        vector<int> s_output_hex4(1, rev_substituion[rev_s_input_hex4]);
        vector<int> s_output_bin4 = Hex2Bin(s_output_hex4);
        vector<int> u4_4 = s_output_bin4;

        for (int L1 = 0; L1 < 16; L1++) {
            for (int L2 = 0; L2 < 16; L2++) {
                vector<int> L1_v_hex(1, L1);
                vector<int> L1_v_bin = Hex2Bin(L1_v_hex);
                vector<int> L2_v_hex(1, L2);
                vector<int> L2_v_bin = Hex2Bin(L2_v_hex);

                vector<int> v4_1 = bitwiseXor(L1_v_bin, yt_1);               
                vector<int> v4_3 = bitwiseXor(L2_v_bin, yt_3);
                
                //对v4_1进行逆代换
                reverseVector(v4_1);
                int rev_s_input_hex1 = Bin2Hex(v4_1).back();
                vector<int> s_output_hex1(1, rev_substituion[rev_s_input_hex1]);
                vector<int> s_output_bin1 = Hex2Bin(s_output_hex1);
                vector<int> u4_1 = s_output_bin1;
                
                //对v4_3进行逆代换
                reverseVector(v4_3);
                int rev_s_input_hex3 = Bin2Hex(v4_3).back();
                vector<int> s_output_hex3(1, rev_substituion[rev_s_input_hex3]);
                vector<int> s_output_bin3 = Hex2Bin(s_output_hex3);
                vector<int> u4_3 = s_output_bin3;
                
                //找出等于0的位置
                int z2 = xt[0] ^ xt[1] ^ xt[3] ^ u4_1[0] ^ u4_2[0] ^ u4_3[0] ^ u4_4[0];
                if (z2 == 0) 
                    candidate_key_pairs_count13_1[L1][L2] += 1;

                int z3 = xt[8] ^ xt[9] ^ xt[11] ^ u4_1[2] ^ u4_2[2] ^ u4_3[2] ^ u4_4[2];
                if (z3 == 0)
                    candidate_key_pairs_count13_2[L1][L2] += 1;
            }
        }
        t++;
        cout << t << "/" << T << " rounds finished" << endl;;
    }
    //找出最大值的位置，遍历
    max = -1;
    for (int L1 = 0; L1 < 16; L1++) {
        for (int L2 = 0; L2 < 16; L2++) {
            candidate_key_pairs_count13_1[L1][L2] = abs(candidate_key_pairs_count13_1[L1][L2] - T / 2);
            candidate_key_pairs_count13_2[L1][L2] = abs(candidate_key_pairs_count13_2[L1][L2] - T / 2);
            candidate_key_pairs_count13[L1][L2] = candidate_key_pairs_count13_1[L1][L2] + candidate_key_pairs_count13_2[L1][L2];
            if (candidate_key_pairs_count13[L1][L2] > max) {
                max = candidate_key_pairs_count13[L1][L2];
                vector<int> L1_v_hex(1, L1);
                vector<int> L2_v_hex(1, L2);
                vector<int> K1 = Hex2Bin(L1_v_hex);
                vector<int> K3 = Hex2Bin(L2_v_hex);
                maxkey[0] = K1;
                maxkey[2] = K3;
            }
        }
    }
    cout << "K1 and K3 analysis finished!" << endl;
    cout << "-----------Results-----------" << endl;
    //输出结果：
    cout << "K5_1:";
    for (int i = 0; i < 4; i++) {
        cout << maxkey[0][i];
    }
    cout << endl;
    cout << "K5_2:";
    for (int i = 0; i < 4; i++) {
        cout << maxkey[1][i];
    }
    cout << endl;
    cout << "K5_3:";
    for (int i = 0; i < 4; i++) {
        cout << maxkey[2][i];
    }
    cout << endl;
    cout << "K5_4:";
    for (int i = 0; i < 4; i++) {
        cout << maxkey[3][i];
    }
    cout << endl;
    //将结果与实际的密钥对比
    vector<int> expected_key;
    for (int i = 0; i < 4; i++) 
        expected_key.insert(expected_key.end(), maxkey[i].begin(), maxkey[i].end());
    
    if (equal(expected_key.begin(), expected_key.end(), K5.begin()))
        cout << "Success!" << endl;
    else
        cout << "Failure!" << endl;
    cout << "LinearCryptanalysis accomplished!" << endl;
    return;
}


int main() {
    l = m = Nr = 4;
    cout << "Based on a SPN Network(l=m=Nr=4)," << endl;
    //开始输入参数T，即明文密文对数
    cout << "First put in T for pairs of plaintext-cipher text:" << endl;
    cout << "T=";
    cin >> T;
    vector<vector<int>> plaintext;
    vector<vector<int>> ciphertext;
    vector<int> key;
    vector<int>* keyscheme = new vector<int>[6];
    for (int i = 0; i < 32; i++) // 生成密钥
        key.push_back(unknownkey[i] - '0');
    
    for (int i = 1; i <= 5; i++) { //生成对应密钥编排方案
        for (int j = 4 * i - 3; j <= 4 * i + 12; j++) {
            keyscheme[i].push_back(key[j - 1]);
        }
    }
    cout << "-----------Start Timing-----------" << endl;
    // 获取开始时间点
    auto start = chrono::high_resolution_clock::now();
    cout << "Timing started!" << endl;
    //开始生成明文密文对
    cout << "-----------Generate Pairs-----------" << endl;
    generatePairs(plaintext, ciphertext, keyscheme);
    //进行SBox的翻转
    cout << "-----------Reverse SBox-----------" << endl;
    getRevsubstitution();
    cout << "Reverse SBox:" << endl;
    for (int i = 0; i < 16; i++) {
        cout << rev_substituion[i] << " ";
    }
    cout << endl;
    //正式开始线性攻击
    cout << "-----------LinearCryptanalysis-----------" << endl;
    LinearCryptanalysis(plaintext, ciphertext, keyscheme[5], rev_substituion);
    // 获取结束时间点
    auto stop = chrono::high_resolution_clock::now();

    // 计算花费的时间
    auto duration = chrono::duration_cast<chrono::seconds>(stop - start);
    int hours = duration.count() / 3600;
    int minutes = (duration.count() % 3600) / 60;
    int seconds = duration.count() % 60;

    cout << "Time taken: " << hours << " hours " << minutes << " minutes " << seconds << " seconds." << endl;
    return 0;

}
