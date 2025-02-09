#include <iostream>
#include <fstream>
#include <cassert>
#include "MarketDataMerger.h"
#include <filesystem>
namespace fs = std::filesystem;

void createTestFile(const std::string &filename, const std::string &content)
{
    std::ofstream file(filename);
    file << content;
    file.close();
}

bool verifyOutputFile(const std::string &outputFile, const std::string &expectedContent)
{
    std::ifstream file(outputFile);
    std::string line, actualContent;

    while (std::getline(file, line))
    {
        actualContent += line + "\n";
    }
    file.close();

    return actualContent == expectedContent;
}

void testMarketDataMerger()
{
    std::string testDir = "test_data";
    std::string outputFile = "test_output.txt";

    fs::create_directory(testDir);

    createTestFile(testDir + "/CSCO.txt",
                   "Timestamp, Price, Size, Exchange, Type\n"
                   "2021-03-05 10:00:00.123, 46.14, 120, NYSE_ARCA, Ask\n"
                   "2021-03-05 10:00:00.130, 46.13, 120, NYSE, TRADE\n");

    createTestFile(testDir + "/MSFT.txt",
                   "Timestamp, Price, Size, Exchange, Type\n"
                   "2021-03-05 10:00:00.123, 228.5, 120, NYSE, Ask\n"
                   "2021-03-05 10:00:00.134, 228.5, 120, NYSE_ARCA, Ask\n");

    MarketDataMerger merger(testDir, outputFile);
    merger.mergeFiles();

    std::string expectedOutput =
        "Symbol, Timestamp, Price, Size, Exchange, Type\n"
        "CSCO, 2021-03-05 10:00:00.123, 46.14, 120, NYSE_ARCA, Ask\n"
        "MSFT, 2021-03-05 10:00:00.123, 228.5, 120, NYSE, Ask\n"
        "CSCO, 2021-03-05 10:00:00.130, 46.13, 120, NYSE, TRADE\n"
        "MSFT, 2021-03-05 10:00:00.134, 228.5, 120, NYSE_ARCA, Ask\n";

    assert(verifyOutputFile(outputFile, expectedOutput));

    std::cout << "✅ Unit Test Passed: MarketDataMerger correctly merges files!\n";

    fs::remove_all(testDir);
    fs::remove(outputFile);
}

int main()
{
    testMarketDataMerger();
    return 0;
}
