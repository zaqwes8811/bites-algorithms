# coding: utf-8

import collections
import copy


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

    def __str__(self):
        return str(self.work)


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


def fuse(work_graph, edge):
    # Узел нужно разделить, или склонировать
    work_edge = copy.deepcopy(edge)

    # Можно вливать узел
    # Заменяем ссылки на себя
    end_bands = work_graph[work_edge.end]
    for band in end_bands:
        # Передаем ссылки новому узлу
        band.begin = work_edge.begin
        work_graph[work_edge.begin].append(band)

    # Пересчитываем связи и удаляем петли
    for band in end_bands:
        # Перенаправляем
        edges = work_graph[band.end]
        for edge in edges:
            if edge.end == work_edge.end:
                edge.work = (edge.begin, work_edge.begin)

            if edge.begin == edge.end:
                edges.remove(edge)

    del work_graph[work_edge.end]  # в графе уже нет! ссылка локальна


def main():
    source_graph = {1: [2, 4], 2: [1, 4, 3], 3: [2, 4], 4: [1, 2, 3]}

    # BAD! Но это указатели, а не копии объектов. Может не так все печально?
    work_graph = collections.defaultdict(list)
    for k, v in source_graph.items():
        for value in v:
            work_graph[k].append(Edge(k, value))

    # слить два узла
    print_graph(work_graph)
    print 'fusing'

    edge = work_graph[1][0]

    fuse(work_graph, edge)
    print 'rest'
    print_graph(work_graph)


if __name__ == '__main__':
    main()