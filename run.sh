#!/bin/bash

# Run your command or program here
./port 5.4.6.4 43

# Check the exit code
if [ $? -eq 0 ]; then
    echo "Command succeeded (exit code 0)"
else
    echo "Command failed (exit code $?)"
fi
