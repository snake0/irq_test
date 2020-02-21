
user_cpu_test.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <rdtscp>:
   0:	0f 01 f9             	rdtscp 
   3:	48 c1 e2 20          	shl    $0x20,%rdx
   7:	89 c0                	mov    %eax,%eax
   9:	48 09 d0             	or     %rdx,%rax
   c:	c3                   	retq   
   d:	0f 1f 00             	nopl   (%rax)

0000000000000010 <cal_sqrt>:
  10:	48 b8 ff ff ff ff ff 	movabs $0x3fffffffffffffff,%rax
  17:	ff ff 3f 
  1a:	48 ba 00 00 00 00 00 	movabs $0x4000000000000000,%rdx
  21:	00 00 40 
  24:	48 39 c7             	cmp    %rax,%rdi
  27:	77 15                	ja     3e <cal_sqrt+0x2e>
  29:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
  30:	48 c1 ea 02          	shr    $0x2,%rdx
  34:	48 39 d7             	cmp    %rdx,%rdi
  37:	72 f7                	jb     30 <cal_sqrt+0x20>
  39:	48 85 d2             	test   %rdx,%rdx
  3c:	74 32                	je     70 <cal_sqrt+0x60>
  3e:	31 c0                	xor    %eax,%eax
  40:	48 8d 0c 10          	lea    (%rax,%rdx,1),%rcx
  44:	48 39 cf             	cmp    %rcx,%rdi
  47:	72 17                	jb     60 <cal_sqrt+0x50>
  49:	48 8d 04 11          	lea    (%rcx,%rdx,1),%rax
  4d:	48 c1 ea 02          	shr    $0x2,%rdx
  51:	48 29 cf             	sub    %rcx,%rdi
  54:	48 d1 e8             	shr    %rax
  57:	48 85 d2             	test   %rdx,%rdx
  5a:	75 e4                	jne    40 <cal_sqrt+0x30>
  5c:	f3 c3                	repz retq 
  5e:	66 90                	xchg   %ax,%ax
  60:	48 c1 ea 02          	shr    $0x2,%rdx
  64:	48 d1 e8             	shr    %rax
  67:	48 85 d2             	test   %rdx,%rdx
  6a:	75 d4                	jne    40 <cal_sqrt+0x30>
  6c:	f3 c3                	repz retq 
  6e:	66 90                	xchg   %ax,%ax
  70:	31 c0                	xor    %eax,%eax
  72:	c3                   	retq   
  73:	0f 1f 00             	nopl   (%rax)
  76:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  7d:	00 00 00 

0000000000000080 <test_cpu>:
  80:	55                   	push   %rbp
  81:	53                   	push   %rbx
  82:	48 8d 1d 00 00 00 00 	lea    0x0(%rip),%rbx        # 89 <test_cpu+0x9>
  89:	bd 0a 00 00 00       	mov    $0xa,%ebp
  8e:	48 83 ec 08          	sub    $0x8,%rsp
  92:	0f 01 f9             	rdtscp 
  95:	48 c1 e2 20          	shl    $0x20,%rdx
  99:	89 c0                	mov    %eax,%eax
  9b:	bf 03 00 00 00       	mov    $0x3,%edi
  a0:	48 09 c2             	or     %rax,%rdx
  a3:	49 89 d0             	mov    %rdx,%r8
  a6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  ad:	00 00 00 
  b0:	48 b8 00 00 00 00 00 	movabs $0x4000000000000000,%rax
  b7:	00 00 40 
  ba:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)
  c0:	48 c1 e8 02          	shr    $0x2,%rax
  c4:	48 39 f8             	cmp    %rdi,%rax
  c7:	77 f7                	ja     c0 <test_cpu+0x40>
  c9:	48 89 f9             	mov    %rdi,%rcx
  cc:	31 f6                	xor    %esi,%esi
  ce:	66 90                	xchg   %ax,%ax
  d0:	48 8d 14 30          	lea    (%rax,%rsi,1),%rdx
  d4:	48 39 ca             	cmp    %rcx,%rdx
  d7:	77 7f                	ja     158 <test_cpu+0xd8>
  d9:	48 8d 34 02          	lea    (%rdx,%rax,1),%rsi
  dd:	48 c1 e8 02          	shr    $0x2,%rax
  e1:	48 29 d1             	sub    %rdx,%rcx
  e4:	48 d1 ee             	shr    %rsi
  e7:	48 85 c0             	test   %rax,%rax
  ea:	75 e4                	jne    d0 <test_cpu+0x50>
  ec:	48 83 fe 01          	cmp    $0x1,%rsi
  f0:	76 24                	jbe    116 <test_cpu+0x96>
  f2:	40 f6 c7 01          	test   $0x1,%dil
  f6:	74 1e                	je     116 <test_cpu+0x96>
  f8:	b9 02 00 00 00       	mov    $0x2,%ecx
  fd:	eb 0e                	jmp    10d <test_cpu+0x8d>
  ff:	90                   	nop
 100:	31 d2                	xor    %edx,%edx
 102:	48 89 f8             	mov    %rdi,%rax
 105:	48 f7 f1             	div    %rcx
 108:	48 85 d2             	test   %rdx,%rdx
 10b:	74 09                	je     116 <test_cpu+0x96>
 10d:	48 83 c1 01          	add    $0x1,%rcx
 111:	48 39 f1             	cmp    %rsi,%rcx
 114:	76 ea                	jbe    100 <test_cpu+0x80>
 116:	48 83 c7 01          	add    $0x1,%rdi
 11a:	48 81 ff 40 42 0f 00 	cmp    $0xf4240,%rdi
 121:	75 8d                	jne    b0 <test_cpu+0x30>
 123:	0f 01 f9             	rdtscp 
 126:	48 c1 e2 20          	shl    $0x20,%rdx
 12a:	89 c0                	mov    %eax,%eax
 12c:	48 89 de             	mov    %rbx,%rsi
 12f:	48 09 c2             	or     %rax,%rdx
 132:	bf 01 00 00 00       	mov    $0x1,%edi
 137:	31 c0                	xor    %eax,%eax
 139:	4c 29 c2             	sub    %r8,%rdx
 13c:	e8 00 00 00 00       	callq  141 <test_cpu+0xc1>
 141:	83 ed 01             	sub    $0x1,%ebp
 144:	0f 85 48 ff ff ff    	jne    92 <test_cpu+0x12>
 14a:	48 83 c4 08          	add    $0x8,%rsp
 14e:	5b                   	pop    %rbx
 14f:	5d                   	pop    %rbp
 150:	c3                   	retq   
 151:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)
 158:	48 c1 e8 02          	shr    $0x2,%rax
 15c:	48 d1 ee             	shr    %rsi
 15f:	48 85 c0             	test   %rax,%rax
 162:	0f 85 68 ff ff ff    	jne    d0 <test_cpu+0x50>
 168:	eb 82                	jmp    ec <test_cpu+0x6c>

Disassembly of section .text.startup:

0000000000000000 <main>:
   0:	48 83 ec 08          	sub    $0x8,%rsp
   4:	31 c0                	xor    %eax,%eax
   6:	e8 00 00 00 00       	callq  b <main+0xb>
   b:	31 c0                	xor    %eax,%eax
   d:	48 83 c4 08          	add    $0x8,%rsp
  11:	c3                   	retq   
