; Foley 211926

;(+ (* 4.7 (- 34.453 47.728)) 3.7)

;(max (sqrt 5) (+ (sin 1) (sin 2) (sin 3)) (/ 17.0 8.0))

;(+ (- (* 2 (expt 2.451 3)) (expt 2.451 2)) (- (* 3 2.451) 5))

;(+ (* 2 2.451 2.451 2.451) (* -1 2.451 2.451) (* 3 2.451) -5)

(define (to-celsius x) (* (/ 5 9.0) (- x 32)))
;(to-celcius 70)
;(to-celcius 32)
(define (to-fahrenheit x) (+ (* x (/ 9 5.0)) 32))
;(to-fahrenheit 21)
(define (my-dist x y) (sqrt(+ (* x x) (* y y))))
;(my-dist 3 4)
(define (f x) (+ (* (sin x) (sin x)) (* x (cos x))))
(define (fprime x) (+ (- (cos x) (* x (sin x))) (* 2 (cos x) (sin x))))
;(f 1)
;(fprime 1)
(define (sign-inc x) (if (= x 0) x (if (< x 0) (- x 1) (+ x 1))))
;(sign-inc 2)
;(sign-inc -2)
;(sign-inc 0)
(define (middle x y z)
   (cond ((or (and (>= x y) (<= x z)) (and (>= x z) (<= x y))) x) 
         ((or (and (>= y x) (<= y z)) (and (>= y z) (<= y x))) y)
         ((or (and (>= z x) (<= z y)) (and (>= z y) (<= z x))) z)
   )
)

;(middle -17 9 2)

(define (factorial x) (if (or (= x 1) (= x 0)) 1 (* x (factorial (- x 1)))))
;(factorial 4)
(define (cm B r) (* B (+ 1 (/ (/ r 100.0) 12.0))))
;(cm 1024.5 0.05)
(define (am B r m) (if (= m 1) (cm B r) (cm (am B r (- m 1)) r)))
;(am 1027.5 0.05 36)
(define (accrue B r y) (am B r (* 12 y)))
;(accrue 1027.5 0.05 3)
(define (fib x) (if (or (= x 1) (= x 0)) 1 (+ (fib (- x 1)) (fib (- x 2)))))
;(fib 2)
;(fib 3)
;(fib 4)
(define (foo x) (let ((a (+ 1 (* (sin x) (sin x))))) (- (+ (/ 1 a) (sqrt a)) (* a a))))
;(foo 5.1)
(define (diff f1 i1 f2 i2) (let ((L1 (+ i1 (* 12 f1))) (L2 (+ i2 (* 12 f2)))) (- L1 L2)))
;(diff 3 7 2 11)
;(display (middle 8 3 7)) (newline)