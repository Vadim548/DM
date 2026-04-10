#!/bin/bash

# FIFO paths
PUBLIC_KEY_FIFO="/tmp/rsa/public_key"
CIPHERTEXT_FIFO="/tmp/rsa/ciphertext"
READY_FIFO="/tmp/rsa/ready"

while [ ! -f "$READY_FIFO" ]; do
    # echo "not ready"
    sleep 0.1
done
while [ ! -s $CIPHERTEXT_FIFO ]; do
    # echo "not cipher"
    sleep 0.1
done
while [ ! -s $PUBLIC_KEY_FIFO ]; do
    # echo "not public"
    sleep 0.1
done

# cat "$READY_FIFO" > /dev/null  # Consume the "ready" signal

PUBLIC_KEY=$(cat "$PUBLIC_KEY_FIFO")

CIPHERTEXT=$(cat "$CIPHERTEXT_FIFO")

# echo ./rsa -c $PUBLIC_KEY $CIPHERTEXT
./rsa -c $PUBLIC_KEY $CIPHERTEXT