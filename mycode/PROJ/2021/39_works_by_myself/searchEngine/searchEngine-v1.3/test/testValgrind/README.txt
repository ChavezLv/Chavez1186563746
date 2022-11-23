01main.c  
$gcc -g main.c  
$valgrind --tool=memcheck ./a.out

>> ==29290== Invalid write of size 4
>> ==29290== Rerun with --leak-check=full to see details of leaked memory
>> ==29290==
>> ==29290== For counts of detected and suppressed errors, rerun with: -v
>> ==29290== ERROR SUMMARY: 1 errors from 1 contexts (suppressed: 0 from 0)


