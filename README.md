# OurScheme Interpreter
## Enviroment
   Dev-C++ IDE
   Compiler: TDM-GCC 4.9.2
## Description
中原大學三下PL(程式語言)project 占分70% 分為 project1 to 4  目的為訓練學生完成大程式的整合以及模組化思考
   - **Project 1 :** [OurSchemeProj1-UTF-8.txt](doc/OurSchemeProj1-UTF-8.txt) \
     Complete Scanner layer, Parser layer, Printer, and basic error handling
   - **Project 2 :** [OurSchemeProj2-UTF-8.txt](doc/OurSchemeProj2-UTF-8.txt)\
     Evaluate "primitive expressions" \
     'define'(expect 'yet' func) \
     Conditional processing ('if'\ and 'cond') \
     Sequencing (functional composition and the use of 'begin') 
   - **Project 3 :** [OurSchemeProj3-UTF-8.txt](doc/OurSchemeProj3-UTF-8.txt)\
     consists of three special “forms”: 'let', 'lambda', and 'define’
   - **Project 4 :** [OurSchemeProj4-UTF-8.txt](doc/OurSchemeProj4-UTF-8.txt)\
     function calls( create-error-object, error-object?,
     read, write, display-string, newline, eval, and set!)

     **Note:** This Project is only completed to Project3 and part of  Project4
## Status
Check details from [OurSchemeIntro](doc/OurSchemeIntro.doc)
   - Constructors
     A list (or a dotted pair)
   - Bypass the default interpretation of an S-exp
   - Binding a symbolic name to an S-exp
   - Default parameters
   - List Primitives
     'car', 'cdr'
   - Primitive predicates
   - Basic arithmetic, logical and string operations
   - Equality comparisons and sameness
   - Conditionals
   - Sequencing & functional composition
   - DEFINE revisited
   - Local variables
   - Lexical error、syntactic error、 semantic error
   - Clean-environment
   
   Does not support :

   1. Target functions:
      - Change of (local) symbol bindings ("assignment")
        'set'
      - Input and output
        'read', 'write'
      - Load files
      - Eval
   3. ERROR object
## Usage
An interactive system running in DOS， It works very similarly to Petite Chez Scheme
   1. The test data starts with an integer (gTestNum) for debugging
   2. Input command
   3. Once the process reaches the '(exit)' entered by the user, directly terminate the program.
      ![image](https://github.com/user-attachments/assets/4dbf627e-ddbe-4b4f-b8f4-c040df3a8c1a)

   4. Since the input is provided by an input file instead of interactive I/O, it is possible that the user has not yet finished entering an \
      S-expression and then runs out of input. In this case, the system should print the following message: 
     
       ERROR (no more input): END-OF-FILE encountered
       
       Thanks for using OurScheme!
       ![image](https://github.com/user-attachments/assets/2829a4ad-db1c-4ff9-a3f7-5a5d1355b5a6)



