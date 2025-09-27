# BFCC64: Brainfuck compact compiler x64
## History
This is mine first pet project. I became interested in code creation and generation. This project was originally conceived as a code translator to nasm, because it was finished product that can create obj file for windows or linux. But i was attracted by the choice of an independent compiler.
Since this is my first project, there may be some bugs, useless code, govnocode(crap code). But i followed the "If it works, dont change it" rule.
## How to use?
### Build
You can use bfcc.exe -h or bfcc.exe --help. But i will write here a complete list of possible arguments.
```
> -h - prints all arguments and exit    (optiobal)
> -p - set platform, by default win64   (Doenst works)
> -f - set file path
> -s - start arg, just interpretate the input program and exit  (optiobal)
> -o - output file name(path), by default a.obj (technically optional)
> -m - set memory size in data section by default 30000(brainfuck standard) (optional)
```
> [!WARNING]
> -m affects the size of the output file

Here is an example:
(for use bfcc without path to program, just add bfcc path in global vars in windows)

``` bfcc -f *file path (name)* -o *output file path(name)* -m 30 ``` <- compile program

or

``` bfcc -f *file path (name)* -s -m 30 ``` <- debugs(run) program
### Link
for minimal size of output executable file i use ld with this args:

```ld *input file name* -o *output file name* -lmsvcrt --gc-sections --entry main --subsystem console```

## Bugs and tips
if you have any questions or advice or bugs, then write to me, I will answer everything you can write in discord: ```meloch_``` or write me an email.