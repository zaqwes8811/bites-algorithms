; fixme: difference arithmetic?

(set-logic QF_IDL) ; optional in Z3
(declare-fun t11 () Int)
(declare-fun t12 () Int)
(declare-fun t21 () Int)
(declare-fun t22 () Int)
(declare-fun t31 () Int)
(declare-fun t32 () Int)
(assert (and (>= t11 0) (>= t12 (+ t11 2)) (<= (+ t12 1) 8)))
(assert (and (>= t21 0) (>= t22 (+ t21 3)) (<= (+ t22 1) 8)))
(assert (and (>= t31 0) (>= t32 (+ t31 2)) (<= (+ t32 3) 8)))
(assert (or (>= t11 (+ t21 3)) (>= t21 (+ t11 2))))
(assert (or (>= t11 (+ t31 2)) (>= t31 (+ t11 2))))
(assert (or (>= t21 (+ t31 2)) (>= t31 (+ t21 3))))
(assert (or (>= t12 (+ t22 1)) (>= t22 (+ t12 1))))
(assert (or (>= t12 (+ t32 3)) (>= t32 (+ t12 1))))
(assert (or (>= t22 (+ t32 3)) (>= t32 (+ t22 1))))
(check-sat)
(get-model)  ; display the model