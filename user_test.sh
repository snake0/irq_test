cd $1

gcc ./user_$1_test.c -O3 -lm -lpthread

if [ $? -ne 0 ]; then
  echo "\ngcc failed. exit."
  exit
else
  echo "waiting..."
  # sleep 3
  echo "started!"
  ./a.out
  cd ..
fi

