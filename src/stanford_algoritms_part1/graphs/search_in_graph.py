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


def shortest_path_app(g):
    raise Exception('Not impl.')

# TODO: api is bad
def bfs_bad_impl(g, start):
    assert start
    assert g

    class Vertex(object):
        """
        About:
        
        TODO: To expensive impl. """
        def __init__(self, own, ends):
            self.self = own
            self.ends = ends

            # Coupled with node
            self.explored = False
            self.distance = 0

        def __str__(self):
            return "self: %s, ex: %s" % (self.self, self.explored)

        @staticmethod
        def recode_graph(g):
            # Prepare data
            # TODO: BAD: дополнительная структура данных и увеличиваю константы в сложности
            # TODO: но! в обхода нет поиска по ключу, но такой же поиск при преобразовании
            store = {}
            for self, raw_ends in g.items():
                store[self] = Vertex(self, raw_ends)

            for no_used, vertex in store.items():
                ref_ends = []
                for elem in vertex.ends:
                    ref_ends.append(store[elem])
                vertex.ends = ref_ends
            return store

    # Finding
    graph_store = Vertex.recode_graph(g)
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


if __name__ == '__main__':
    graph = get_fake_graph()
    bfs_bad_impl(graph, 's')


