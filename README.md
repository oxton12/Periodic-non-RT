Assignment 2.1
−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
1. Build command
g++ src/mainNonRT.cpp src/ThreadStopwatch.cpp src/LoaderNonRT.cpp  src/Incrementinator.cpp -Iinclude -o main

2. Run command:
./main

3. Wait for the output


Assignment 2.2
−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−−
(Must be executed on a device with set up Xenomai4)

1. Build command
g++ src/mainRT.cpp src/ThreadStopwatch.cpp src/LoaderRT.cpp src/Incrementinator.cpp -Iinclude -I/usr/evl/include -L/usr/evl/lib -levl -o main

2. Run command:
sudo ./main

3. Wait for the output