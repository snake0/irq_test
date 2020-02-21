sudo rmmod kernel_cpu_test.ko
make
sleep 10
sudo insmod kernel_cpu_test.ko
