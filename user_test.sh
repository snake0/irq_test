cd $1
gcc ./user_$1_test.c -O3 -lm
echo "waiting..."
sleep 10
echo "started!"
./a.out
cd ..

