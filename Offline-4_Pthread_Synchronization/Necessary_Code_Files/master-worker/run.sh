#!/bin/bash

cnt=1

for total_items in {1..10}; 
do
	for buffer_size in {1..10};
	do
		for workers in {1..10};
		do
			for masters in {1..10};
			do
				echo -n "$cnt: "
				./test-master-worker.sh $total_items $buffer_size $workers $masters
				cnt=$(($cnt + 1))
			done
		done
	done
done

echo "Some Large Cases:"
./test-master-worker.sh 1544658 6853 1256 783
./test-master-worker.sh 1544658 6853 456 523
