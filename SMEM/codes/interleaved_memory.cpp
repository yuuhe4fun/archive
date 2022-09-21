#include <iostream>
#include <vector>
#include <algorithm>

int bitsSelfXor (int x);
int main() {
    // flag
    int num_000 = 0;
    int num_001 = 0;
    int num_010 = 0;
    int num_011 = 0;
    int num_100 = 0;
    int num_101 = 0;
    int num_110 = 0;
    int num_111 = 0;

    // mask reg
    int mask0 = 0b1010011101001;
    int mask1 = 0b1110100111010;
    int mask2 = 0b1101001110100;

    int memoryAddr = 0b1111111111111 - 0b0000000000000 + 1;
    // offset address
    int offAddr;
    int bankAddr;
    int fullAddr;

    // vector to store these address
    std::vector<int> address;
    
    for (int i = 0b0000000000000; i <= 0b1111111111111; ++i)
    {
        int addr0 = bitsSelfXor(i bitand mask0);
        int addr1 = bitsSelfXor(i bitand mask1);
        int addr2 = bitsSelfXor(i bitand mask2);
        offAddr = i >> 3;
        bankAddr = addr2 * 4 + addr1 * 2 + addr0;
        fullAddr = offAddr * 8 + bankAddr;

        // bank test
        switch (bankAddr) {
            case 0:
                num_000 += 1;
                break;
            case 1:
                num_001 += 1;
                break;
            case 2:
                num_010 += 1;
                break;
            case 3:
                num_011 += 1;
                break;
            case 4:
                num_100 += 1;
                break;
            case 5:
                num_101 += 1;
                break;
            case 6:
                num_110 += 1;
                break;
            case 7:
                num_111 += 1;
                break;
        }

        // save address
        address.push_back(fullAddr);
    }

    // deduplication
    sort(address.begin(), address.end());
    address.erase(std::unique(address.begin(), address.end()), address.end());
    std::cout << "There are " << memoryAddr << " addresses." << std::endl;
    std::cout << "After hash..." << std::endl;
    std::cout << "There are " << address.size() << " addresses." << std::endl;

    // output bank distribution
    std::cout << "Bank 000: " << num_000 << " times." << std::endl;
    std::cout << "Bank 001: " << num_001 << " times." << std::endl;
    std::cout << "Bank 010: " << num_010 << " times." << std::endl;
    std::cout << "Bank 011: " << num_011 << " times." << std::endl;
    std::cout << "Bank 100: " << num_100 << " times." << std::endl;
    std::cout << "Bank 101: " << num_101 << " times." << std::endl;
    std::cout << "Bank 110: " << num_110 << " times." << std::endl;
    std::cout << "Bank 111: " << num_111 << " times." << std::endl;

    return 0;
}

int bitsSelfXor (int x)
{
    int xorResult = x % 2;
    for (int i = 1; i < 13; ++i)
    {
        x = x / 2;
        xorResult = xorResult ^ (x % 2);
    }
    return xorResult;
}