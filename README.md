sudo apt-get install parallel

./your_program | tee -a iplist.txt

cat iplist.txt | grep open
