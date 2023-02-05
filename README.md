# Thread-synchronization-exercise
practice on writing code in C, using threads-programming

Create an array of 100 entries. 
Create 4 threads in ascending priority:
 
Thread 4:
 
Endless loop of:
Thread 4 (highest priority) is triggered (awaken) every 1-10msec randomly
Read the system clock
Take the last 2 digits (decimal 0-99) as a “parameter”
trigger threads 2 and 3 simultaneously with this “parameter”.
 
Tread 2 and 3:
 
Triggered upon receiving the parameter
Tread 2 and 3 writes a string to the “parameter” entry in the array
The string is: the thread name “Th222222222” or “tH33333333”, the system time, the line number of the current executed line of code”.
 
Tread 1:
 
Tread 1 is triggered every 50msec and add the array to a log file 
When the log file exceeds the size of 1Mbyte it should end the process.
 
The file should be readable and well organized.
