; Foley 211926


(define (round100 x) (/ (round (* x 100)) 100.0))

(define (toUSD x from)
  (if (equal? from 'euro) (* x 1.23510)
      (if (equal? from 'ukpound) (* x 1.78250)
          (if (equal? from 'yen) (* x 0.009321)
              (if (equal? from 'usdollar) x 'error)))))

(define (convert x from to)
  (let ((d (toUSD x from)))
    (if(equal? to 'usdollar) d
       (if(equal? to 'ukpound) (* d 0.56101)
          (if(equal? to 'euro) (* d 0.80965)
             (if(equal? to 'yen) (* d 107.290) 'error))))))


;(round100 (convert 3.50 'usdollar 'euro))

;(round100 (convert 25.00 'yen 'ukpound))

(define (s-tip x p) (* x (+ 1 (* p 0.01))))

(define (tip x) (cons (s-tip x 15) (cons (s-tip x 20) '())))

;(tip 30)

(define (squares i j) (if (= i j) (cons (* i i) '()) (cons (* i i) (squares (+ i 1) j))))

;(squares 2 12)

(define (tip-range x p1 p2) (if (= p1 p2) (cons (s-tip x p2) '()) (cons (s-tip x p1) (tip-range x (+ 1 p1) p2))))

;(tip-range 34.50 10 25)

(define (sum-abs L) (if (equal? L '()) 0 (+ (if (< (car L) 0) (* -1 (car L)) (car L)) (sum-abs (cdr L)))))

;(sum-abs '(-3 2 11 -5))

(define L '( (a x 2) (("the") b c) (z 12)))

;(caar L)
;(caaadr L)
;(cadaddr L)
;(car (cdaddr L))

(define P '(34 18 25 23 29 11 28 24 27 29))
(define (sum-cash L) (if (equal? L '()) 0 (+ (toUSD (caar L) (cadar L)) (sum-cash (cdr L)))))
;(sum-cash '((12.20 usdollar) (340 yen) (8.50 euro)))

(define (len L) (if (equal? L '()) 0 (+ 1.0 (length (cdr L)))))
;(len P)
(define (sum L) (if (equal? L '()) 0.0 (+ (car L) (sum (cdr L)))))
;(sum P)
(define (avg L) (/ (sum L) (len L)))
;(avg P)
(define (std-dev-n L a) (if (equal? L '()) 0 (+ (expt (- (car L) a) 2) (std-dev-n (cdr L) a))))
(define (std-dev L) (sqrt (/ (std-dev-n L (avg L)) (- (len L) 1))))
;(std-dev P)
