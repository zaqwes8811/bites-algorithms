# coding: utf-8

import collections
import copy
import random

# App
#import min_cut_big_super_vertices


class Edge(object):
    def __init__(self, begin, end):
        # Исходное пложение ребра
        self.source_ = (begin, end)

        # Рабочее пложение ребра
        self.work = (begin, end)

    @property
    def begin(self):
        return self.work[0]

    @begin.setter
    def begin(self, value):
        self.work = value, self.work[1]

    @property
    def end(self):
        return self.work[1]

    @end.setter
    def end(self, value):
        self.work = self.work[0], value

    def __str__(self):
        #return str(self.work) + ":" + \
        return str(self.source_)


def print_graph(work_graph):
    for k, v in work_graph.items():
        print k,
        for edge in v:
            print edge,
        print


def print_edges(edges):
    for edge in edges:
        print edge,
    print


def get_random_edge(work_graph):
    keys = work_graph.keys()
    key = random.randint(0, len(keys) - 1)
    connections = work_graph[keys[key]]
    cut_edge = connections[random.randint(0, len(connections) - 1)]
    return cut_edge


def fuse(graph, cut_edge):
    # Узел нужно разделить, или склонировать
    cut_edge_copy = copy.deepcopy(cut_edge)

    idx_new = cut_edge_copy.begin
    idx_old = cut_edge_copy.end

    # Можно вливать узел
    # Заменяем ссылки на себя
    bands = graph[idx_old]
    for band in bands:
        # Меняем связь
        band.begin = idx_new

        # Передаем ссылки новому узлу
        graph[idx_new].append(band)

    # Пересчитываем связи и удаляем петли
    for band in bands:
        # Перенаправляем
        terminal_edges = graph[band.end]
        to_remove = []
        for edge in terminal_edges:
            if edge.end == idx_old:
                edge.end = idx_new

            if edge.begin == edge.end:
                # Похоже менять списко при итерации не стоит
                to_remove.append(edge)

        # Удаляем отдельно
        for edge in to_remove:
            terminal_edges.remove(edge)

    del graph[idx_old]  # в графе уже нет! ссылка локальна


def main():
    source_graph = {1: [2, 4], 2: [1, 4, 3], 3: [2, 4], 4: [1, 2, 3]}

    # BAD! Но это указатели, а не копии объектов. Может не так все печально?
    work_graph = collections.defaultdict(list)
    for k, v in source_graph.items():
        for value in v:
            work_graph[k].append(Edge(k, value))

    # слить два узла
    while len(work_graph) > 2:
        edge = get_random_edge(work_graph)
        fuse(work_graph, edge)

    print_graph(work_graph)


if __name__ == '__main__':
    main()