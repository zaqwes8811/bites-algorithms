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

    return source_v, end_v


def main():
    """
    Полагаем что доступ по ключу O(n)
    """
    source_graph = {1: [2, 4], 2: [1, 4, 3], 3: [2, 4], 4: [1, 2, 3]}

    # One iteration
    source, end = get_random_edge(source_graph)


main()
