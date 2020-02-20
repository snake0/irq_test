sudo rmmod irq_switch.ko
make
sleep 10
sudo insmod irq_switch.ko
sleep 1
dmesg
