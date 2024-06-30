#!/bin/bash

input_file="performance_metrics.csv"
output_file="sorted_performance_metrics.csv"

# Extract the header
header=$(head -n 1 "$input_file")
header="$header,Total Time(ms)"

# Extract data without header, calculate sum, and sort
tail -n +2 "$input_file" | \
awk -F, '{ totalTime = $5 + $6; print $0 "," totalTime }' | \
sort -t, -k7,7n | \
awk -F, 'BEGIN {OFS=","} { print $1, $2, $3, $4, $5, $6, $7 }' > sorted_data.csv

# Combine header and sorted data
echo "$header" > "$output_file"
cat sorted_data.csv >> "$output_file"

# Clean up
rm sorted_data.csv
