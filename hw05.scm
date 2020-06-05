;(define (foo L) (let* ((P (car L)) (x (car P)) (y (cadr P))) (+ x y)))
(define (foo L) (let ((P (car L))) (let ((x (car P))) (let ((y (cadr P))) (+ x y)))))

(define kons (lambda (x y) (lambda (w) (if w x y))))
(define kar (lambda (L) (L #t)))
(define kdr (lambda (L) (L #f)))
(define knull (letrec ((F (lambda (x) F))) F))

(define (kadd L) (if (equal? L knull) 0 (+ (kar L) (kadd (kdr L)))))


(define (diet-scheme P)
  (if (list? P)
      (if (not (equal? (car P) 'let))
          (map diet-scheme P)
          (let ((name (caaadr P)) (res (diet-scheme (caddr P))) (val (diet-scheme (car (cdaadr P))))) (list (list 'lambda (list name) res) val))  
      )
      P)
  )


;(define prog1 '(let ((x (sqrt 3.14159))) (+ x (/ 1 x))))
;(define prog2 '(let ((x (sqrt 3.14159))) (let ((y (/ 2.7128 x))) (expt (+ x y) 3))))

;(list (caaadr args) (caddr args) (cdaadr P))
;(list (cons 'lambda (append (list (caaar args)) (list (cadr args))))  (cadaar P))
;(let ((name (caaadr args)) (res (caddr args)) (val (car (cdaadr args)))) (list (list 'lambda (list name) (list res)) val))
;(let ((name (caaadr P)) (res (caddr P)) (val (car (cdaadr P)))) (list (list 'lambda (list name) res) val))