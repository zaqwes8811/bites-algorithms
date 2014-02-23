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

    @end.setter
    def end(self, value):
        self.work = self.work[0], value

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
        edges = graph[band.end]
        for edge in edges:
            if edge.end == idx_old:
                edge.end = idx_new

            if edge.begin == edge.end:
                edges.remove(edge)  # не удаляет все

    del graph[idx_old]  # в графе уже нет! ссылка локальна


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

    edge = work_graph[2][2]#work_graph[1][0]

    fuse(work_graph, edge)
    print 'rest'
    print_graph(work_graph)


if __name__ == '__main__':
    main()