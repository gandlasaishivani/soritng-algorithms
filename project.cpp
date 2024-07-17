#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

// Test function prototype
bool testAlgorithm(const std::string& code, const std::string& algorithmType);

int main() {
    std::string algorithmType;
    std::cout << "Enter the algorithm type (sort/search): ";
    std::getline(std::cin, algorithmType);

    std::string code;
    std::cout << "Enter the " << algorithmType << " algorithm code: ";
    std::getline(std::cin, code);

    if (testAlgorithm(code, algorithmType)) {
        std::cout << "Yes\n";
    } else {
        std::cout << "No\n";
    }

    return 0;
}

bool testAlgorithm(const std::string& code, const std::string& algorithmType) {
    std::string funcSignature, fullCode;

    if (algorithmType == "sort") {
        // Function signature for the sort function
        funcSignature = "void sort(std::vector<int>& arr)";
        // Complete code with the function signature and the user-provided code
        fullCode = R"(
#include <vector>
#include <iostream>
#include <algorithm> // for std::swap
)" + funcSignature + " {\n" + code + "\n}\n" + R"(
int main() {
    std::vector<int> arr = {4, 2, 7, 1, 3};
    sort(arr);
    for (int num : arr) {
        std::cout << num << " ";
    }
    return 0;
}
)";
    } else if (algorithmType == "search") {
        // Function signature for the search function
        funcSignature = "int search(const std::vector<int>& arr, int target)";
        // Complete code with the function signature and the user-provided code
        fullCode = R"(
#include <vector>
#include <iostream>
)" + funcSignature + " {\n" + code + "\n}\n" + R"(
int main() {
    std::vector<int> arr = {4, 2, 7, 1, 3};
    int target = 7;
    int index = search(arr, target);
    std::cout << index;
    return 0;
}
)";
    } else {
        std::cerr << "Unknown algorithm type\n";
        return false;
    }

    // Write the code to a temporary file
    std::ofstream tmpFile("temp.cpp");
    if (!tmpFile.is_open()) {
        std::cerr << "Error opening file for writing\n";
        return false;
    }
    tmpFile << fullCode;
    tmpFile.close();

    // Compile the temporary file
    if (system("g++ temp.cpp -o temp") != 0) {
        std::cerr << "Compilation failed\n";
        return false;
    }

    // Run the compiled program and capture the output
    if (system("temp > output.txt") != 0) {
        std::cerr << "Execution failed\n";
        return false;
    }

    // Read the output from the output file
    std::ifstream outputFile("output.txt");
    if (!outputFile.is_open()) {
        std::cerr << "Error opening output file\n";
        return false;
    }

    std::vector<int> arr;
    int num;
    while (outputFile >> num) {
        arr.push_back(num);
    }
    outputFile.close();

    if (algorithmType == "sort") {
        // Expected output for sort
        std::vector<int> expected = {1, 2, 3, 4, 7};
        // Compare the output with the expected result
        return arr == expected;
    } else if (algorithmType == "search") {
        // Expected output for search
        int expectedIndex = 2; // for target = 7
        // Compare the output with the expected result
        return !arr.empty() && arr[0] == expectedIndex;
    }

    return false;
}
