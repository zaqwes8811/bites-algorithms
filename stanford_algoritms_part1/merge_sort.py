#-*- coding: utf-8 -*-
def _one_recursive_step(a_list, b_list):
    """ """
    c_list = []
    
    return c_list
    
def _merge(in_list, size_task):
    print 'New call...'
    # Нужно разбить на пары
    size_in_list = len(in_list)
    ab_list = []
    ptr = 0
    current_size_task = size_task*2
    while ptr < size_in_list:
        print in_list[ptr:ptr+current_size_task]
        ptr += current_size_task
        
    # Снова вызываем рекурсивный оборот
    new_size_task = size_task*2
    if current_size_task*2 < size_in_list:
        _merge(in_list, new_size_task)

if __name__=='__main__':
    """ Одна часть рекурсия, а другая слияние? """
    # length = n = 2^3
    in_list = [5, 4, 1, 8, 7, 2, 6, 3]
    size_task = 1  # размерность задачи
    _merge(in_list, size_task)
    

# Тест рекурсии - вывод левого поддерева
#if list[:len(list)/2]:
    #_merge(list[:len(list)/2])