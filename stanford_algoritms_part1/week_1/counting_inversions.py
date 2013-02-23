#-*- coding: utf-8 -*-
# Что если массив нацело не делится на два
def count_and_sort(A, n):
    if n == 1: 
        return A, 0
    else:
        B_raw = A[:n/2]
        C_raw = A[n/2:]  # может быть не равен длине первого
        B, x = count_and_sort(B_raw, len(B_raw))
        C, y = count_and_sort(C_raw, len(C_raw))
        D, z = count_and_merge(B, C, n)
        return D, x+y+z

def count_and_merge(B, C, n):
    """ 
    
    Args: 
        - Input arrays need be sorted
    """
    one_st_len = n/2
    two_st_len = n-one_st_len  # Будет либо равна первой длине, либо будет больше
    
    # Returns
    D = []
    number_inversion = 0
    
    # tmp
    i = 0
    j = 0
    for k in range(n):
        # Один из массивов закончился
        if i > one_st_len-1:
            D.append(C[j])
            j += 1
            continue
            
        if j > two_st_len-1:
            D.append(B[i])
            i += 1
            continue
        
        # Штатное сравнение
        if B[i] < C[j]:
            D.append(B[i])
            i += 1
        else :
            D.append(C[j])
            number_inversion += one_st_len-i
            print '>> ', C[j], B[i:], one_st_len-i
            j += 1
            
    return D, number_inversion

if __name__=='__main__':
    """ Одна часть рекурсия, а другая слияние? """
    print
    print 'Begin'
    
    # Модифицированное слияние
    A = [1,3,5, 2,4, 6]
    n = len(A)
    
    # fake-call
    #print count_and_merge(A[:n/2], A[n/2:], n)
    a, num = count_and_sort(A, n)
    print a, num
    
    print 'Done'

