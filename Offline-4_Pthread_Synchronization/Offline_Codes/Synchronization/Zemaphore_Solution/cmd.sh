echo "TESTSET: Running Testcases"
g++ test.cpp zemaphore.c -o test -lpthread
./test 1 5