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



int main() {
    std::string targetCombination;
    std::string filename;
    std::string bitCombination;
    std::string bitCombination1;
    std::ifstream file("C:\\Users\\user\\Desktop\\bin\\psp.bin", std::ios::binary);
    if (file) {



        // Reading only the first half of the file contents Половина файла
        // std::streampos halfFileSize = fileSize / 2;
        // std::vector<char> buffer(halfFileSize);
        // file.read(buffer.data(), halfFileSize);

        /* // Reading file contentsВесь блок
         std::vector<char> buffer(fileSize);
         file.read(buffer.data(), fileSize);*/



         // Determining the file size Сам задаю размер 
        file.seekg(0, std::ios::end);
        std::streampos fileSize = file.tellg();
        file.seekg(0, std::ios::beg);

        // Desired size of bitCombination
        std::streampos desiredSize = 1 << 20;  // Меняется на 2**12, от 2**13 до 2**20 ответ один и тот же 
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
    }
    else {
        std::cout << "Failed to open the file." << std::endl;
    }
    // Преобразование каждого символа в reversedBitCombination1
    std::string transformedBitCombination;
    std::vector<int> transformedValues;
    int sum1 = 0; // Переменная для накопления суммы символов
    for (char bit : bitCombination) {
        int value = (bit == '0') ? -1 : 1;
        transformedBitCombination += std::to_string(value) + " ";
        transformedValues.push_back(value); // Добавляем значение в вектор transformedValues
        sum1 += value; // Накопление суммы символов
    }
    // Вычисление последовательностей Sn
    std::vector<int> sequenceS;
    std::string sequenceString; // Строка для формирования новой последовательности
    int sum = 0;
    for (int value : transformedValues) {
        sum += value;
        sequenceS.push_back(sum);
        sequenceString += std::to_string(sum) + " ";
    }
    // Добавляем ноль в начало и конец строки sequenceString
    sequenceString = "0 " + sequenceString + "0";
    // Сохранение строки в файл
    std::ofstream outputFile("C:\\Users\\user\\Desktop\\bin\\1.txt");
    if (outputFile.is_open()) {
        outputFile << "New sequence string: " << sequenceString << std::endl;
        outputFile.close();
        std::cout << "File saved successfully." << std::endl;
    }
    else {
        std::cout << "Unable to open the file." << std::endl;
    }
    // Вывод новой строки последовательности Sn
    std::cout << "New sequence string: " << sequenceString << std::endl;

    // Подсчет количества нулей по элементам в строке sequenceString
    double zeroCount = 0;
    std::istringstream issCount(sequenceString);
    int valueCount;
    while (issCount >> valueCount) {
        if (valueCount == 0) {
            zeroCount++;
        }
    }
    double zeroCountAdjusted = zeroCount - 1; // Исправленное значение zeroCount

    zeroCount = zeroCount - 1;
    // Вывод количества нулей
    std::cout << "Number of zeros in sequenceString: " << zeroCount << std::endl;

    // Разделение sequenceString на блоки
    std::vector<std::string> blocks;
    std::istringstream iss(sequenceString);
    std::string block;
    std::string value;
    bool inBlock = false;
    int blockNumber = 1;

    std::map<int, int> repetitions; // Словарь для подсчета повторений чисел

    while (iss >> value) {
        if (value == "0") {
            if (inBlock) {
                blocks.push_back(block);
                block.clear();
                inBlock = false;
                blockNumber++;
            }
        }
        else {
            block += value + " ";
            inBlock = true;

            int num = std::stoi(value);
            if (blockNumber <= 6) {
                repetitions[num]++;
            }
        }
    }

    // Вывод блоков
    int blockCount = blocks.size();
    std::cout << "Number of blocks: " << blockCount << std::endl;
    for (int i = 0; i < blockCount; i++) {
        std::cout << "Block " << i + 1 << ": " << blocks[i] << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Repetitions in each block:" << std::endl;
    double sumRatio = 0.0;

    // Векторы для хранения значений V для каждого блока
    std::vector<int> v1_values;
    std::vector<int> v2_values;
    std::vector<int> v3_values;
    std::vector<int> v4_values;
    std::vector<int> v_neg4_values;
    std::vector<int> v_neg3_values;
    std::vector<int> v_neg2_values;
    std::vector<int> v_neg1_values;

    for (int i = 0; i < blocks.size(); i++) {
        std::istringstream issBlock(blocks[i]);
        std::string valueBlock;
        std::map<int, int> blockRepetitions; // Словарь для подсчета повторений чисел в блоке
        while (issBlock >> valueBlock) {
            int numBlock = std::stoi(valueBlock);
            blockRepetitions[numBlock]++;
        }
        std::cout << "Block " << i + 1 << ":" << std::endl;
        std::cout << "Block content: " << blocks[i] << std::endl;
        std::cout << "V(1) = " << blockRepetitions[1] << std::endl;
        std::cout << "V(2) = " << blockRepetitions[2] << std::endl;
        std::cout << "V(3) = " << blockRepetitions[3] << std::endl;
        std::cout << "V(4) = " << blockRepetitions[4] << std::endl;
        std::cout << "V(-4) = " << blockRepetitions[-4] << std::endl;
        std::cout << "V(-3) = " << blockRepetitions[-3] << std::endl;
        std::cout << "V(-2) = " << blockRepetitions[-2] << std::endl;
        std::cout << "V(-1) = " << blockRepetitions[-1] << std::endl;
        std::cout << std::endl;

        // Добавление значений V в соответствующие векторы
        v1_values.push_back(blockRepetitions[1]);
        v2_values.push_back(blockRepetitions[2]);
        v3_values.push_back(blockRepetitions[3]);
        v4_values.push_back(blockRepetitions[4]);
        v_neg4_values.push_back(blockRepetitions[-4]);
        v_neg3_values.push_back(blockRepetitions[-3]);
        v_neg2_values.push_back(blockRepetitions[-2]);
        v_neg1_values.push_back(blockRepetitions[-1]);
    }
    // Вывод значений V со всех блоков
    std::cout << "Values from all blocks:" << std::endl;
    std::cout << "V(1): ";
    int count0 = 0, count1 = 0, count2 = 0, count3 = 0, count4 = 0, count5 = 0;

    for (const auto& value : v1_values) {
        std::cout << value << " ";

        // Подсчет количества чисел 0, 1, 2, 3, 4 и 5
        switch (value) {
        case 0:
            count0++;
            break;
        case 1:
            count1++;
            break;
        case 2:
            count2++;
            break;
        case 3:
            count3++;
            break;
        case 4:
            count4++;
            break;
        case 5:
            count5++;
            break;
        default:
            if (value > 5 || count5 > 0) {
                count5++;
            }
            break;
        }
    }

    std::cout << std::endl;
    double K = 5.0;
    // Вывод количества чисел 0, 1, 2, 3, 4 и 5
    std::cout << "Count of numbers:" << std::endl;
    std::cout << "Count of 0: " << count0 << std::endl;
    std::cout << "Count of 1: " << count1 << std::endl;
    std::cout << "Count of 2: " << count2 << std::endl;
    std::cout << "Count of 3: " << count3 << std::endl;
    std::cout << "Count of 4: " << count4 << std::endl;
    std::cout << "Count of 5: " << count5 << std::endl;
    double p10 = 0.5000;
    double p11 = 0.2500;
    double p12 = 0.1250;
    double p13 = 0.0625;
    double p14 = 0.03125;
    double p15 = 0.03125;
    double OBS1 = (((count0 - zeroCount * p10) * (count0 - zeroCount * p10)) / (zeroCount * p10)) +
        (((count1 - zeroCount * p11) * (count1 - zeroCount * p11)) / (zeroCount * p11)) +
        (((count2 - zeroCount * p12) * (count2 - zeroCount * p12)) / (zeroCount * p12)) +
        (((count3 - zeroCount * p13) * (count3 - zeroCount * p13)) / (zeroCount * p13)) +
        (((count4 - zeroCount * p14) * (count4 - zeroCount * p14)) / (zeroCount * p14)) +
        (((count5 - zeroCount * p15) * (count5 - zeroCount * p15)) / (zeroCount * p15));
    double P1 = igamc(K, OBS1);


    std::cout << "V(2): ";
    int count2_0 = 0, count2_1 = 0, count2_2 = 0, count2_3 = 0, count2_4 = 0, count2_5 = 0;

    for (const auto& value : v2_values) {
        std::cout << value << " ";

        // Подсчет количества чисел 0, 1, 2, 3, 4 и 5
        switch (value) {
        case 0:
            count2_0++;
            break;
        case 1:
            count2_1++;
            break;
        case 2:
            count2_2++;
            break;
        case 3:
            count2_3++;
            break;
        case 4:
            count2_4++;
            break;
        case 5:
            count2_5++;
            break;
        default:
            if (value > 5 || count2_5 > 0) {
                count2_5++;
            }
            break;
        }
    }

    std::cout << std::endl;

    // Вывод количества чисел 0, 1, 2, 3, 4 и 5 для V(2)
    std::cout << "Count of numbers for V(2):" << std::endl;
    std::cout << "Count of 0: " << count2_0 << std::endl;
    std::cout << "Count of 1: " << count2_1 << std::endl;
    std::cout << "Count of 2: " << count2_2 << std::endl;
    std::cout << "Count of 3: " << count2_3 << std::endl;
    std::cout << "Count of 4: " << count2_4 << std::endl;
    std::cout << "Count of 5: " << count2_5 << std::endl;
    double p20 = 0.7500;
    double p21 = 0.0625;
    double p22 = 0.046875;
    double p23 = 0.0351562;
    double p24 = 0.0263672;
    double p25 = 0.0791016;
    double OBS2 = (((count2_0 - zeroCountAdjusted * p20) * (count2_0 - zeroCountAdjusted * p20)) / (zeroCountAdjusted * p20)) +
        (((count2_1 - zeroCountAdjusted * p21) * (count2_1 - zeroCountAdjusted * p21)) / (zeroCountAdjusted * p21)) +
        (((count2_2 - zeroCountAdjusted * p22) * (count2_2 - zeroCountAdjusted * p22)) / (zeroCountAdjusted * p22)) +
        (((count2_3 - zeroCountAdjusted * p23) * (count2_3 - zeroCountAdjusted * p23)) / (zeroCountAdjusted * p23)) +
        (((count2_4 - zeroCountAdjusted * p24) * (count2_4 - zeroCountAdjusted * p24)) / (zeroCountAdjusted * p24)) +
        (((count2_5 - zeroCountAdjusted * p25) * (count2_5 - zeroCountAdjusted * p25)) / (zeroCountAdjusted * p25));
    double P2 = igamc(K, OBS2);

    std::cout << "V(3): ";
    int count3_0 = 0, count3_1 = 0, count3_2 = 0, count3_3 = 0, count3_4 = 0, count3_5 = 0;

    for (const auto& value : v3_values) {
        std::cout << value << " ";

        // Подсчет количества чисел 0, 1, 2, 3, 4 и 5
        switch (value) {
        case 0:
            count3_0++;
            break;
        case 1:
            count3_1++;
            break;
        case 2:
            count3_2++;
            break;
        case 3:
            count3_3++;
            break;
        case 4:
            count3_4++;
            break;
        case 5:
            count3_5++;
            break;
        default:
            if (value > 5 || count3_5 > 0) {
                count3_5++;
            }
            break;
        }
    }

    std::cout << std::endl;

    // Вывод количества чисел 0, 1, 2, 3, 4 и 5 для V(2)
    std::cout << "Count of numbers for V(3):" << std::endl;
    std::cout << "Count of 0: " << count3_0 << std::endl;
    std::cout << "Count of 1: " << count3_1 << std::endl;
    std::cout << "Count of 2: " << count3_2 << std::endl;
    std::cout << "Count of 3: " << count3_3 << std::endl;
    std::cout << "Count of 4: " << count3_4 << std::endl;
    std::cout << "Count of 5: " << count3_5 << std::endl;
    double p30 = 0.875;
    double p31 = 0.0277778;
    double p32 = 0.0243056;
    double p33 = 0.0212674;
    double p34 = 0.0186089;
    double p35 = 0.0330404;
    double OBS3 = (((count3_0 - (zeroCountAdjusted * p30)) * (count3_0 - (zeroCountAdjusted * p30))) / (zeroCountAdjusted * p30)) +
        (((count3_1 - (zeroCountAdjusted * p31)) * (count3_1 - (zeroCountAdjusted * p31))) / (zeroCountAdjusted * p31)) +
        (((count3_2 - (zeroCountAdjusted * p32)) * (count3_2 - (zeroCountAdjusted * p32))) / (zeroCountAdjusted * p32)) +
        (((count3_3 - (zeroCountAdjusted * p33)) * (count3_3 - (zeroCountAdjusted * p33))) / (zeroCountAdjusted * p33)) +
        (((count3_4 - (zeroCountAdjusted * p34)) * (count3_4 - (zeroCountAdjusted * p34))) / (zeroCountAdjusted * p34)) +
        (((count3_5 - (zeroCountAdjusted * p35)) * (count3_5 - (zeroCountAdjusted * p35))) / (zeroCountAdjusted * p35));
    double P3 = igamc(K, OBS3);

    std::cout << "V(4): ";
    int count4_0 = 0, count4_1 = 0, count4_2 = 0, count4_3 = 0, count4_4 = 0, count4_5 = 0;

    for (const auto& value : v4_values) {
        std::cout << value << " ";

        // Counting the numbers 0, 1, 2, 3, 4, and 5
        switch (value) {
        case 0:
            count4_0++;
            break;
        case 1:
            count4_1++;
            break;
        case 2:
            count4_2++;
            break;
        case 3:
            count4_3++;
            break;
        case 4:
            count4_4++;
            break;
        case 5:
            count4_5++;
            break;
        default:
            if (value > 5 || count4_5 > 0) {
                count4_5++;
            }
            break;
        }
    }

    std::cout << std::endl;

    // Output the count of numbers 0, 1, 2, 3, 4, and 5 for V(4)
    std::cout << "Count of numbers for V(4):" << std::endl;
    std::cout << "Count of 0: " << count4_0 << std::endl;
    std::cout << "Count of 1: " << count4_1 << std::endl;
    std::cout << "Count of 2: " << count4_2 << std::endl;
    std::cout << "Count of 3: " << count4_3 << std::endl;
    std::cout << "Count of 4: " << count4_4 << std::endl;
    std::cout << "Count of 5: " << count4_5 << std::endl;
    double p40 = 0.9375;
    double p41 = 0.015625;
    double p42 = 0.0146484;
    double p43 = 0.0137329;
    double p44 = 0.0128746;
    double p45 = 0.00561905;
    double OBS4 = (((count4_0 - zeroCountAdjusted * p40) * (count4_0 - zeroCountAdjusted * p40)) / (zeroCountAdjusted * p40)) +
        (((count4_1 - zeroCountAdjusted * p41) * (count4_1 - zeroCountAdjusted * p41)) / (zeroCountAdjusted * p41)) +
        (((count4_2 - zeroCountAdjusted * p42) * (count4_2 - zeroCountAdjusted * p42)) / (zeroCountAdjusted * p42)) +
        (((count4_3 - zeroCountAdjusted * p43) * (count4_3 - zeroCountAdjusted * p43)) / (zeroCountAdjusted * p43)) +
        (((count4_4 - zeroCountAdjusted * p44) * (count4_4 - zeroCountAdjusted * p44)) / (zeroCountAdjusted * p44)) +
        (((count4_5 - zeroCountAdjusted * p45) * (count4_5 - zeroCountAdjusted * p45)) / (zeroCountAdjusted * p45));
    double P4 = igamc(K, OBS4);

    std::cout << "V(-4): ";
    int count_neg4_0 = 0, count_neg4_1 = 0, count_neg4_2 = 0, count_neg4_3 = 0, count_neg4_4 = 0, count_neg4_5 = 0;

    for (const auto& value : v_neg4_values) {
        std::cout << value << " ";

        // Counting the numbers 0, 1, 2, 3, 4, and 5
        switch (value) {
        case 0:
            count_neg4_0++;
            break;
        case 1:
            count_neg4_1++;
            break;
        case 2:
            count_neg4_2++;
            break;
        case 3:
            count_neg4_3++;
            break;
        case 4:
            count_neg4_4++;
            break;
        case 5:
            count_neg4_5++;
            break;
        default:
            if (value > 5 || count_neg4_5 > 0) {
                count_neg4_5++;
            }
            break;
        }
    }

    std::cout << std::endl;

    // Output the count of numbers 0, 1, 2, 3, 4, and 5 for V(-4)
    std::cout << "Count of numbers for V(-4):" << std::endl;
    std::cout << "Count of 0: " << count_neg4_0 << std::endl;
    std::cout << "Count of 1: " << count_neg4_1 << std::endl;
    std::cout << "Count of 2: " << count_neg4_2 << std::endl;
    std::cout << "Count of 3: " << count_neg4_3 << std::endl;
    std::cout << "Count of 4: " << count_neg4_4 << std::endl;
    std::cout << "Count of 5: " << count_neg4_5 << std::endl;

    double OBSn4 = (((count_neg4_0 - zeroCountAdjusted * p40) * (count_neg4_0 - zeroCountAdjusted * p40)) / (zeroCountAdjusted * p40)) +
        (((count_neg4_1 - zeroCountAdjusted * p41) * (count_neg4_1 - zeroCountAdjusted * p41)) / (zeroCountAdjusted * p41)) +
        (((count_neg4_2 - zeroCountAdjusted * p42) * (count_neg4_2 - zeroCountAdjusted * p42)) / (zeroCountAdjusted * p42)) +
        (((count_neg4_3 - zeroCountAdjusted * p43) * (count_neg4_3 - zeroCountAdjusted * p43)) / (zeroCountAdjusted * p43)) +
        (((count_neg4_4 - zeroCountAdjusted * p44) * (count_neg4_4 - zeroCountAdjusted * p44)) / (zeroCountAdjusted * p44)) +
        (((count_neg4_5 - zeroCountAdjusted * p45) * (count_neg4_5 - zeroCountAdjusted * p45)) / (zeroCountAdjusted * p45));
    double Pn4 = igamc(K, OBSn4);


    std::cout << "V(-3): ";
    int count_neg3_0 = 0, count_neg3_1 = 0, count_neg3_2 = 0, count_neg3_3 = 0, count_neg3_4 = 0, count_neg3_5 = 0;

    for (const auto& value : v_neg3_values) {
        std::cout << value << " ";

        // Counting the numbers 0, 1, 2, 3, 4, and 5
        switch (value) {
        case 0:
            count_neg3_0++;
            break;
        case 1:
            count_neg3_1++;
            break;
        case 2:
            count_neg3_2++;
            break;
        case 3:
            count_neg3_3++;
            break;
        case 4:
            count_neg3_4++;
            break;
        case 5:
            count_neg3_5++;
            break;
        default:
            if (value > 5 || count_neg3_5 > 0) {
                count_neg3_5++;
            }
            break;
        }
    }

    std::cout << std::endl;

    // Output the count of numbers 0, 1, 2, 3, 4, and 5 for V(-3)
    std::cout << "Count of numbers for V(-3):" << std::endl;
    std::cout << "Count of 0: " << count_neg3_0 << std::endl;
    std::cout << "Count of 1: " << count_neg3_1 << std::endl;
    std::cout << "Count of 2: " << count_neg3_2 << std::endl;
    std::cout << "Count of 3: " << count_neg3_3 << std::endl;
    std::cout << "Count of 4: " << count_neg3_4 << std::endl;
    std::cout << "Count of 5: " << count_neg3_5 << std::endl;
    double OBSn3 = (((count_neg3_0 - zeroCountAdjusted * p30) * (count_neg3_0 - zeroCountAdjusted * p30)) / (zeroCountAdjusted * p30)) +
        (((count_neg3_1 - zeroCountAdjusted * p31) * (count_neg3_1 - zeroCountAdjusted * p31)) / (zeroCountAdjusted * p31)) +
        (((count_neg3_2 - zeroCountAdjusted * p32) * (count_neg3_2 - zeroCountAdjusted * p32)) / (zeroCountAdjusted * p32)) +
        (((count_neg3_3 - zeroCountAdjusted * p33) * (count_neg3_3 - zeroCountAdjusted * p33)) / (zeroCountAdjusted * p33)) +
        (((count_neg3_4 - zeroCountAdjusted * p34) * (count_neg3_4 - zeroCountAdjusted * p34)) / (zeroCountAdjusted * p34)) +
        (((count_neg3_5 - zeroCountAdjusted * p35) * (count_neg3_5 - zeroCountAdjusted * p35)) / (zeroCountAdjusted * p35));
    double Pn3 = igamc(K, OBSn3);

    std::cout << "V(-2): ";

    int count_neg2_0 = 0, count_neg2_1 = 0, count_neg2_2 = 0, count_neg2_3 = 0, count_neg2_4 = 0, count_neg2_5 = 0;

    for (const auto& value : v_neg2_values) {
        std::cout << value << " ";

        // Counting the numbers 0, 1, 2, 3, 4, and 5
        switch (value) {
        case 0:
            count_neg2_0++;
            break;
        case 1:
            count_neg2_1++;
            break;
        case 2:
            count_neg2_2++;
            break;
        case 3:
            count_neg2_3++;
            break;
        case 4:
            count_neg2_4++;
            break;
        case 5:
            count_neg2_5++;
            break;
        default:
            if (value > 5 || count_neg2_5 > 0) {
                count_neg2_5++;
            }
            break;
        }
    }

    std::cout << std::endl;

    // Output the count of numbers 0, 1, 2, 3, 4, and 5 for V(-2)
    std::cout << "Count of numbers for V(-2):" << std::endl;
    std::cout << "Count of 0: " << count_neg2_0 << std::endl;
    std::cout << "Count of 1: " << count_neg2_1 << std::endl;
    std::cout << "Count of 2: " << count_neg2_2 << std::endl;
    std::cout << "Count of 3: " << count_neg2_3 << std::endl;
    std::cout << "Count of 4: " << count_neg2_4 << std::endl;
    std::cout << "Count of 5: " << count_neg2_5 << std::endl;
    double OBSn2 = (((count_neg2_0 - zeroCountAdjusted * p20) * (count_neg2_0 - zeroCountAdjusted * p20)) / (zeroCountAdjusted * p20)) +
        (((count_neg2_1 - zeroCountAdjusted * p21) * (count_neg2_1 - zeroCountAdjusted * p21)) / (zeroCountAdjusted * p21)) +
        (((count_neg2_2 - zeroCountAdjusted * p22) * (count_neg2_2 - zeroCountAdjusted * p22)) / (zeroCountAdjusted * p22)) +
        (((count_neg2_3 - zeroCountAdjusted * p23) * (count_neg2_3 - zeroCountAdjusted * p23)) / (zeroCountAdjusted * p23)) +
        (((count_neg2_4 - zeroCountAdjusted * p24) * (count_neg2_4 - zeroCountAdjusted * p24)) / (zeroCountAdjusted * p24)) +
        (((count_neg2_5 - zeroCountAdjusted * p25) * (count_neg2_5 - zeroCountAdjusted * p25)) / (zeroCountAdjusted * p25));
    double Pn2 = igamc(K, OBSn2);

    std::cout << "V(-1): ";
    int count_neg1_0 = 0, count_neg1_1 = 0, count_neg1_2 = 0, count_neg1_3 = 0, count_neg1_4 = 0, count_neg1_5 = 0;

    for (const auto& value : v_neg1_values) {
        std::cout << value << " ";

        // Counting the numbers 0, 1, 2, 3, 4, and 5
        switch (value) {
        case 0:
            count_neg1_0++;
            break;
        case 1:
            count_neg1_1++;
            break;
        case 2:
            count_neg1_2++;
            break;
        case 3:
            count_neg1_3++;
            break;
        case 4:
            count_neg1_4++;
            break;
        case 5:
            count_neg1_5++;
            break;
        default:
            if (value > 5 || count_neg1_5 > 0) {
                count_neg1_5++;
            }
            break;
        }
    }

    std::cout << std::endl;

    // Output the count of numbers 0, 1, 2, 3, 4, and 5 for V(-1)
    std::cout << "Count of numbers for V(-1):" << std::endl;
    std::cout << "Count of 0: " << count_neg1_0 << std::endl;
    std::cout << "Count of 1: " << count_neg1_1 << std::endl;
    std::cout << "Count of 2: " << count_neg1_2 << std::endl;
    std::cout << "Count of 3: " << count_neg1_3 << std::endl;
    std::cout << "Count of 4: " << count_neg1_4 << std::endl;
    std::cout << "Count of 5: " << count_neg1_5 << std::endl;
    double OBSn1 = (((count_neg1_0 - zeroCountAdjusted * p10) * (count_neg1_0 - zeroCountAdjusted * p10)) / (zeroCountAdjusted * p10)) +
        (((count_neg1_1 - zeroCountAdjusted * p11) * (count_neg1_1 - zeroCountAdjusted * p11)) / (zeroCountAdjusted * p11)) +
        (((count_neg1_2 - zeroCountAdjusted * p12) * (count_neg1_2 - zeroCountAdjusted * p12)) / (zeroCountAdjusted * p12)) +
        (((count_neg1_3 - zeroCountAdjusted * p13) * (count_neg1_3 - zeroCountAdjusted * p13)) / (zeroCountAdjusted * p13)) +
        (((count_neg1_4 - zeroCountAdjusted * p14) * (count_neg1_4 - zeroCountAdjusted * p14)) / (zeroCountAdjusted * p14)) +
        (((count_neg1_5 - zeroCountAdjusted * p15) * (count_neg1_5 - zeroCountAdjusted * p15)) / (zeroCountAdjusted * p15));
    double Pn1 = igamc(K, OBSn1);


    std::cout << std::setprecision(30) << std::fixed;
    std::cout << "J = " << zeroCountAdjusted << std::endl;
    std::cout << "Chi2_n4 = " << OBSn4 << "	" << "P-value = " << Pn4 << std::endl;
    std::cout << "Chi2_n3 = " << OBSn3 << "	" << "P-value = " << Pn3 << std::endl;
    std::cout << "Chi2_n2 = " << OBSn2 << "	" << "P-value = " << Pn2 << std::endl;
    std::cout << "Chi2_n1 = " << OBSn1 << "	" << "P-value = " << Pn1 << std::endl;
    std::cout << "Chi2_1 = " << OBS1 << "	" << "P-value = " << P1 << std::endl;
    std::cout << "Chi2_2 = " << OBS2 << "	" << "P-value = " << P2 << std::endl;
    std::cout << "Chi2_3 = " << OBS3 << "	" << "P-value = " << P3 << std::endl;
    std::cout << "Chi2_4 = " << OBS4 << "	" << "P-value = " << P4 << std::endl;
}


