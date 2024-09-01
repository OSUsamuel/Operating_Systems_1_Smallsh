This is my portfolio project for my Operating Systems 1 class. 

Description of files:
  main.c: This is the code that creates the user interface
  code.c: This code is where all the leg work done
  smallsh: This is the executable file for the project
  p3testscript: This is the test file my graders used

Criteria (Found on the canvas page for this assignment):
In this assignment you will write smallsh your own shell in C. smallsh will implement a subset of features of well-known shells, such as bash. Your program will
<ol>
  <li>Provide a prompt for running commands</li>
  <li>Handle blank lines and comments, which are lines beginning with the # character</li>
  <li>Provide expansion for the variable $$</li>
  <li>Execute 3 commands exit, cd, and status via code built into the shell</li>
  <li>Execute other commands by creating new processes using a function from the exec family of functions</li>
  <li>Support input and output redirection</li>
  <li>Support running commands in foreground and background processes</li>
  <li>Implement custom handlers for 2 signals, SIGINT and SIGTSTP</li>
</ol>



Learning Outcomes
After successful completion of this assignment, you should be able to do the following
Describe the Unix process API (Module 4, MLO 2)
Write programs using the Unix process API (Module 4, MLO 3)
Explain the concept of signals and their uses (Module 5, MLO 2)
Write programs using the Unix API for signal handling (Module 5, MLO 3)
Explain I/O redirection and write programs that can employ I/O redirection (Module 5, MLO 4)
