### unknown (tuctf 2017)

[Binary](unknown)  
This is reverse-engineering task worth 200 points from TUCTF_2017. I've created this tool during this ctf especially for this task (it could have been done easier via gdb scripts or some pwntools but that wouldn't be so much fun).

Let's play with it first

```
$ ./unknown
Try again.
$ file unknown
unknown: ELF 64-bit LSB executable, x86-64, version 1 (SYSV), dynamically linked, interpreter /lib64/ld-linux-x86-64.so.2, for GNU/Linux 2.6.32, BuildID[sha1]=53ec94bd1406ec6b9a28f5308a92e4d906444edb, stripped

radare2 unknown
[0x00400600]> aaa
[0x00400600]> s main
[0x00401c02]> pd 80
/ (fcn) main 210
|           0x00401c02    55           push rbp
|           0x00401c03    4889e5       mov rbp, rsp
|           0x00401c06    4883ec20     sub rsp, 0x20
|           0x00401c0a    897dec       mov [rbp-0x14], edi
|           0x00401c0d    488975e0     mov [rbp-0x20], rsi
|           0x00401c11    837dec02     cmp dword [rbp-0x14], 0x2
|       ,=< 0x00401c15    7414         jz 0x401c2b
|       |   0x00401c17    bf761d4000   mov edi, str.Tryagain.
|       |   0x00401c1c    e84fe9ffff   call sym.imp.puts
|       |      sym.imp.puts(unk)
|       |   0x00401c21    b8ffffffff   mov eax, 0xffffffff
|      ,==< 0x00401c26    e9a7000000   jmp loc.00401cd2
|      |`-> 0x00401c2b    488b45e0     mov rax, [rbp-0x20]
|      |    0x00401c2f    4883c008     add rax, 0x8
|      |    0x00401c33    488b00       mov rax, [rax]
|      |    0x00401c36    4889c7       mov rdi, rax
|      |    0x00401c39    e842e9ffff   call sym.imp.strlen
|      |       sym.imp.strlen()
|      |    ; DATA XREF from 0x00401e8c (unk)
|      |    0x00401c3e    8b1548020000 mov edx, [rip+0x248] ; 0x00401e8c 
|      |    0x00401c44    89d2         mov edx, edx
|      |    0x00401c46    4839d0       cmp rax, rdx
|     ,===< 0x00401c49    7411         jz 0x401c5c
|     ||    0x00401c4b    bf811d4000   mov edi, str.Stillnope.
|     ||    0x00401c50    e81be9ffff   call sym.imp.puts
|     ||       sym.imp.puts()
|     ||    0x00401c55    b8feffffff   mov eax, 0xfffffffe
|           ; CODE (CALL) XREF from 0x00401cd2 (unk)
|    ,====< 0x00401c5a    eb76         jmp loc.00401cd2
|    |`---> 0x00401c5c    488b45e0     mov rax, [rbp-0x20]
|    | |    0x00401c60    488b4008     mov rax, [rax+0x8]
|    | |    0x00401c64    488945f8     mov [rbp-0x8], rax
|    | |    0x00401c68    c745f400000. mov dword [rbp-0xc], 0x0
|   ,=====< 0x00401c6f    eb23         jmp 0x401c94 ; (loc.00401ae1)
| .-------> 0x00401c71    8b55f4       mov edx, [rbp-0xc]
|- fcn.00401c94 96
| | || |    0x00401c74    488b45f8     mov rax, [rbp-0x8]
| | || |    0x00401c78    89d6         mov esi, edx
| | || |    0x00401c7a    4889c7       mov rdi, rax
| | || |    0x00401c7d    e80e020000   call fcn.00401e90
| | || |       fcn.00401e90() ; section_end..DATA
| | || |    0x00401c82    85c0         test eax, eax
| |,======< 0x00401c84    740a         jz 0x401c90
| |||| |    0x00401c86    c705f413200. mov dword [rip+0x2013f4], 0x1
| |`------> 0x00401c90    8345f401     add dword [rbp-0xc], 0x1
| | |       ; CODE (CALL) XREF from 0x00401c6f (unk)
| | `-----> 0x00401c94    8b55f4       mov edx, [rbp-0xc]
| |  | |    ; DATA XREF from 0x00401e8c (unk)
| |  | |    0x00401c97    8b05ef010000 mov eax, [rip+0x1ef] ; 0x00401e8c 
| |  | |    0x00401c9d    39c2         cmp edx, eax
| `=======< 0x00401c9f    72d0         jb 0x401c71
|    | |    ; DATA XREF from 0x00403084 (unk)
|    | |    0x00401ca1    8b05dd132000 mov eax, [rip+0x2013dd] ; 0x00403084 
|    | |    0x00401ca7    85c0         test eax, eax
| ========< 0x00401ca9    740c         jz 0x401cb7
|    | |    0x00401cab    bf8d1d4000   mov edi, str.Nope.
|    | |    ; CODE (CALL) XREF from 0x00400570 (fcn.0040056c)
|    | |    0x00401cb0    e8bbe8ffff   call sym.imp.puts
|    | |       sym.imp.puts()
| ========< 0x00401cb5    eb16         jmp loc.00401ccd
| --------> 0x00401cb7    488b45f8     mov rax, [rbp-0x8]
|    | |    0x00401cbb    4889c6       mov rsi, rax
|    | |    0x00401cbe    bf931d4000   mov edi, str.Congraztheflagiss
|    | |    0x00401cc3    b800000000   mov eax, 0x0
|    | |    0x00401cc8    e8d3e8ffff   call sym.imp.printf
|    | |       sym.imp.printf()
|           ; CODE (CALL) XREF from 0x00401cb5 (unk)
|- loc.00401ccd 7
| --------> 0x00401ccd    b800000000   mov eax, 0x0
|    | |    ; CODE (CALL) XREF from 0x00401c5a (unk)
|    | |    ; CODE (CALL) XREF from 0x00401c26 (unk)
|- loc.00401cd2 2
|    `-`--> 0x00401cd2    c9           leave
|           0x00401cd3    c3           ret
```

So this binary firstly checks if `argc != 2 and strlen(argv[1]) != 56` and in both cases it exits. Then it goes like this:

```
for(int i = 0; i < 56; i++){
  call 0x401e90 (i, argv[1][i])
  //this function returned 0 or 1 but was quite complicated "inside"
  //[details](https://github.com/Sinkmanu/CTF/tree/master/TUCTF-2017-Unknown)
  //so I've tried bruteforcing it
}

