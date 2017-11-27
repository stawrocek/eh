### simple xorme

This is very simple example of eh usage.  
[Source code](xorme.c)  
[Binary](xorme)  

Let's try some passwords first

```
./xorme 0987654321
wrong.
./xorme 0987654321098765432
Wrong! I knew you were trouble!
./xorme 09876543210987654aa
Wrong! I knew you were trouble!
```

Firstly we need to determine constants used by eh.  
```
objdump -d xorme -M intel

...

00000000004005ab <main>:
  4005ab:	55                   	push   rbp
  4005ac:	48 89 e5             	mov    rbp,rsp
  4005af:	53                   	push   rbx
  4005b0:	48 83 ec 28          	sub    rsp,0x28
  4005b4:	89 7d dc             	mov    DWORD PTR [rbp-0x24],edi
  4005b7:	48 89 75 d0          	mov    QWORD PTR [rbp-0x30],rsi
  4005bb:	83 7d dc 01          	cmp    DWORD PTR [rbp-0x24],0x1
  4005bf:	7f 14                	jg     4005d5 <main+0x2a>
  4005c1:	bf 28 07 40 00       	mov    edi,0x400728
  4005c6:	e8 65 fe ff ff       	call   400430 <puts@plt>
  4005cb:	b8 ff ff ff ff       	mov    eax,0xffffffff
  4005d0:	e9 b7 00 00 00       	jmp    40068c <main+0xe1>
  4005d5:	48 8b 45 d0          	mov    rax,QWORD PTR [rbp-0x30]
  4005d9:	48 83 c0 08          	add    rax,0x8
  4005dd:	48 8b 00             	mov    rax,QWORD PTR [rax]
  4005e0:	48 89 c7             	mov    rdi,rax
  4005e3:	e8 58 fe ff ff       	call   400440 <strlen@plt>
  4005e8:	48 83 f8 13          	cmp    rax,0x13
  4005ec:	74 14                	je     400602 <main+0x57>
  4005ee:	bf 39 07 40 00       	mov    edi,0x400739
  4005f3:	e8 38 fe ff ff       	call   400430 <puts@plt>
  4005f8:	b8 ff ff ff ff       	mov    eax,0xffffffff
  4005fd:	e9 8a 00 00 00       	jmp    40068c <main+0xe1>
  400602:	c7 45 ec 00 00 00 00 	mov    DWORD PTR [rbp-0x14],0x0
  400609:	eb 41                	jmp    40064c <main+0xa1>
  40060b:	48 8b 45 d0          	mov    rax,QWORD PTR [rbp-0x30]
  40060f:	48 83 c0 08          	add    rax,0x8
  400613:	48 8b 10             	mov    rdx,QWORD PTR [rax]
  400616:	8b 45 ec             	mov    eax,DWORD PTR [rbp-0x14]
  400619:	48 98                	cdqe   
  40061b:	48 01 d0             	add    rax,rdx
  40061e:	0f b6 00             	movzx  eax,BYTE PTR [rax]
  400621:	0f be c0             	movsx  eax,al
  400624:	8b 55 ec             	mov    edx,DWORD PTR [rbp-0x14]
  400627:	89 d6                	mov    esi,edx
  400629:	89 c7                	mov    edi,eax
  40062b:	e8 46 ff ff ff       	call   400576 <checkCharacter>
  400630:	3d e5 03 00 00       	cmp    eax,0x3e5
  400635:	75 11                	jne    400648 <main+0x9d>
  400637:	bf 40 07 40 00       	mov    edi,0x400740
  40063c:	e8 ef fd ff ff       	call   400430 <puts@plt>
  400641:	b8 ff ff ff ff       	mov    eax,0xffffffff
  400646:	eb 44                	jmp    40068c <main+0xe1>
  400648:	83 45 ec 01          	add    DWORD PTR [rbp-0x14],0x1
  40064c:	8b 45 ec             	mov    eax,DWORD PTR [rbp-0x14]
  40064f:	48 63 d8             	movsxd rbx,eax
  400652:	48 8b 45 d0          	mov    rax,QWORD PTR [rbp-0x30]
  400656:	48 83 c0 08          	add    rax,0x8
  40065a:	48 8b 00             	mov    rax,QWORD PTR [rax]
  40065d:	48 89 c7             	mov    rdi,rax
  400660:	e8 db fd ff ff       	call   400440 <strlen@plt>
  400665:	48 39 c3             	cmp    rbx,rax
  400668:	72 a1                	jb     40060b <main+0x60>
  40066a:	48 8b 45 d0          	mov    rax,QWORD PTR [rbp-0x30]
  40066e:	48 83 c0 08          	add    rax,0x8
  400672:	48 8b 00             	mov    rax,QWORD PTR [rax]
  400675:	48 89 c6             	mov    rsi,rax
  400678:	bf 60 07 40 00       	mov    edi,0x400760
  40067d:	b8 00 00 00 00       	mov    eax,0x0
  400682:	e8 c9 fd ff ff       	call   400450 <printf@plt>
  400687:	b8 00 00 00 00       	mov    eax,0x0
  40068c:	48 83 c4 28          	add    rsp,0x28
  400690:	5b                   	pop    rbx
  400691:	5d                   	pop    rbp
  400692:	c3                   	ret
```

So, we can see that must be 0x13 characters long
```
  4005e3:	e8 58 fe ff ff       	call   400440 <strlen@plt>
  4005e8:	48 83 f8 13          	cmp    rax,0x13
```
Next, we can see that if `checkCharacter` returns 997 we have trouble. 

```
400624:	8b 55 ec             	mov    edx,DWORD PTR [rbp-0x14]
400627:	89 d6                	mov    esi,edx
400629:	89 c7                	mov    edi,eax
40062b:	e8 46 ff ff ff       	call   400576 <checkCharacter>
400630:	3d e5 03 00 00       	cmp    eax,0x3e5
```
From this snippet we can also take both addresses needed by eh.   
Let `addr1=0x400630` and `addr2=0x40624`.  
We also see that `eax` is used for comparison.

So now we simply run.   
```
./eh xorme 0x400630 0x400624 997 eax 0x13
[7159] Password(0) = 
[7159] Password(1) = w
[7159] Password(2) = wu
[7159] Password(3) = wub
[7159] Password(4) = wubb
[7159] Password(5) = wubba
[7159] Password(6) = wubba_
[7159] Password(7) = wubba_l
[7159] Password(8) = wubba_lu
[7159] Password(9) = wubba_lub
[7159] Password(10) = wubba_lubb
[7159] Password(11) = wubba_lubba
[7159] Password(12) = wubba_lubba_
[7159] Password(13) = wubba_lubba_d
[7159] Password(14) = wubba_lubba_du
[7159] Password(15) = wubba_lubba_dub
[7159] Password(16) = wubba_lubba_dub_
[7159] Password(17) = wubba_lubba_dub_d
Welcome aboard! wubba_lubba_dub_dub
```