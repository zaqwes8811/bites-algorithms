# coding: utf-8

import copy
import random


def get_random_edge(source_graph):
    """
    Keys is positive integer.
    """

    keys = source_graph.keys()

    source_v = random.randint(1, len(keys))

    connections = source_graph[source_v]
    end_v = connections[random.randint(0, len(connections) - 1)]
    print end_v, connections, source_graph

    return source_v, end_v


def main():
    """
    Полагаем что доступ по ключу O(n)

    Как сохранить информацию о ребре, а не просто их схлопнуть?

    Использовать стопки, но это дополнительное место
    """
    source_graph = {1: [2, 4], 2: [1, 4, 3], 3: [2, 4], 4: [1, 2, 3]}

    # One iteration
    while source_graph.__len__() > 2:
        edge = get_random_edge(source_graph)
        fuse(source_graph, edge)

    print source_graph


def fuse(graph, edge):
    source = edge[0]
    end = edge[1]

    # source <- end
    source_vs = graph[source]
    end_vs = graph[end]

    # Дублированные удалять нельзя, но нужно удалять петли
    source_vs.extend(end_vs)

    graph[source] = filter(lambda a: a != source, graph[source])

    del graph[end]  # Ссылки содержатся в списках


def remove_key(d, key):
    r = dict(d)
    del r[key]
    return r


def benchmark_delete_key():
    """
    http://docs.python.org/2/library/collections.html

    """

    import timeit

    code = """
    results = {'A': 1, 'B': 2, 'C': 3}
    del results['A']
    del results['B']
    """
    print timeit.timeit(code, number=100000)
    code = """
    results = {'A': 1, 'B': 2, 'C': 3}
    results.pop('A')
    results.pop('B')
    """
    print timeit.timeit(code, number=100000)

    code = """
    results = {'A': 1, 'B': 2, 'C': 3}
    def remove_key(d, key):
        r = dict(d)
        del r[key]
        return r
    remove_key(results, 'A')
    remove_key(results, 'B')
    """
    print timeit.timeit(code, number=100000)

    code = """
    #import collections
    for i in range(100000):
        #results = collections.defaultdict({'A': 1, 'B': 2, 'C': 3})
        #del results['A']
        #del results['B']
        pass
    """
    print timeit.timeit(code, number=10)


main()
#benchmark_delete_key()
