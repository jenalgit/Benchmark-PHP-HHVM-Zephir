#! /usr/bin/env bash
echo "Starting FCGI process"
$HPHP_HOME/hphp/hhvm/hhvm --mode server -vServer.Type=fastcgi -vServer.FileSocket="/tmp/treffynnon_bench.socket" -vEval.Jit=0 > /dev/null 2>&1 &
echo "Sleep for a bit to allow process to ready itself"
sleep 5

echo "Attempting to warm up the server"
for ((n=0;n<15;n++)); do
    # Using a low number for the seed to make the warm up faster
    ./"$1" 100 > /dev/null 2>&1
done
