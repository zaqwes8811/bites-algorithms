# coding: utf-8

# iter vs rec:
# http://stackoverflow.com/questions/9201166/iterative-dfs-vs-recursive-dfs-and-different-elements-order
#
# Speed up:
# http://codereview.stackexchange.com/questions/29404/why-is-my-code-for-kasorajus-algorithm-so-slow
#
# Solved
# http://teacode.wordpress.com/2013/07/27/algo-week-4-graph-search-and-kosaraju-ssc-finder/

from pprint import pprint

import search_in_graph
import sys
import resource

import gr_readers

# TODO: BUGS - Iterative version is broken!!
sys.setrecursionlimit(10 ** 6)
resource.setrlimit(resource.RLIMIT_STACK, (2 ** 29, 2 ** 30))

g_t = 0
g_finals = {}


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

    global g_t
    copy_t = g_t
    while not S.empty():
        v = S.top()
        S.pop()
        for w in G[v]:
            if not explored_set[w]:
                explored_set[w] = True
                S.push(w)

        # final
        global g_finals
        g_t += 1
        g_finals[v] = g_t
    return g_t - copy_t


def dfs_separate_recursion_impl(G, SV, explored_set):
    """ """
    global g_t
    copy_t = g_t

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
    return g_t - copy_t


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

    tops = []
    for k in gr_inv.keys():
        explored_set[k] = False
    for vertex in reversed(RANGE):  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[vertex]:
            tops.append(vertex)
            dfs(rename_gr, vertex, explored_set)

    sizes = []
    for k in gr_inv.keys():
        explored_set[k] = False
    for vertex in tops:  # TODO: bad. Ключи не обязательно следуют так.
        if not explored_set[vertex]:
            sizes.append(dfs(rename_gr, vertex, explored_set))

    sizes.extend([0, 0, 0, 0, 0])
    sizes.sort(reverse=True)
    return sizes


def main():
    # get_real_graph()#
    #source_gr = get_fake_graph()
    source_gr = gr_readers.get_real_graph()
    print "Readed. Start calc"
    tops = scc(source_gr
               , dfs_iterative_impl
               #, dfs_separate_recursion_impl
               #, dfs_copy
    )
    
    gold = [434821, 968, 459, 313, 211]
    for a, b in zip(gold, tops[0:5]):
        print a, b
    assert tops[0:5] == gold


if __name__ == '__main__':
    main()
