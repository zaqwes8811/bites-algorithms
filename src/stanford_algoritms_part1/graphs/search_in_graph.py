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
            self.ends = ends

            # Coupled with node
            self.explored = False

        def __str__(self):
            return "self: %s, ex: %s" % (self.self, self.explored)

    source_graph = get_fake_graph()
    vertices = {}
    for k, v in source_graph.items():
        vertices[k] = Vertex(k, v)

    # Finding
    Q = Queue()
    start = vertices['s']

    # Mark
    start.explored = True
    Q.put(start)

    assert Q.qsize() == 1

    while not Q.empty():
        size = Q.qsize()
        v = Q.get()
        print v
        assert Q.qsize() == size - 1
        for w in v.ends:
            node = vertices[w]
            if not node.explored:
                node.explored = True
                Q.put(node)

    assert Q.empty()
