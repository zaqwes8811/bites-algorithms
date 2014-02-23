# coding: utf-8


def main():
    source_graph = {1: [2, 4], 2: [1, 4, 3], 3: [2, 4], 4: [1, 2, 3]}

    class Edge(object):
        def __init__(self, begin, end):
            # Исходное пложение ребра
            self.source_ = (begin, end)

            # Рабочее пложение ребра
            self.work = (begin, end)

main()