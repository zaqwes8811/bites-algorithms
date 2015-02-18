# -*- coding: utf-8 -*-
A = [6, 5, 4, 3, 2, 1]
n = len(A)
print A
for j in range(0, n - 1):
    k = j
    minElement = A[j]
    # Ищем минимальный и его позицию
    for i in range(j + 1, n):
        print j, range(j + 1, n)
        if minElement > A[i]:
            minElement = A[i]
            k = i
    # Route
    while k > j:
        A[k] = A[k - 1]
        k -= 1
    # Сохраняем минимальный элемент
    A[j] = minElement
print A