while true;do
    cat /proc/cpuinfo |grep 'cpu MHz'
    echo '-----------------------------'
    sleep 0.5
done