# coding: utf-8

# iter vs rec:
# http://stackoverflow.com/questions/9201166/iterative-dfs-vs-recursive-dfs-and-different-elements-order

from pprint import pprint

import search_in_graph


g_t = 0
g_finals = {}


def dfs_copy(G, SV, visited):
    stack = [SV]
    while stack:
        vertex = stack.pop()
        if not visited[vertex]:
            visited[vertex] = True
            for w in G[vertex]:
                if not visited[w]:
                    stack.append(w)

        # final
        global g_t, g_finals
        g_t += 1
        g_finals[vertex] = g_t


def dfs_iterative_impl(G, SV, explored_set):
    class Stack(object):
        """ http://interactivepython.org/runestone/static/pythonds/BasicDS/stacks.html """
        def __init__(self):
            self.items = []

        def empty(self):
            return self.items == []

        def push(self, item):
            self.items.append(item)

        def pop(self):
            return self.items.pop()

        def top(self):
            return self.items[len(self.items) - 1]

        def size(self):
            return len(self.items)
    assert G
    assert SV
    assert explored_set

    explored_set[SV] = True
    S = Stack()
    S.push(SV)

    assert S.size() == 1

    print
    while not S.empty():
        size = S.size()
        v = S.top()
        S.pop()
        assert S.size() == size - 1
        for w in G[v]:
            if not explored_set[w]:
                explored_set[w] = True
                S.push(w)

        # final
        # TODO: bug is here!
        global g_t, g_finals
        g_t += 1
        g_finals[v] = g_t

    assert S.empty()


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


def scc(source_gr, dfs):
    assert source_gr
    assert dfs

    # Work
    RANGE = source_gr.keys()

    # inv.
    gr_inv = invert_digraph(source_gr)

    # Used DFS
    explored_set = {}
    for k, v in gr_inv.items():
        explored_set[k] = False

    for vertex in reversed(RANGE):  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[vertex]:
            dfs(gr_inv, vertex, explored_set)

    # Next step
    explored_set = {}
    rename_gr = graph_rename(source_gr, g_finals)
    for k, v in g_finals.items():
        print k, v

    tops = []
    for k in gr_inv.keys():
        explored_set[k] = False
    for vertex in reversed(RANGE):  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[vertex]:
            tops.append(vertex)
            dfs(rename_gr, vertex, explored_set)

    return tops


def main():
    # get_real_graph()#
    source_gr = get_fake_graph()
    # source_gr = get_real_graph()
    print "Readed. Start calc"
    tops = scc(source_gr
               , dfs_iterative_impl
               #, dfs_separate_recursion_impl
               #, dfs_copy
    )
    print tops


if __name__ == '__main__':
    main()
