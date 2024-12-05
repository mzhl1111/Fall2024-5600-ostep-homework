#!/bin/bash

echo "===================== Running wrk benchmark ====================="

echo "## $1" >> wrk-taskset.md

echo "\`\`\`" >> wrk-taskset.md

# Run `wrk` in the background
wrk -t8 -c500 -d30s http://127.0.0.1:8080 >> wrk-taskset.md 

# # PID of `wrk` process
# WRK_PID=$!

# # Monitor CPU usage while `wrk` is running
# echo "Monitoring CPU usage..." >> wrk-taskset.md
# echo "\nCPU Usage:" >> wrk-taskset.md

# while kill -0 "$WRK_PID" 2>/dev/null; do
#   # Capture the overall CPU usage summary using `top` and filter the percentage
#   top -l 1 | grep "CPU usage" >> wrk-taskset.md
#   sleep 1
# done

echo "\`\`\`" >> wrk-taskset.md