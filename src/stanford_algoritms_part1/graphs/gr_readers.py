
import util

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
    return g, util.invert_digraph(g)


def get_real_graph():
    filename = '/home/zaqwes/tmp/SCC.txt'
    f = open(filename, 'rt')
    graph = {}
    g_inv = {}
    for i in range(1, 875714 + 1):
        graph[i] = []
        g_inv[i] = []

    lines = f.readlines()
    for line in lines:
        pair = line.lstrip().rstrip().split(' ')
        assert len(pair) == 2

        source = int(pair[0])
        destination = int(pair[1])

        graph[source].append(destination)
        g_inv[destination].append(source)

    return graph, g_inv