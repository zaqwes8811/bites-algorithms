# coding: utf-8

# Info:
#  http://eddmann.com/posts/depth-first-search-and-breadth-first-search-in-python/

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


# TODO: api is bad
def bfs(g, start):
    assert start
    assert g

    # Prepare data
    # TODO: BAD: дополнительная структура данных
    graph_store = {}
    for self, raw_ends in g.items():
        graph_store[self] = Vertex(self, raw_ends)

    for no_used, vertex in graph_store.items():
        ref_ends = []
        for elem in vertex.ends:
            ref_ends.append(graph_store[elem])
        vertex.ends = ref_ends

    # Finding
    start = graph_store[start]

    # Mark
    start.explored = True
    Q = Queue()
    Q.put(start)

    assert Q.qsize() == 1

    while not Q.empty():
        size = Q.qsize()
        v = Q.get()
        print v  # data extracting
        assert Q.qsize() == size - 1
        for w in v.ends:
            if not w.explored:
                w.explored = True
                Q.put(w)

    assert Q.empty()


class Vertex(object):
    def __init__(self, own, ends):
        self.self = own
        self.ends = ends

        # Coupled with node
        self.explored = False

    def __str__(self):
        return "self: %s, ex: %s" % (self.self, self.explored)


if __name__ == '__main__':
    graph = get_fake_graph()
    bfs(graph, 's')


