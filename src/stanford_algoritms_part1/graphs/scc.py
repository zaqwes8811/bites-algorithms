# coding: utf-8

from pprint import pprint

import search_in_graph


g_t = 0
g_finals = {}


def dfs_separate_recursion_impl(G, SV, explored_set):
    """ """

    def __dfs(vertex):
        explored_set[vertex] = True
        ends = G[vertex]
        for w in ends:
            if not explored_set[w]:
                __dfs(w)

        global g_t, g_finals
        g_t += 1
        g_finals[vertex] = g_t

    assert G
    assert SV
    assert explored_set

    __dfs(SV)


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
    for i in range(1, 875714+1):
        graph[i] = []

    lines = f.readlines()
    for line in lines:
        pair = line.lstrip().rstrip().split(' ')
        assert len(pair) == 2
        source = int(pair[0])
        destination = int(pair[1])
        graph[source].append(destination)

    return graph


def invert_digraph(g):
    copy_gr = {}

    for k in g.keys():
        copy_gr[k] = []

    for k, v in g.items():
        ends = g[k]
        for elem in ends:
            copy_gr[elem].append(k)

    return copy_gr


def graph_rename(G, recoder):
    gr_copy = {}
    for k, v in G.items():
        gr_copy[recoder[k]] = []
        for elem in v:
            gr_copy[recoder[k]].append(recoder[elem])

    return gr_copy


def scc(source_gr):
    assert source_gr
    assert dfs_separate_recursion_impl

    # Work
    dfs = dfs_separate_recursion_impl
    RANGE = source_gr.keys()

    print "First pass - Inv. Gr."
    print "Inversion..."
    gr_inv = invert_digraph(source_gr)

    explored_set = {}
    for k, v in gr_inv.items():
        explored_set[k] = False

    for i in reversed(RANGE):  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[i]:
            dfs(gr_inv, i, explored_set)

    print "Second pass"
    explored_set = {}
    rename_gr = graph_rename(source_gr, g_finals)
    for k, v in gr_inv.items():
        explored_set[k] = False

    tops = []
    for i in reversed(RANGE):  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[i]:
            tops.append(i)
            dfs(rename_gr, i, explored_set)

    return tops


def main():
    # get_real_graph()#
    source_gr = get_fake_graph()
    #source_gr = get_real_graph()
    print "Readed. Start calc"
    tops = scc(source_gr)
    print tops


if __name__ == '__main__':
    main()
