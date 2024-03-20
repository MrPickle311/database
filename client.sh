#!/bin/bash

HOST="$1"
PORT="$2"

if [ -z "$HOST" ] || [ -z "$PORT" ]; then
    echo "Bad usage! Usage: ./client.sh HOST PORT"
    exit 1
fi

send_query() {
    read -r query

    query="$query |"

    response=$(echo "$query" | netcat "$HOST" "$PORT")

    echo "$response"
}

while true; do
    echo -n ">> "

    send_query

    echo ""
done
