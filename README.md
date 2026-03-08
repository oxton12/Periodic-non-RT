## Non-RT
g++ src/mainNonRT.cpp src/ThreadStopwatch.cpp src/LoaderNonRT.cpp -Iinclude -o main

## RT
g++ src/mainRT.cpp src/ThreadStopwatch.cpp src/LoaderRT.cpp -Iinclude -I/usr/evl/include -L/usr/evl/lib -levl -o main