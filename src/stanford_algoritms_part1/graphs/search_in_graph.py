# coding: utf-8

from pprint import pprint

from Queue import Queue  # Thread-safe - overhead

def get_fake_graph():
    return {'s': ['a', 'b'],  # s
            'a': ['s', 'c'],  # a
            'b': ['s', 'c', 'd'],  # b
            'c': ['a', 'b', 'd', 'e'],  # c
            'd': ['b', 'c', 'e'],  # d
            'e': ['c', 'd']  # e
    }


def bfs(g, start):
    pass


if __name__ == '__main__':
    class Vertex(object):
        def __init__(self, own, ends):
            self.self = own
            self.end_points = ends

            # Coupled with node
            self.explored = False

    source_graph = get_fake_graph()
    vertices = {}
    for k, v in source_graph.items():
        vertices[k] = Vertex(k, v)

    pprint(vertices)

    # Finding
    Q = Queue()

    while not Q.empty():
        pass