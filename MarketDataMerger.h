#ifndef MARKET_DATA_MERGER_H
#define MARKET_DATA_MERGER_H

#include <string>
#include <queue>
#include <vector>
#include <fstream>
#include <map>

struct MarketDataEntry
{
    std::string symbol;
    std::string timestamp;
    double price;
    int size;
    std::string exchange;
    std::string type;
    std::ifstream *file;

    bool operator>(const MarketDataEntry &other) const
    {
        if (timestamp == other.timestamp)
            return symbol > other.symbol;
        return timestamp > other.timestamp;
    }
};

class MarketDataMerger
{
public:
    MarketDataMerger(const std::string &inputDir, const std::string &outputFile);
    void mergeFiles();

private:
    std::string inputDir;
    std::string outputFile;
    std::priority_queue<MarketDataEntry, std::vector<MarketDataEntry>, std::greater<>> minHeap;
    std::map<std::string, std::ifstream> fileStreams;

    void loadInitialEntries();
    bool loadNextEntry(MarketDataEntry &entry);
};

#endif
