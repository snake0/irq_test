sudo rmmod kernel_$1_test.ko

cd $1
make

if [ $? -ne 0 ]; then
  echo "\nmake failed. exit."
  exit
else
  echo "\nwaiting..."
  sleep 10
  echo "started!"
  sudo insmod kernel_$1_test.ko
  cd ..
fi
