#!/bin/bash

PUBLIC_KEY_FIFO="/tmp/rsa/public_key"
CIPHERTEXT_FIFO="/tmp/rsa/ciphertext"
READY_FIFO="/tmp/rsa/ready"

# Wait for the ready signal
while [ ! -f "$READY_FIFO" ]; do
    sleep 0.1
done
# cat "$READY_FIFO" > /dev/null  # Consume the "ready" signal

PUBLIC_KEY=$(cat "$PUBLIC_KEY_FIFO")
echo $PUBLIC_KEY

read -p "Enter message: " MESSAGE

CIPHERTEXT=$(
    echo "$PUBLIC_KEY" | awk '{print $1, $2}' | xargs -n 2 ./rsa -e "$MESSAGE"
)
echo "$CIPHERTEXT" > "$CIPHERTEXT_FIFO"