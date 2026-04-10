#!/bin/bash

PUBLIC_KEY_FIFO="public_key"
CIPHERTEXT_FIFO="ciphertext"
READY_FIFO="ready"
mkdir -p /tmp/rsa
rm -f /tmp/rsa/*
# touch /tmp/rsa/{$PUBLIC_KEY_FIFO,$CIPHERTEXT_FIFO,$READY_FIFO}

KEYS=$(./rsa -g)
PUBLIC_KEY=$(echo "$KEYS" | head -n 1)
PRIVATE_KEY=$(echo "$KEYS" | tail -n 1)

echo "Public key (not visible for a cracker): $PUBLIC_KEY"
echo "Private key (not visible for a cracker): $PRIVATE_KEY"

# Share only the public key
echo "$PUBLIC_KEY" > /tmp/rsa/$PUBLIC_KEY_FIFO
echo "ready" > /tmp/rsa/$READY_FIFO

# Wait for ciphertext
while [ ! -s /tmp/rsa/$CIPHERTEXT_FIFO ]; do
    # echo "not cipher"
    sleep 0.1
done
CIPHERTEXT=$(cat /tmp/rsa/$CIPHERTEXT_FIFO)

# echo "$PRIVATE_KEY" | awk '{print $1, $2}' | xargs -n 2 -t 
# echo ./rsa -d $PRIVATE_KEY $CIPHERTEXT
./rsa -d $PRIVATE_KEY $CIPHERTEXT


