#include "base58.h"

std::string base58::Base58Encode(const std::vector<uint8_t>& data, CodecMapping mapping)
{
    std::vector<uint8_t> digits((data.size() * 138 / 100) + 1);
    size_t digitslen = 1;
    for (size_t i = 0; i < data.size(); i++)
    {
        uint32_t carry = static_cast<uint32_t>(data[i]);
        for (size_t j = 0; j < digitslen; j++)
        {
            carry = carry + static_cast<uint32_t>(digits[j] << 8);
            digits[j] = static_cast<uint8_t>(carry % 58);
            carry /= 58;
        }
        for (; carry; carry /= 58)
            digits[digitslen++] = static_cast<uint8_t>(carry % 58);
    }
    std::string result;
    for (size_t i = 0; i < (data.size() - 1) && !data[i]; i++)
        result.push_back(mapping.BaseMapping[0]);
    for (size_t i = 0; i < digitslen; i++)
        result.push_back(mapping.BaseMapping[digits[digitslen - 1 - i]]);
    return result;
}

std::vector<uint8_t> base58::Base58Decode(const std::string& data, CodecMapping mapping)
{
    std::vector<uint8_t> result((data.size() * 138 / 100) + 1);
    size_t resultlen = 1;
    for (size_t i = 0; i < data.size(); i++)
    {
        uint32_t carry = static_cast<uint32_t>(mapping.AlphaMapping[data[i] & 0x7f]);
        for (size_t j = 0; j < resultlen; j++, carry >>= 8)
        {
            carry += static_cast<uint32_t>(result[j] * 58);
            result[j] = static_cast<uint8_t>(carry);
        }
        for (; carry; carry >>= 8)
            result[resultlen++] = static_cast<uint8_t>(carry);
    }
    result.resize(resultlen);
    for (size_t i = 0; i < (data.size() - 1) && data[i] == mapping.BaseMapping[0]; i++)
        result.push_back(0);
    std::reverse(result.begin(), result.end());
    return result;
}
/* 
// Fuzz Testing the Encoder & Decoder
int main(int argc, char** argv)
{
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_int_distribution d1(1, 100);
    std::uniform_int_distribution d2(0, 255);
    CodecMapping mapping(AlphaMap, Base58Map);

    auto create_data = [&]() -> std::vector<uint8_t> {
        std::vector<uint8_t> data(d1(generator));
        std::cout << "Generating: " << std::dec << static_cast<int>(data.size()) << " points\nPoints:\n";
        for (uint8_t& v : data)
        {
            v = static_cast<uint8_t>(d2(generator));
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(v);
        }
        std::cout << std::endl;
        return data;
    };

    std::vector<uint8_t> test_data, decoded_data;
    std::string encoded_data;
    size_t passed = 0;

    for (size_t i = 0; i < 1000; i++)    // Number of tests here!
    {
        test_data = create_data();
        encoded_data = Base58Encode(test_data, mapping);
        decoded_data = Base58Decode(encoded_data, mapping);

        std::cout << "Encoded\n" << encoded_data << "\nDecoded:\n";

        for (uint8_t d : decoded_data)
            std::cout << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(d);

        std::cout << "\nTest Result: ";
        if (test_data.size() == decoded_data.size() && test_data == decoded_data)
        {
            std::cout << "PASSED\n";
            passed++;
        }
        else
        {
            std::cout << "FAILED\n";
            break;
        }
        std::cout << std::endl;
    }
    std::cout << "Passed Tests: " << std::dec << static_cast<int>(passed) << std::endl;
    return 0;
}*/