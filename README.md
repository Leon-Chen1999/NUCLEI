# NUCLEI
  The programming language LISP, developed in 1958, is one of the oldest languages still in common use. The langauage is famous for: being fully parenthesised (that is, every instruction is inside its own brackets), having a prefix notation (e.g. functions are written (PLUS 1 2) and not (1 PLUS 2)) and its efficent linked-list Car/Cdr structure for (de-)composing lists.
  Here, we develop a very simple langauge inspired by these concepts called NUCLEI (Neill’s UnCommon Lisp Expression Interpreter) and a means to parse or interpret the instructions.
  The interpreter (but not parser) builds on Exercise 11.4 - you’ll need to have your own version of the linked.c, lisp.h and specific.h files
  Examples
<img width="416" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/5094ef31-705f-4a2b-8350-61176dfcea7d">  


Loops are possible too, here a loop counts down from 5 to 1, using the variable C as a counter and a Boolean test:  
<img width="408" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/675376cd-e99d-4e30-8d3d-53891d8ba59d">  
 
The IF is similar; based on a Boolean, one of two possible sets of instructions are taken :   
<img width="404" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/fa2231d2-34f2-4cf7-8de5-d5e0e7b6f167">  

The Formal Grammar:  
<img width="384" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/a89a3971-2d93-49e2-87b5-d7a71161b5eb">   
<img width="455" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/8321821d-1673-4738-be94-da9314a8d834">    
<img width="383" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/d0a64876-d704-4119-b545-75690c423a82">    
<img width="389" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/ddf694d2-2806-4311-b772-c0c6996d7c4a">   
<img width="383" alt="image" src="https://github.com/Leon-Chen1999/NUCLEI/assets/122807406/64566a6a-ac47-4200-9421-ee628c764d99">     




