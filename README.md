# NUCLEI
  The programming language LISP, developed in 1958, is one of the oldest languages still in common use. The langauage is famous for: being fully parenthesised (that is, every instruction is inside its own brackets), having a prefix notation (e.g. functions are written (PLUS 1 2) and not (1 PLUS 2)) and its efficent linked-list Car/Cdr structure for (de-)composing lists.
  Here, we develop a very simple langauge inspired by these concepts called NUCLEI (Neill’s UnCommon Lisp Expression Interpreter) and a means to parse or interpret the instructions.
  The interpreter (but not parser) builds on Exercise 11.4 - you’ll need to have your own version of the linked.c, lisp.h and specific.h files
  Examples
Parsing :  
(  
    (SET A ’1’)  
    (PRINT A)  
)  

leads to the output :
Parsed OK  
or with the interpreter :  
1  
The CONS instruction is used to construct lists :   
(  
(PRINT (CONS ’1’ (CONS ’2’ NIL)))  
)  
  
Parsed OK  
and when interpreted :  
(1 2)  

The CAR instruction is used to deconstruct lists : 
(  
(SET A ’(5 (1 2 3))’)  
(PRINT (CAR A))  
)  
Parsed OK   
and when interpreted :  
5  
Loops are possible too, here a loop counts down from 5 to 1, using the variable C as a counter and a Boolean test:  
(  
(SET C ’5’)  
   (WHILE (LESS ’0’ C)(  
      (PRINT C)  
      (SET A (PLUS ’-1’ C))  
(SET C A)) )  
)  
  
Parsed OK
  
and when interpreted :  
5  
4  
3  
2  
1  
The IF is similar; based on a Boolean, one of two possible sets of instructions are taken :   
(  
   (IF (EQUAL ’1’ ’1’) ((PRINT "YES"))((GARBAGE)))  
)  
Here the parser fails because it doesn’t understand GARBAGE :  
Was expecting a Function name ?  
However, the interpreter never gets to the f alse instruction since the Boolean equates to true and so :  
YES  
The Formal Grammar:  
<img width="384" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/a89a3971-2d93-49e2-87b5-d7a71161b5eb">  
<img width="455" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/8321821d-1673-4738-be94-da9314a8d834">  

<img width="383" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/d0a64876-d704-4119-b545-75690c423a82">  
<img width="381" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/f00bb7ac-0705-414b-af10-92f132249254">    
<img width="383" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/64566a6a-ac47-4200-9421-ee628c764d99">    




