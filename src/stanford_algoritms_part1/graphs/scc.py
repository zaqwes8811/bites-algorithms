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
import util

# TODO: BUGS - Iterative version is broken!!
sys.setrecursionlimit(10 ** 6)
resource.setrlimit(resource.RLIMIT_STACK, (2 ** 29, 2 ** 30))


class TraverseInfo(object):
    def __init__(self):
        self.g_t = 0
        self.g_finals = {}
        self.explored = set()


def dfs_iterative_impl(G, SV, tr):
    assert G
    assert SV
    assert tr

    tr.explored.add(SV)
    S = util.Stack()
    S.push(SV)

    copy_t = tr.g_t
    while not S.empty():
        v = S.top()
        S.pop()
        for w in G[v]:
            if w not in tr.explored:
                tr.explored.add(w)
                S.push(w)

        # final
        # TODO: BUG is here somewhere
        tr.g_t += 1
        tr.g_finals[v] = tr.g_t
    return tr.g_t - copy_t


def dfs_separate_recursion_impl(G, SV, tr):
    def __dfs(vertex):
        tr.explored.add(vertex)
        ends = G[vertex]
        for w in ends:
            if w not in tr.explored:
                __dfs(w)

        tr.g_t += 1
        tr.g_finals[vertex] = tr.g_t

    assert G
    assert SV
    assert tr

    copy_t = tr.g_t
    __dfs(SV)
    return tr.g_t - copy_t


def scc(source_gr, dfs, t):
    assert source_gr
    assert dfs
    assert not t.explored
    assert t.g_t == 0

    # Work
    RANGE = source_gr.keys()

    # inv.
    gr_inv = util.invert_digraph(source_gr)

    # Used DFS
    for vertex in reversed(RANGE):
        if vertex not in t.explored:
            dfs(gr_inv, vertex, t)

    # Next step
    t.explored = set()
    rename_gr = util.graph_rename(source_gr, t.g_finals)

    tops = []
    t.explored = set()
    for vertex in reversed(RANGE):
        if vertex not in t.explored:
            tops.append(vertex)
            dfs(rename_gr, vertex, t)

    sizes = []
    t.explored = set()
    for vertex in tops:
        if vertex not in t.explored:
            sizes.append(dfs(rename_gr, vertex, t))

    sizes.extend([0, 0, 0, 0, 0])
    sizes.sort(reverse=True)
    return sizes


def main():
    source_gr = gr_readers.get_fake_graph()
    source_gr = gr_readers.get_real_graph()

    t = TraverseInfo()

    tops = scc(source_gr
               #, dfs_iterative_impl
               , dfs_separate_recursion_impl
               #, dfs_copy
               , t
    )
    
    gold = [434821, 968, 459, 313, 211]
    #gold = [3, 3, 3, 0, 0]
    assert tops[0:5] == gold


if __name__ == '__main__':
    main()
