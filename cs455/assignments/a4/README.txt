Tim Hung
thung1@binghamton.edu
CS 455
Assignment 4

Execution instructions:

    Code is all located in dev/

    Run "make" to compile all code to the ./driver executable.

    Execute ./driver to run the program.

    Hotkeys:
        
        <ESC>       Exit
        <SPACE_BAR> Revert to original image
        <g>         Convert to grayscale
        <r>         Convert to rgb 
        <n>         Negative
        <D>         Detect regions
        <b>         Binary
        <B>         Blur
        <u>         Unsharp mask
        <d>         Dilating
        <e>         Eroding
        <o>         Opening
        <c>         Closing
        <x>         Cross strel
        <s>         Square strel
        <1-9>       Set strel size
        <p>	        Count pigs macro

    To count the pigs, run these commands in this order:
        <g>         Convert to grayscale
        <b>         Binary
        <2>         Set strel size to 2
        <s>         Sets strel to a square shape
        <o>         Open once
        <c>         Close once
        <D>         Detect regions, output is printed in console

    or, just run the command
        <p>         Automatically runs those commands
