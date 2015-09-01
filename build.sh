#!/bin/sh
set -e

printf "Build binaries\n"
make

printf "Copying files to bin/ directory\n"
cp ./client/client.a ./bin/client.a
cp ./server/server.a ./bin/server.a

printf "Cleaning intermediate files\n"
make clean

