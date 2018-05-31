#!/bin/bash

source=shellcode.asm
interval=1

build() {
  make
}

get_ts() {
  date -r $1
}

last_ts=$(get_ts $source)
build
while true; do
  sleep $interval
  current_ts=$(get_ts $source)
  if [ "$last_ts" != "$current_ts" ]; then
    last_ts="$current_ts"
    build
  fi
done
