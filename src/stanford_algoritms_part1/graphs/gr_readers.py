__author__ = 'zaqwes'


def get_fake_graph():
    g = {
        1: [4],
        2: [8],
        3: [6],
        4: [7],
        5: [2],
        6: [9],
        7: [1],
        8: [6, 5],
        9: [3, 7]
    }
    return g


def get_real_graph():
    filename = '/home/zaqwes/tmp/SCC.txt'
    f = open(filename, 'rt')
    graph = {}
    for i in range(1, 875714 + 1):
        graph[i] = []

    lines = f.readlines()
    for line in lines:
        pair = line.lstrip().rstrip().split(' ')
        assert len(pair) == 2
        source = int(pair[0])
        destination = int(pair[1])
        graph[source].append(destination)

    return graph