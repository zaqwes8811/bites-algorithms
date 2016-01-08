#!/usr/bin/env python
# coding: utf-8

# eval:
#   http://docs.sympy.org/dev/modules/numeric-computation.html

# Scilab for control eng.:
#   http://bsiswoyo.lecture.ub.ac.id/files/2012/01/SG.pdf
#   limit() - предел
#
# fixme: не нашел z-преобразования
#   http://comments.gmane.org/gmane.comp.python.sympy/18287
#
#   там только непрерывные преобразования - to scipy.signal
#
# CAS != numerical calc package

# Хотелось бы:
#   - Вывод данных из CAS
#   - Выражение одниж переменных через другие
#   - Решение неравенств
#   - Упрощение выражений

from sympy.integrals import laplace_transform as laplace
from sympy.abc import t, s, a
from sympy import exp, symbols, Eq
from sympy import *

import numpy as np
import matplotlib.pyplot as plt

def main():
    # laplace
    print laplace(exp(-t), t, s)

    # fixme: find Bode?

if __name__ == '__main__':
    main()

    x, y, z = symbols('x y z')

    print Eq(x + 1, 4)
    
    # fixme: in interactive mode?
    init_printing(use_unicode=True)
    pprint(Integral(sqrt(1/x), x))

    # Simplifications
    pprint(expand((x+1)**2))
    pprint(factor( expand((x+1)**2)) )

    expr = x*y + x - 3 + 2*x**2 - z*x**2 + x**3
    pprint(collect(expr, x))

    # Calculus
    print cos(x).diff(x, x)
    print limit(sin(x)/x, x, 0)

    # Inequality Solvers
    # http://docs.sympy.org/dev/modules/solvers/inequalities.html



