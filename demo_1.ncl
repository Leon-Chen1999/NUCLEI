(
  (SET L '(3 4)')
  (SET C '2')
  (SET N (PLUS (CAR L)  (CAR (CDR L))))
  (PRINT N)
  (WHILE (LESS C '10') (
      (SET N (PLUS (CAR L)  (CAR (CDR L))))
      (SET M (CONS N L))
      (SET L M)
      (SET B (PLUS '1' C))
      (SET C B)
  ))
  (PRINT M)
  (SET I (CONS '5' '(6 7)'))
  (PRINT I)
  (SET J (CONS '1' (CONS '2' NIL)))
  (PRINT J)
  (SET K '(1 2 3 (4 5) 6)')
  (PRINT K)
  (SET E (MINUS B '5'))
  (PRINT E)
  (IF (EQUAL '2' '3') ((PRINT "YES"))((PRINT "NO")))
  (IF (EQUAL '2' '2') ((PRINT "YES"))((PRINT "NO")))
  (IF (EQUAL (LENGTH NIL) '0')   ((PRINT "NIL HAS ZERO LENGTH")) ((PRINT "FAILURE")))
  (SET N (CONS '1' L))
  (PRINT N)
  (SET P (CONS N M))
  (PRINT P)
  (SET D (APPEND '(2 8)' '(6 7)'))
  (PRINT D)
)