```

As we can see this is perfect example for eh.  

```
addr1 = 0x401c82
addr2 might be 0x401c9f
we want to avoid 1 as call 0x401e90 results
rax is the register we use
and passwords has 56 characters
```

```
./dbgkeygen unknown 0x401c82 0x401c9f 1 rax 56
[7794] Password(0) = 
[7794] Password(1) = T
[7794] Password(2) = TU
[7794] Password(3) = TUC
[7794] Password(4) = TUCT
[7794] Password(5) = TUCTF
[7794] Password(6) = TUCTF{
[7794] Password(7) = TUCTF{w
[7794] Password(8) = TUCTF{w3
[7794] Password(9) = TUCTF{w3l
[7794] Password(10) = TUCTF{w3lc
[7794] Password(11) = TUCTF{w3lc0
[7794] Password(12) = TUCTF{w3lc0m
[7794] Password(13) = TUCTF{w3lc0m3
[7794] Password(14) = TUCTF{w3lc0m3_
[7794] Password(15) = TUCTF{w3lc0m3_7
[7794] Password(16) = TUCTF{w3lc0m3_70
[7794] Password(17) = TUCTF{w3lc0m3_70_
[7794] Password(18) = TUCTF{w3lc0m3_70_7
[7794] Password(19) = TUCTF{w3lc0m3_70_7u
[7794] Password(20) = TUCTF{w3lc0m3_70_7uc
[7794] Password(21) = TUCTF{w3lc0m3_70_7uc7
[7794] Password(22) = TUCTF{w3lc0m3_70_7uc7f
[7794] Password(23) = TUCTF{w3lc0m3_70_7uc7f_
[7794] Password(24) = TUCTF{w3lc0m3_70_7uc7f_4
[7794] Password(25) = TUCTF{w3lc0m3_70_7uc7f_4n
[7794] Password(26) = TUCTF{w3lc0m3_70_7uc7f_4nd
[7794] Password(27) = TUCTF{w3lc0m3_70_7uc7f_4nd_
[7794] Password(28) = TUCTF{w3lc0m3_70_7uc7f_4nd_7
[7794] Password(29) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h
[7794] Password(30) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4
[7794] Password(31) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4n
[7794] Password(32) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk
[7794] Password(33) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_
[7794] Password(34) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y
[7794] Password(35) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0
[7794] Password(36) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u
[7794] Password(37) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_
[7794] Password(38) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f
[7794] Password(39) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0
[7794] Password(40) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r
[7794] Password(41) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_
[7794] Password(42) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p
[7794] Password(43) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4
[7794] Password(44) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r
[7794] Password(45) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r7
[7794] Password(46) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71
[7794] Password(47) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c
[7794] Password(48) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1
[7794] Password(49) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1p
[7794] Password(50) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1p4
[7794] Password(51) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1p47
[7794] Password(52) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1p471
[7794] Password(53) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1p471n
[7794] Password(54) = TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1p471n6
Congraz the flag is: TUCTF{w3lc0m3_70_7uc7f_4nd_7h4nk_y0u_f0r_p4r71c1p471n6!}
```

It was very entertaining task.