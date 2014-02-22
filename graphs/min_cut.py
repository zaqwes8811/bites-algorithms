# coding: utf-8

import copy
import random


def get_random_edge(find_edge_graph):
    """
    Not None in list!
    """
    source_v = random.randint(0, len(find_edge_graph)-1)
    connections = find_edge_graph[source_v]
    end_v = connections[random.randint(0, len(connections)-1)]
    return source_v+1, end_v


def main():
    """
    Чтобы извлекать за константное время нужно оставлять ссылки на местах.
    Чтобы рандомно выбирать за конастантное время нужно ужимать список
    """
    source_graph = {1:[2, 4], 2:[1, 4, 3], 3:[2, 4], 4:[1, 2, 3]}

    for i in range(19):
        print get_random_edge(source_graph)



main()
