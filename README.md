sudo apt-get install parallel

./run.sh | tee -a iplist.txt

cat iplist.txt | grep open
