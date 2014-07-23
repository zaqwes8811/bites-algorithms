# coding: utf-8

# Weak 5

"""
Initialize:
- X = [S]  [vert. processed so far]
- A[S] = 0  [computed shortest path distances]
- B[S] = []  [path - help only]

- while X != V:
    - among all edges (v, w) in E, with v in X, w not in X
    - pick the one that minimize A[v] + l_vw  [call it (v*, w*)]
    - add w* to X
    - set A[w*] := A[v*] + l_v*w*
    - set B[w*] :=B[v*] or (v*, w*)

"""