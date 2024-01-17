#!/bin/bash

# Function to convert IP parts to integer
ip_to_int() {
  local a b c d
  IFS=. read -r a b c d <<< "$1"
  echo $((a * 256 ** 3 + b * 256 ** 2 + c * 256 + d))
}

# Function to convert integer to IP
int_to_ip() {
  local num=$1
  echo "$((num >> 24 & 255)).$((num >> 16 & 255)).$((num >> 8 & 255)).$((num & 255))"
}

# Function to iterate through IP range
iterate_ip_range() {
  local start_ip end_ip
  start_ip=$(ip_to_int $1)
  end_ip=$(ip_to_int $2)

  for ((i = start_ip; i <= end_ip; i++)); do
    echo $(int_to_ip $i)
    
       
# Run your command or program here
./port $(int_to_ip $i) 443

# Check the exit code
if [ $? -eq 0 ]; then
    echo "Command succeeded (exit code 0)"
    echo $(int_to_ip $i) >> iplist.txt
else
    echo "Command failed (exit code $?)"
fi
       
  done
}

# Example usage: iterate_ip_range <start_ip> <end_ip>
iterate_ip_range "142.250.195.142" "142.250.196.142"

