

def enc_digraph_to_select_in_edges(g):
    """
      Change selection out edges to select in edges

      @param g - Graph in Adj. List. Vertex correspond out edges
    """
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
