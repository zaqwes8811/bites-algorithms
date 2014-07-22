# coding: utf-8

from pprint import pprint

import search_in_graph


def dfs_expensive_impl(G, SV, explored_set):
    """ """
    def __dfs(vertex):
        explored_set[vertex] = True
        ends = G[vertex]
        is_end_vertex = True
        for w in ends:
            if not explored_set[w]:
                is_end_vertex = False
                __dfs(w)

        if is_end_vertex:
            # Terminal vertex
            print vertex

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


def invert_digraph(g):
    copy_gr = {}

    for k, v in g.items():
        copy_gr[k] = []

    for k, v in g.items():
        ends = g[k]
        for elem in ends:
            copy_gr[elem].append(k)

    return copy_gr


def main():
    source_gr = get_fake_graph()
    gr_inv = invert_digraph(source_gr)

    dfs = dfs_expensive_impl

    #
    explored_set = {}
    if not explored_set:
        for k, v in gr_inv.items():
            explored_set[k] = False

    dfs(gr_inv, 9, explored_set)


if __name__ == '__main__':
    main()
