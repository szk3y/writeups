#!/bin/sh
filename=shellcode.asm
interval=1
last_timestamp=$(ls --full-time $filename | awk '{ print $6 $7 }')
while :
do
  sleep $interval
  current_timestamp=$(ls --full-time $filename | awk '{ print $6 $7 }')
  if [ $current_timestamp != $last_timestamp ] ; then
    make
    last_timestamp=$current_timestamp
  fi
done
