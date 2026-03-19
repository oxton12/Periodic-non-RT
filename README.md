## Non-RT
g++ src/mainNonRT.cpp src/ThreadStopwatch.cpp src/LoaderNonRT.cpp  src/Incrementinator.cpp -Iinclude -o main

## RT
g++ src/mainRT.cpp src/ThreadStopwatch.cpp src/LoaderRT.cpp src/Incrementinator.cpp -Iinclude -I/usr/evl/include -L/usr/evl/lib -levl -o main