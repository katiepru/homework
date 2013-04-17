
; *********************************************
; *  314 Principles of Programming Languages  *
; *  Spring 2013                              *
; *  Student Version                          *
; *********************************************

;; contains "ctv", "A", and "reduce" definitions
(load "include.ss")

;; contains simple dictionary definition
(load "test-dictionary.ss")

;; -----------------------------------------------------
;; HELPER FUNCTIONS

;; Generate master bitvector as a list of bitvectors
(define gen_master_bitvector
  (lambda (hashfunctions dict)
    (if (pair? hashfunctions)
	  (cons 
		(map (lambda (x) ((car hashfunctions) x)) dict) 
		(gen_master_bitvector (cdr hashfunctions) dict)
      )
	  '()
	)
  )
)

;; Returns 1 if x is an element in y
(define ismemof
  (lambda (x y)
	(if (pair? y)
	  (if (= x (car y))
		#t
		(ismemof x (cdr y))
	  )
	  #f
	)
  )
)

;; Check word hashes against each set of hashes in bitvector
;; Assert size of wordhashes = size of bitvector
(define check_word
  (lambda (wordhashes bitvector)
	(if (pair? wordhashes)
  	  (if (ismemof (car wordhashes) (car bitvector))
		(check_word(cdr wordhashes) (cdr bitvector))
		#f
      )
      #t
	)
  )
)
 

;; -----------------------------------------------------
;; KEY FUNCTION

(define key
  (lambda (w)
	(foldl
	  (lambda (char int)
		(+ (* 33 int) (ctv char))
	  ) 
	  w
	  5381
	)
))


;; -----------------------------------------------------
;; HASH FUNCTION GENERATORS

;; value of parameter "size" should be a prime number
(define gen-hash-division-method
  (lambda (size) ;; range of values: 0..size-1
	(lambda (k) (modulo (key k) size))
))

;; value of parameter "size" is not critical
;; Note: hash functions may return integer values in "real"
;;       format, e.g., 17.0 for 17

(define gen-hash-multiplication-method
  (lambda (size) ;; range of values: 0..size-1
	(lambda (k) (floor (* size (- (* (key k) A) (floor (* (key k) A))))))
))


;; -----------------------------------------------------
;; EXAMPLE HASH FUNCTIONS AND HASH FUNCTION LISTS

(define hash-1 (gen-hash-division-method 701))
(define hash-2 (gen-hash-division-method 899))
(define hash-3 (gen-hash-multiplication-method 700))
(define hash-4 (gen-hash-multiplication-method 900))

(define hashfl-1 (list hash-1 hash-2 hash-3 hash-4))
(define hashfl-2 (list hash-1 hash-3))


;; -----------------------------------------------------
;; EXAMPLE HASH VALUES
;;   to test your hash function implementation
;;
;;  (hash-1 '(h e l l o)) ==> 657
;;  (hash-1 '(d a y))     ==> 46
;;  (hash-1 '(c l a s s)) ==> 183
;;
;;  (hash-2 '(h e l l o)) ==> 273
;;  (hash-2 '(d a y))     ==> 218
;;  (hash-2 '(c l a s s)) ==> 254
;;
;;  (hash-3 '(h e l l o)) ==> 595.0
;;  (hash-3 '(d a y))     ==> 546.0
;;  (hash-3 '(c l a s s)) ==> 169.0
;;
;;  (hash-4 '(h e l l o)) ==> 765.0
;;  (hash-4 '(d a y))     ==> 702.0
;;  (hash-4 '(c l a s s)) ==> 217.0


;; -----------------------------------------------------
;; SPELL CHECKER GENERATOR

(define gen-checker
  (lambda (hashfunctionlist dict)
	;;Generate bitvector
	(
	  (lambda (bitvector)
		(lambda (word)
		  (check_word 
			(map (lambda (hashfunction) (hashfunction word)) hashfunctionlist)
			bitvector
          )
        )
      )
	  (gen_master_bitvector hashfunctionlist dict)
	)
  )
)


;; -----------------------------------------------------
;; EXAMPLE SPELL CHECKERS

(define checker-1 (gen-checker hashfl-1 dictionary))
(define checker-2 (gen-checker hashfl-2 dictionary))

;; EXAMPLE APPLICATIONS OF A SPELL CHECKER
;;
;;  (checker-1 '(a r g g g)) ==> #f
;;  (checker-2 '(h e l l o)) ==> #t

