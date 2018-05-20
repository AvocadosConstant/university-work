        .file   "gcdR.c"
        .text
        .globl  gcdR
        .type   gcdR, @function
gcdR:
        pushq   %rbp  # write previous stack pointer to new frame
        movq    %rsp, %rbp
        subq    $16, %rsp
        movl    %edi, -4(%rbp) # write param i to stack
        movl    %esi, -8(%rbp) # write param j to stack
        movl    -4(%rbp), %eax # read i to eax
        cmpl    -8(%rbp), %eax # read j  and comp to eax (i)
        jne     .L2
        movl    -4(%rbp), %eax # read i to eax for return
        jmp     .L3
.L2:
        movl    -4(%rbp), %eax # read i to eax
        cmpl    -8(%rbp), %eax # read j and comp to eax (i)
        jle     .L4
        movl    -4(%rbp), %eax # read i value to eax
        subl    -8(%rbp), %eax # subtract eax (i) from j
        movl    -8(%rbp), %edx # move j to edx
        movl    %edx, %esi # move j to esi for next call param
        movl    %eax, %edi # move j - i to edi for next call
        call    gcdR
        jmp     .L3
.L4:
        movl    -8(%rbp), %eax # read j to eax
        subl    -4(%rbp), %eax # Subtract i from eax
        movl    %eax, %edx
        movl    -4(%rbp), %eax # move i to eax
        movl    %edx, %esi
        movl    %eax, %edi
        call    gcdR
.L3:
        leave
        ret
        .size   gcdR, .-gcdR
        .ident  "GCC: (Debian 6.3.0-18) 6.3.0 20170516"
        .section        .note.GNU-stack,"",@progbits
