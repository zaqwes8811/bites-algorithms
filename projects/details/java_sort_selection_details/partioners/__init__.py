# coding: utf-8


class Partitioner(object):
    @staticmethod
    def partition(array):
        """
             Берем певый в качастве Pivot
        """
        IDX_PIVOT = 0  # Возможно лучше выбрать последний
        # тогда будет проще работать с индексами
        pivot_value = array[IDX_PIVOT]
        i = 1
        for j, value in enumerate(array):
            # j сканирет весь, но первый элемент пропускаем
            if j == 0:
                continue

            if pivot_value > array[j]:
                swap(array, j, i)
                i += 1

        # Last swap
        swap(array, IDX_PIVOT, i-1)
        return i  # Текущий номер pivot


def swap(array, i, j):
    a, b = i, j
    array[b], array[a] = array[a], array[b]
