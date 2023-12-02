// SHA-256.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <vector>
#include <array>
#include <fstream>
#include <string>

using std::string;
using std::vector;

void little_to_big_convert(uint32_t& word)
{
    word = (word >> 24) | ((word >> 8) & 0xff00)
        | ((word << 8) & 0xff0000) | (word << 24);
}

int RightRotate32(uint32_t n, uint32_t d)
{
    return (n >> d) | (n << (32 - d));
}

uint32_t C0(uint32_t word)
{
    return RightRotate32(word, 7) ^ RightRotate32(word, 18) ^ (word >> 3);
}

uint32_t C1(uint32_t word)
{
    return RightRotate32(word, 17) ^ RightRotate32(word, 19) ^ (word >> 10);
}

void ExpensionBlock(std::array<uint32_t, 64>& block)
{
    for (int i = 16; i < 64; i++)
    {
        block[i] = block[i - 16] + C0(block[i - 15]) + block[i - 7] + C1(block[i - 2]);
    }
}

void init_table(std::array<uint32_t, 64>& block, unsigned char bytes[64])
{
    //int mod = count % 4;

    for (int i = 0; i < 16; i++)
    {
        /*char temp1 = bytes[(i * 4) + 3];
        char temp2 = bytes[(i * 4) + 2] << 8;
        char temp3 = bytes[(i * 4) + 1] << 16;
        char temp4 = bytes[(i * 4)] << 24;*/
        block[i] = (bytes[(i * 4) + 3]) | (bytes[(i * 4) + 2] << 8) | (bytes[(i * 4) + 1] << 16) | (bytes[(i * 4)] << 24);
    }
}



uint32_t Sum0(uint32_t a)
{
    return RightRotate32(a, 2) ^ RightRotate32(a, 13) ^ RightRotate32(a, 22);
}
uint32_t Sum1(uint32_t e)
{
    return RightRotate32(e, 6) ^ RightRotate32(e, 11) ^ RightRotate32(e, 25);
}

uint32_t Choice(uint32_t e, uint32_t f, uint32_t g)
{
    return (e & f) ^ ((~e) & g);
}

uint32_t Majority(uint32_t a, uint32_t b, uint32_t c)
{
    return (a & b) ^ (a & c) ^ (b & c);
}


void compress(std::array<uint32_t, 64> w, uint32_t H [])
{
    const std::array<uint32_t, 64> K = {
        0x428a2f98,0x71374491,0xb5c0fbcf,0xe9b5dba5,
        0x3956c25b,0x59f111f1,0x923f82a4,0xab1c5ed5,
        0xd807aa98,0x12835b01,0x243185be,0x550c7dc3,
        0x72be5d74,0x80deb1fe,0x9bdc06a7,0xc19bf174,
        0xe49b69c1,0xefbe4786,0x0fc19dc6,0x240ca1cc,
        0x2de92c6f,0x4a7484aa,0x5cb0a9dc,0x76f988da,
        0x983e5152,0xa831c66d,0xb00327c8,0xbf597fc7,
        0xc6e00bf3,0xd5a79147,0x06ca6351,0x14292967,
        0x27b70a85,0x2e1b2138,0x4d2c6dfc,0x53380d13,
        0x650a7354,0x766a0abb,0x81c2c92e,0x92722c85,
        0xa2bfe8a1,0xa81a664b,0xc24b8b70,0xc76c51a3,
        0xd192e819,0xd6990624,0xf40e3585,0x106aa070,
        0x19a4c116,0x1e376c08,0x2748774c,0x34b0bcb5,
        0x391c0cb3,0x4ed8aa4a,0x5b9cca4f,0x682e6ff3,
        0x748f82ee,0x78a5636f,0x84c87814,0x8cc70208,
        0x90befffa,0xa4506ceb,0xbef9a3f7,0xc67178f2
    };

    uint32_t a = H[0];
    uint32_t b = H[1];
    uint32_t c = H[2];
    uint32_t d = H[3];
    uint32_t e = H[4];
    uint32_t f = H[5];
    uint32_t g = H[6];
    uint32_t h = H[7];

    for (size_t i = 0; i < 64; i++)
    {
        uint32_t Temp1 = h + Sum1(e) + Choice(e, f, g) + K[i] + w[i];
        uint32_t Temp2 = Sum0(a) + Majority(a, b, c);

        h = g;
        g = f;
        f = e;
        e = d + Temp1;
        d = c;
        c = b;
        b = a;
        a = Temp1 + Temp2;

        if (i == 62)
        {
            int dsafds = 4;
        }
    }

    H[0] += a;
    H[1] += b;
    H[2] += c;
    H[3] += d;
    H[4] += e;
    H[5] += f;
    H[6] += g;
    H[7] += h;
}

