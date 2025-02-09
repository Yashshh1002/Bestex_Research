#include "MarketDataMerger.h"
#include <iostream>
#include <filesystem>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

MarketDataMerger::MarketDataMerger(const std::string &inputDir, const std::string &outputFile)
    : inputDir(inputDir), outputFile(outputFile) {}

void MarketDataMerger::loadInitialEntries()
{
    for (const auto &entry : fs::directory_iterator(inputDir))
    {
        if (entry.is_regular_file())
        {
            std::ifstream &file = fileStreams[entry.path().filename().string()];
            file.open(entry.path());
            if (!file.is_open())
                continue;

            std::string header;
            std::getline(file, header);

            MarketDataEntry data;
            data.file = &file;
            data.symbol = entry.path().filename().stem().string();

            if (loadNextEntry(data))
            {
                minHeap.push(data);
            }
        }
    }
}

bool MarketDataMerger::loadNextEntry(MarketDataEntry &entry)
{
    std::string line;
    if (std::getline(*entry.file, line))
    {
        std::istringstream iss(line);

        std::getline(iss, entry.timestamp, ',');
        if (entry.timestamp.empty())
            return false;

        iss >> entry.price;
        iss.ignore();
        iss >> entry.size;
        iss.ignore();
        std::getline(iss, entry.exchange, ',');
        std::getline(iss, entry.type, ',');

        if (entry.timestamp.empty() || entry.exchange.empty() || entry.type.empty())
        {
            return false;
        }

        return true;
    }
    return false;
}

void MarketDataMerger::mergeFiles()
{
    loadInitialEntries();
    std::ofstream outFile(outputFile);
    if (!outFile.is_open())
    {
        throw std::runtime_error("Failed to open output file");
    }

    outFile << "Symbol, Timestamp, Price, Size, Exchange, Type\n";

    while (!minHeap.empty())
    {
        MarketDataEntry entry = minHeap.top();
        minHeap.pop();

        outFile << entry.symbol << ", " << entry.timestamp << ", "
                << std::fixed << std::setprecision(2) << entry.price << ", "
                << entry.size << ", " << entry.exchange << ", " << entry.type << "\n";

        if (loadNextEntry(entry))
        {
            minHeap.push(entry);
        }
    }

    outFile.close();
    for (auto &[_, file] : fileStreams)
    {
        file.close();
    }
}
