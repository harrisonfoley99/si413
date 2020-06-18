;Foley 211926
;scheme05

(define (nps-tr i j acc) (if (> i j) acc (nps-tr (+ 1 i) j (if (> (sin i) 0) (+ 1 acc) acc))))
(define (nps i j) (nps-tr i j 0))

(define (split-sum-tr L res) (if (null? L) res (split-sum-tr (cddr L) (list (+ (car L) (car res)) (+ (cadr L) (cadr res))))))
(define (split-sum L) (split-sum-tr L '(0 0)))
;(split-sum '(1 2 3 4))

(define (min-sin-r L val) (if (null? L) val (if (< (sin (car L)) (sin val)) (min-sin-r (cdr L) (car L)) (min-sin-r (cdr L) val))))
(define (min-sin x . vals) (min-sin-r vals x))
;(min-sin 1.1 2.3 4.5 6.7)

(define (difs-r L) (if (null? (cdr L)) '() (cons (- (cadr L) (car L)) (difs-r (cdr L)))))
(define (difs x . vals) (difs-r (cons x vals)))


(define (create-stack . params)
 (let ((size (if (null? params) 0 (cadr params))) (stack (if (null? params) '() (car params))) (out 0))
    (lambda (message . data)
      (cond
        ((equal? message 'size) size)
        ((equal? message 'push) (set! size (+ size (length data))) (set! stack (append data stack)))
        ((equal? message 'pop) (set! out (if (null? stack) 'error (car stack))) (set! stack (if (null? stack) '() (cdr stack))) (set! size (if (> size 0) (- size 1) 0)) out)
        ((equal? message 'clone) (let ((S (create-stack stack size))) S))
        (#t 'error)))))
