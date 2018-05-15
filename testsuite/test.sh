#!/bin/bash

for bson_file in test-cases/bson/test*.bson; do
	filename=$(basename "$bson_file" .bson)
	echo "$filename"
	./testbinson "$bson_file" > "/tmp/$filename.json"
    #echo "/tmp/$filename.json" "test-cases/json/$filename.json"
	diff "/tmp/$filename.json" "test-cases/json/$filename.json"
done
