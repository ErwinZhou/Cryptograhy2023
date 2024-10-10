//
// Created by zyc13 on 2024/5/21.
//
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <regex>

struct PingResult {
    std::string ip;
    int icmp_seq;
    int ttl;
    double time;
};

std::vector<PingResult> parsePingOutput(const std::string& pingOutput) {
    std::vector<PingResult> results;
    std::istringstream stream(pingOutput);
    std::string line;
    std::regex regexPattern(R"(\d+ bytes from ([\d.]+) \([\d.]+\): icmp_seq=(\d+) ttl=(\d+) time=([\d.]+) ms)");

    while (std::getline(stream, line)) {
        std::smatch match;
        if (std::regex_search(line, match, regexPattern)) {
            PingResult result;
            result.ip = match[1];
            result.icmp_seq = std::stoi(match[2]);
            result.ttl = std::stoi(match[3]);
            result.time = std::stod(match[4]);
            results.push_back(result);
        }
    }

    return results;
}

int main() {
    std::string pingOutput =
            "PING baidu.com (39.156.66.10) 56(84) bytes of data.\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=1 ttl=128 time=9.58 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=2 ttl=128 time=11.1 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=3 ttl=128 time=9.22 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=4 ttl=128 time=10.9 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=5 ttl=128 time=9.73 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=6 ttl=128 time=8.46 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=7 ttl=128 time=8.49 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=8 ttl=128 time=8.90 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=9 ttl=128 time=8.70 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=10 ttl=128 time=8.75 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=11 ttl=128 time=16.1 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=12 ttl=128 time=8.98 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=13 ttl=128 time=9.30 ms\n"
            "64 bytes from 39.156.66.10 (39.156.66.10): icmp_seq=14 ttl=128 time=9.82 ms\n";

    std::vector<PingResult> results = parsePingOutput(pingOutput);

    for (const auto& result : results) {
        std::cout << "IP: " << result.ip
                  << ", ICMP Seq: " << result.icmp_seq
                  << ", TTL: " << result.ttl
                  << ", Time: " << result.time << " ms" << std::endl;
    }

    return 0;
}
