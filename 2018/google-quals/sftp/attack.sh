#!/bin/bash

rand/rand_predictor | head -n 5 | cut -d' ' -f3 | xargs ./exploit.py $1