//Чтобы не путаться с little/big endian. Потом исправлю
void write_to_bytes_array(unsigned char bytes[], uint64_t count)
{
    bytes[63] = count;
    bytes[62] = count >> 8;
    bytes[61] = count >> 16;
    bytes[60] = count >> 24;
    bytes[59] = count >> 32;
    bytes[58] = count >> 40;
    bytes[57] = count >> 48;
    bytes[56] = count >> 56;


}


void Calculate()
{

}

int main()
{

    std::string line;

    unsigned char bytes[64];

    uint64_t file_count_bytes = 0;

    uint32_t H[8] = { 0x6a09e667, 0xbb67ae85 , 0x3c6ef372 , 0xa54ff53a , 0x510e527f , 0x9b05688c , 0x1f83d9ab , 0x5be0cd19 };

    std::ifstream in; // окрываем файл для чтения
    in.open("IMG_20230517_191956.jpg", std::ios::in | std::ios::binary);
    if (in.is_open())
    {
        /*while (std::getline(in, line))
        {
            std::cout << line << std::endl;
        }*/
        
        /*uint32_t a = H[0];
        uint32_t b = H[1];
        uint32_t c = H[2];
        uint32_t d = H[3];
        uint32_t e = H[4];
        uint32_t f = H[5];
        uint32_t g = H[6];
        uint32_t h = H[7];*/

        while (in.read((char*)bytes, 64))
        {
            int temp_count = in.gcount();
            file_count_bytes += temp_count;

            //uint32_t temp = (bytes[3]) | (bytes[2] << 8) | (bytes[1] << 16) | (bytes[0] << 24);
            //little_to_big_convert(temp);
            
            //Делал по-быстрому, поэтому пока так
            std::vector<std::array <uint32_t, 64>> blocks(1);

            init_table(blocks[0], bytes);

            ExpensionBlock(blocks[0]);

            compress(blocks[0], H);
        }
        int temp_count = in.gcount();
        file_count_bytes += temp_count; //Последние байты

        if (temp_count > 55)
        {
            int a = 5;
            //Надо добавить ещё один блок и заполнить нулями до конца

            std::vector<std::array <uint32_t, 64>> blocks(1);

            for (int i = temp_count; i < 64; i++)
            {
                bytes[i] = 0x00;
            }

            bytes[temp_count] = 0x80;
            temp_count = -1;
            //write_to_bytes_array(bytes, file_count_bytes * 8);

            init_table(blocks[0], bytes);
            
            ExpensionBlock(blocks[0]);

            compress(blocks[0], H);
        }

        //В конце добавляем последний блок данных. После данных один байт 0x80, в конце 8 байт file_count_bytes
            std::vector<std::array <uint32_t, 64>> blocks(1);

                



            for (int i = temp_count+1; i < 64-8; i++)
            {
                bytes[i] = 0x00;
            }
            if (temp_count != -1)
            {
                bytes[temp_count] = 0x80;
            }


            write_to_bytes_array(bytes, file_count_bytes * 8);
            init_table(blocks[0], bytes);
            
            //Таблица заполнена. Обрабатываем
            //
            ExpensionBlock(blocks[0]);

            compress(blocks[0], H);
    }
    in.close();     // закрываем файл
    
}




