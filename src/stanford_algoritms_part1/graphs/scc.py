# coding: utf-8

from pprint import pprint

import search_in_graph


g_t = 0
g_finals = {}


def dfs_separate_impl(G, SV, explored_set):
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


def invert_digraph(g):
    copy_gr = {}

    for k, v in g.items():
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


def scc(G, MAX_KEY_VAL):
    dfs = dfs_separate_impl

    # First pass - Inv. Gr.
    gr_inv = invert_digraph(G)
    explored_set = {}
    for k, v in gr_inv.items():
        explored_set[k] = False

    for i in reversed(range(1, MAX_KEY_VAL)):  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[i]:
            dfs(gr_inv, i, explored_set)

    # Second pass - ?
    explored_set = {}
    rename_gr = graph_rename(G, g_finals)
    for k, v in gr_inv.items():
        explored_set[k] = False

    tops = []
    for i in reversed(range(1, MAX_KEY_VAL)):  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[i]:
            tops.append(i)
            dfs(rename_gr, i, explored_set)

    return tops


def main():
    source_gr = get_fake_graph()
    tops = scc(source_gr, 10)
    print tops


if __name__ == '__main__':
    main()
