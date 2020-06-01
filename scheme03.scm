; Foley 211926


;Finite Difference
(define (fd-at f x) (- (f (+ x 1)) (f x)))
(define (f x) (* x x))
;(fd-at f 3)


;Winner of a list
;(define (nearer-10? x y) (< (abs (- x 10)) (abs (- y 10))))
(define (winner beats? L)
  (if (null? (cdr L)) (car L)
      (if (beats? (car L) (car (cdr L))) (winner beats? (cons (car L) (cddr L))) (winner beats? (cdr L)))))

;(winner nearer-10? '(14 8 -1 22 13 15 7 9 15))
;(winner <  '(14 8 -1 22 13 15 7 9 15))


;Map/Apply Probs
(define (prod-root L) (apply * (map sqrt L)))
;(prod-root '(1 4 9))

(define (len L) (+ (* 12 (car L)) (cadr L)))
(define (sum-lengths L) (apply + (map len L)))

;(sum-lengths '((3 8) (1 11)))


;SSQD
(define (ssqd L u) (apply + (map (lambda (x) (expt (- x u) 2)) L)))
;(ssqd '(3 8 2 6) 4)


;DISTANCE
(define (dist p1 p2) (sqrt (apply + (map (lambda (x y) (expt (- x y) 2)) p1 p2))))
;(dist '(1 1 3 0) '(5 4 0 2))

