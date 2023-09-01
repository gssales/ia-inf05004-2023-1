with open('8puzzle_instances.txt') as f:
    instances = f.readlines()

with open('8puzzle-bfs.csv') as f:
    results = f.readlines()

for i in range(len(instances)):
    metrics = results[i].strip().split(',')
    print(f'{instances[i].strip()} & {" & ".join(metrics)} \\\\ \\hline')