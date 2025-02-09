#include <iostream>
#include "MarketDataMerger.h"

int main(int argc, char *argv[])
{
    if (argc < 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input_directory> <output_file>\n";
        return 1;
    }

    std::string inputDir = argv[1];
    std::string outputFile = argv[2];

    try
    {
        MarketDataMerger merger(inputDir, outputFile);
        merger.mergeFiles();
        std::cout << "Merging completed successfully. Output file: " << outputFile << "\n";
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
