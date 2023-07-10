#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm> 
#include <cmath>
#include <boost/optional.hpp>
#include <boost/math/special_functions/gamma.hpp>
#include <iomanip>

double igamc(double K, double sumSquaredErrors) {
    double a = K / 2.0;
    double x = sumSquaredErrors / 2.0;

    double result = boost::math::gamma_q(a, x);

    return result;
}

std::vector<int> countCombinationsInBlocks(const std::string& bitCombination, int blockSize, const std::string& targetCombination) {
    std::vector<int> results;
    int numBlocks = bitCombination.length() / blockSize;

    for (int i = 0; i < numBlocks; i++) {
        std::string blockBitCombination = bitCombination.substr(i * blockSize, blockSize);
        int count = 0;
        size_t pos = 0;
        while ((pos = blockBitCombination.find(targetCombination, pos)) != std::string::npos) {
            count++;
            pos += targetCombination.length();
        }
        results.push_back(count);
    }

    return results;
}

int main() {
    std::string targetCombination;
    std::string filename;
    int numBlocks;
    std::string bitCombination;
    std::string bitCombination1;
    std::ifstream file("C:\\Users\\user\\Desktop\\bin\\psp.bin", std::ios::binary);
    if (file) {

        // Determining the file size
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Reading file contents
       // std::vector<char> buffer(fileSize);
       // file.read(buffer.data(), fileSize);

        // Reading only the first half of the file contents Половина файла
       // std::streampos halfFileSize = fileSize / 2;
        //std::vector<char> buffer(halfFileSize);
        //file.read(buffer.data(), halfFileSize);

        // Desired size of bitCombination
        std::streampos desiredSize = 1 << 16;  // Меняется на 2**12, от 2**13 до 2**20 ответ один и тот же 
        //Reading the desired number of bytes from the file
        std::vector<char> buffer(desiredSize);
        file.read(buffer.data(), desiredSize);


        // Converting the read bytes into a binary sequence
        for (char byte : buffer) {
            for (int i = 0; i < 8; i++) {
                char bit = (byte >> i) & 1;
                bitCombination += bit ? '1' : '0';
            }
        }
        file.close();

        // Output of the bit sequence length
        std::cout << "Length of the bit combination: " << bitCombination.length() << " bits" << std::endl;

        // Output of a binary sequence
        std::cout << "Bit combination read from the file: " << bitCombination << std::endl;
        std::cout << std::endl;
    }
    else {
        std::cout << "Failed to open the file." << std::endl;
    }
    std::cout << std::endl;

    // Byte order inversion for the little-endian format
    std::reverse(bitCombination.begin(), bitCombination.end());

    // Inversion of the order of bits in each byte
    for (size_t i = 0; i < bitCombination.length(); i += 8) {
        std::string byteBits = bitCombination.substr(i, 8);
        std::reverse(byteBits.begin(), byteBits.end());
        bitCombination1 += byteBits;
    }

    //std::cout << "Bit combination read from the file (little-endian): " << bitCombination1 << std::endl;
    std::cout << std::endl;

    // bitCombination1 output in right-to-left format
    std::string reversedBitCombination1(bitCombination1.rbegin(), bitCombination1.rend());
    if (reversedBitCombination1.length() % 64 != 0) {
        size_t remainder = reversedBitCombination1.length() % 64;
        reversedBitCombination1 = reversedBitCombination1.substr(0, reversedBitCombination1.length() - remainder);
        std::cout << "Trimmed bit combination (revers) to make it divisible by 64. New length: " << reversedBitCombination1.length() << " bits" << std::endl;
        std::cout << std::endl;
    }
    std::cout << "Bit combination read from the file (reversed): " << reversedBitCombination1 << std::endl;
    std::cout << std::endl;

    // Output of the first 9 bits from reversedBitCombination1
    std::string firstNineBits = reversedBitCombination1.substr(0, 9);
    std::cout << "First 9 bits of reversedBitCombination1: " << firstNineBits << std::endl;
    std::cout << std::endl;

    std::cout << "Enter the number of blocks: ";
    std::cin >> numBlocks;

    int blockSize = reversedBitCombination1.length() / numBlocks;
    std::cout << "Block size: " << blockSize << std::endl;

    std::cout << "Enter the target combination: ";
    std::cin >> targetCombination;

    std::cout << "Number of Blocks: " << numBlocks << std::endl;
    std::vector<int> results = countCombinationsInBlocks(reversedBitCombination1, blockSize, targetCombination);

    double K;
    std::cout << "Enter the number of degrees of freedom: ";
    std::cin >> K;

    std::cout << "Results:" << std::endl;
    std::map<int, int> variableCountMap;
    int maxCount = 0;
    for (int i = 0; i < results.size(); i++) {
        std::cout << "Block " << i + 1 << ": " << results[i] << " combinations of '" << targetCombination << "'" << std::endl;

        // Update the maximum count
        maxCount = std::max(maxCount, results[i]);

        // Increase the count for the corresponding variable "Vn"
        int count = results[i];
        variableCountMap[count]++;

        // Print the bit combination for the current block
        std::string blockBitCombination = reversedBitCombination1.substr(i * blockSize, blockSize);
        std::cout << "Bit combination for Block " << i + 1 << ": " << blockBitCombination << std::endl;
        std::cout << "==============================================" << std::endl;
    }

    if (variableCountMap.find(0) == variableCountMap.end()) {
        variableCountMap[0] = 0;
    }
    std::cout << std::endl;

    std::cout << "Variables: ";
    for (int i = 0; i <= maxCount; i++) {
        std::cout << "V" << i << "=" << variableCountMap[i] << " ";
    }
    std::cout << std::endl;

    if (maxCount > K) {
        int vK = 0;
        for (int i = K; i <= maxCount; i++) {
            vK += variableCountMap[i];
        }
        variableCountMap[K] = vK;
        std::cout << "V" << K << "=" << variableCountMap[K] << " (updated)" << std::endl;
    }

    std::cout << std::endl;

    double lambda = (blockSize - targetCombination.length() + 1) / std::pow(2, targetCombination.length());
    std::cout << "lambda = " << lambda << std::endl;

    double nu = lambda / 2.0;
    std::cout << "nu =  " << nu << std::endl;
    // Calculate Vn - N * Pn and (Vn - N * Pn)^2 / (N * Pn)
    double sumSquaredErrors = 0.0;
    for (int i = 0; i <= 5; i++) {
        double Vn = variableCountMap[i];
        double N = numBlocks;
        double Pn;

        if (i == 0) {
            Pn = exp(-nu); //
        }
        else if (i == 1) {
            Pn = nu / 2.0 * std::exp(-nu); // 
        }
        else if (i == 2) {
            Pn = ((nu * std::exp(-nu)) / 8.0) * (nu + 2);//
        }
        else if (i == 3) {
            Pn = ((nu * std::exp(-nu)) / 8.0) * (((nu * nu) / 6) + nu + 1); // 
        }
        else if (i == 4) {
            Pn = ((nu * std::exp(-nu)) / 16.0) * (((nu * nu * nu) / 24) + ((nu * nu) / 2) + ((3 * nu) / 2) + 1);
        }
        else if (i == 5) {
            Pn = 1 - (exp(-nu)) - (nu / 2.0 * std::exp(-nu)) - (((nu * std::exp(-nu)) / 8.0) * (nu + 2)) - (((nu * std::exp(-nu)) / 8.0) * (((nu * nu) / 6) + nu + 1)) - (((nu * std::exp(-nu)) / 16.0) * (((nu * nu * nu) / 24) + ((nu * nu) / 2) + ((3 * nu) / 2) + 1));
        }
        std::cout << std::endl;
        std::cout << "P" << i << " = " << Pn << std::endl;
        std::cout << "V" << i << " = " << Vn << std::endl;
        std::cout << "N" << i << " = " << numBlocks << std::endl;
        // Calculate the difference Vn - N * Pn
        double diff = Vn - numBlocks * Pn;
        std::cout << "V" << i << " - N * P" << i << " =  " << diff << std::endl;

        // Calculate (Vn - N * Pn)^2 / (N * Pn)
        double error = std::pow(diff, 2) / (numBlocks * Pn);
        std::cout << std::setprecision(30) << std::fixed;
        std::cout << "(V" << i << " - N * P" << i << ")^2 / (N * P" << i << ") =  " << error << std::endl;

        // Add to the sum of squared errors
        sumSquaredErrors += error;
    }
    std::cout << std::endl;
    // Output the sum of error squares
    std::cout << std::setprecision(30) << std::fixed;
    std::cout << "OBS =  " << sumSquaredErrors << std::endl;
    std::cout << std::endl;
    /*double K;
    std::cout << "Enter the K: ";
    std::cin >> K;*/

    double P = igamc(K, sumSquaredErrors);
    std::cout << std::setprecision(30) << std::fixed;
    std::cout << "P-value = " << P << std::endl;
    if (P < 0.01) {
        std::cout << "The sequence is non-random." << std::endl;
    }
    else {
        std::cout << "The sequence is random." << std::endl;
    }
    return 0;
}