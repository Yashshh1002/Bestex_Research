CC = g++
CFLAGS = -std=c++17 -Wall

TARGET = market_data_merger
TEST_TARGET = test_runner

SRC = main.cpp MarketDataMerger.cpp
TEST_SRC = test_market_data_merger.cpp MarketDataMerger.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC)
	$(CC) $(CFLAGS) -o $(TEST_TARGET) $(TEST_SRC)

clean:
	rm -f $(TARGET) $(TEST_TARGET) test_output.txt
	rm -rf test_data
