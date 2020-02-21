cd $1
sudo rmmod kernel_$1_test.ko
make
echo "waiting..."
sleep 10
echo "started!"
sudo insmod kernel_$1_test.ko
cd ..

