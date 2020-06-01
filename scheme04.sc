;Foley 211926
;scheme exercise 4

;MAX SIN

;recursive
(define (max-sin-r L)
  (if (equal? (cdr L) '()) (sin (car L))
      (if (> (sin (car L)) (max-sin-r (cdr L))) (max-sin-r (cons (car L) (cddr L))) (max-sin-r (cdr L)))))
;(max-sin-r '(3 4 7 8 10 12))

;map/apply
(define (max-sin-f L)
  (apply max (map sin L)))
;(max-sin-r '(3 4 7 8 10 12))

;arg-max-sin
(define (arg-max-sin L)
  (if (equal? (cdr L) '()) (car L)
      (if (> (sin (car L)) (sin (arg-max-sin (cdr L)))) (arg-max-sin (cons (car L) (cddr L)))(arg-max-sin (cdr L)))))
;(arg-max-sin '(2.5 2.0 1.5 1.0))


;cross
(define (pw x L)
  (map (lambda (y) (cons x (list y))) L))
;(pw 4 '(a b c))
(define (cross L1 L2)
  (map (lambda (y) (pw y L2)) L1))
;(cross '(1 2 3) '(a b))


;filter in/out
(define (filter-out reject? L)
  (if (equal? L '()) '()
      (if (reject? (car L)) (filter-out reject? (cdr L)) (cons (car L) (filter-out reject? (cdr L))))))
;(filter-out even? '(10 9 8 7 6 5 4 3 2 1))

(define (filter-in keep? L)
  (if (equal? L '()) '()
      (if (keep? (car L)) (cons (car L) (filter-in keep? (cdr L))) (filter-in keep? (cdr L)))))
;(filter-in even? '(10 9 8 7 6 5 4 3 2 1))


;comes before
(define (comes-before? a b L)
  (> (length (member a L)) (length (member b L))))
;(comes-before? 'mon 'wed '(sun mon tue wed thu fri sat))
;(comes-before? 'fri 'wed '(sun mon tue wed thu fri sat))