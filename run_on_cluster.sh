#!/bin/bash
make all
scp start_shortest_path_faster.sh technikum-cluster:~/dev/spf/
ssh technikum-cluster "chmod +x ~/dev/spf/start_shortest_path_faster.sh"
scp ./out/* technikum-cluster:~/dev/spf/
ssh technikum-cluster '~/dev/spf/start_shortest_path_faster.sh cluster 45'
scp technikum-cluster:shortest_path_faster_result.txt ./shortest_path_faster_result.txt