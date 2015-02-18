

def benchmark_delete_key():
    """
    http://docs.python.org/2/library/collections.html

    """

    import timeit

    code = """
    results = {'A': 1, 'B': 2, 'C': 3}
    del results['A']
    del results['B']
    """
    print timeit.timeit(code, number=100000)
    code = """
    results = {'A': 1, 'B': 2, 'C': 3}
    results.pop('A')
    results.pop('B')
    """
    print timeit.timeit(code, number=100000)

    code = """
    results = {'A': 1, 'B': 2, 'C': 3}
    def remove_key(d, key):
        r = dict(d)
        del r[key]
        return r
    remove_key(results, 'A')
    remove_key(results, 'B')
    """
    print timeit.timeit(code, number=100000)

    code = """
    #import collections
    for i in range(100000):
        #results = collections.defaultdict({'A': 1, 'B': 2, 'C': 3})
        #del results['A']
        #del results['B']
        pass
    """
    print timeit.timeit(code, number=10)