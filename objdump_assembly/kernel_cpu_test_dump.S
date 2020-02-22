
../irq_switch.o:     file format elf64-x86-64


Disassembly of section .text:

0000000000000000 <test_prime>:
   0:	e8 00 00 00 00       	callq  5 <test_prime+0x5>
   5:	55                   	push   %rbp
   6:	48 89 e5             	mov    %rsp,%rbp
   9:	41 54                	push   %r12
   b:	49 c7 c4 00 00 00 00 	mov    $0x0,%r12
  12:	53                   	push   %rbx
  13:	bb 0a 00 00 00       	mov    $0xa,%ebx
  18:	0f 01 f9             	rdtscp 
  1b:	48 c1 e2 20          	shl    $0x20,%rdx
  1f:	89 c0                	mov    %eax,%eax
  21:	bf 03 00 00 00       	mov    $0x3,%edi
  26:	48 09 c2             	or     %rax,%rdx
  29:	49 89 d1             	mov    %rdx,%r9
  2c:	48 b8 00 00 00 00 00 	movabs $0x4000000000000000,%rax
  33:	00 00 40 
  36:	48 c1 e8 02          	shr    $0x2,%rax
  3a:	48 39 f8             	cmp    %rdi,%rax
  3d:	77 f7                	ja     36 <test_prime+0x36>
  3f:	48 89 f9             	mov    %rdi,%rcx
  42:	31 f6                	xor    %esi,%esi
  44:	48 8d 14 06          	lea    (%rsi,%rax,1),%rdx
  48:	48 39 ca             	cmp    %rcx,%rdx
  4b:	77 07                	ja     54 <test_prime+0x54>
  4d:	48 8d 34 02          	lea    (%rdx,%rax,1),%rsi
  51:	48 29 d1             	sub    %rdx,%rcx
  54:	48 c1 e8 02          	shr    $0x2,%rax
  58:	48 d1 ee             	shr    %rsi
  5b:	48 85 c0             	test   %rax,%rax
  5e:	75 e4                	jne    44 <test_prime+0x44>
  60:	48 83 fe 01          	cmp    $0x1,%rsi
  64:	76 23                	jbe    89 <test_prime+0x89>
  66:	40 f6 c7 01          	test   $0x1,%dil
  6a:	74 1d                	je     89 <test_prime+0x89>
  6c:	b9 02 00 00 00       	mov    $0x2,%ecx
  71:	eb 0d                	jmp    80 <test_prime+0x80>
  73:	31 d2                	xor    %edx,%edx
  75:	48 89 f8             	mov    %rdi,%rax
  78:	48 f7 f1             	div    %rcx
  7b:	48 85 d2             	test   %rdx,%rdx
  7e:	74 09                	je     89 <test_prime+0x89>
  80:	48 83 c1 01          	add    $0x1,%rcx
  84:	48 39 f1             	cmp    %rsi,%rcx
  87:	76 ea                	jbe    73 <test_prime+0x73>
  89:	48 83 c7 01          	add    $0x1,%rdi
  8d:	48 81 ff 40 42 0f 00 	cmp    $0xf4240,%rdi
  94:	75 96                	jne    2c <test_prime+0x2c>
  96:	0f 01 f9             	rdtscp 
  99:	48 c1 e2 20          	shl    $0x20,%rdx
  9d:	41 89 c0             	mov    %eax,%r8d
  a0:	b9 40 42 0f 00       	mov    $0xf4240,%ecx
  a5:	49 09 d0             	or     %rdx,%r8
  a8:	48 c7 c6 00 00 00 00 	mov    $0x0,%rsi
  af:	4c 89 e2             	mov    %r12,%rdx
  b2:	4d 29 c8             	sub    %r9,%r8
  b5:	48 c7 c7 00 00 00 00 	mov    $0x0,%rdi
  bc:	e8 00 00 00 00       	callq  c1 <test_prime+0xc1>
  c1:	83 eb 01             	sub    $0x1,%ebx
  c4:	0f 85 4e ff ff ff    	jne    18 <test_prime+0x18>
  ca:	5b                   	pop    %rbx
  cb:	31 c0                	xor    %eax,%eax
  cd:	41 5c                	pop    %r12
  cf:	5d                   	pop    %rbp
  d0:	c3                   	retq   
  d1:	0f 1f 44 00 00       	nopl   0x0(%rax,%rax,1)
  d6:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
  dd:	00 00 00 

00000000000000e0 <cal_sqrt>:
  e0:	e8 00 00 00 00       	callq  e5 <cal_sqrt+0x5>
  e5:	55                   	push   %rbp
  e6:	48 b8 ff ff ff ff ff 	movabs $0x3fffffffffffffff,%rax
  ed:	ff ff 3f 
  f0:	48 ba 00 00 00 00 00 	movabs $0x4000000000000000,%rdx
  f7:	00 00 40 
  fa:	48 39 c7             	cmp    %rax,%rdi
  fd:	48 89 e5             	mov    %rsp,%rbp
 100:	77 0e                	ja     110 <cal_sqrt+0x30>
 102:	48 c1 ea 02          	shr    $0x2,%rdx
 106:	48 39 d7             	cmp    %rdx,%rdi
 109:	72 f7                	jb     102 <cal_sqrt+0x22>
 10b:	48 85 d2             	test   %rdx,%rdx
 10e:	74 20                	je     130 <cal_sqrt+0x50>
 110:	31 c0                	xor    %eax,%eax
 112:	48 8d 0c 10          	lea    (%rax,%rdx,1),%rcx
 116:	48 39 f9             	cmp    %rdi,%rcx
 119:	77 07                	ja     122 <cal_sqrt+0x42>
 11b:	48 8d 04 11          	lea    (%rcx,%rdx,1),%rax
 11f:	48 29 cf             	sub    %rcx,%rdi
 122:	48 c1 ea 02          	shr    $0x2,%rdx
 126:	48 d1 e8             	shr    %rax
 129:	48 85 d2             	test   %rdx,%rdx
 12c:	75 e4                	jne    112 <cal_sqrt+0x32>
 12e:	5d                   	pop    %rbp
 12f:	c3                   	retq   
 130:	31 c0                	xor    %eax,%eax
 132:	5d                   	pop    %rbp
 133:	c3                   	retq   
 134:	66 90                	xchg   %ax,%ax
 136:	66 2e 0f 1f 84 00 00 	nopw   %cs:0x0(%rax,%rax,1)
 13d:	00 00 00 

0000000000000140 <rdtscp>:
 140:	e8 00 00 00 00       	callq  145 <rdtscp+0x5>
 145:	55                   	push   %rbp
 146:	48 89 e5             	mov    %rsp,%rbp
 149:	0f 01 f9             	rdtscp 
 14c:	48 c1 e2 20          	shl    $0x20,%rdx
 150:	89 c0                	mov    %eax,%eax
 152:	48 09 d0             	or     %rdx,%rax
 155:	5d                   	pop    %rbp
 156:	c3                   	retq   

Disassembly of section .init.text:

0000000000000000 <init_module>:
   0:	e8 00 00 00 00       	callq  5 <init_module+0x5>
   5:	55                   	push   %rbp
   6:	83 ca ff             	or     $0xffffffff,%edx
   9:	31 f6                	xor    %esi,%esi
   b:	48 c7 c1 00 00 00 00 	mov    $0x0,%rcx
  12:	48 c7 c7 00 00 00 00 	mov    $0x0,%rdi
  19:	48 89 e5             	mov    %rsp,%rbp
  1c:	e8 00 00 00 00       	callq  21 <init_module+0x21>
  21:	48 3d 00 f0 ff ff    	cmp    $0xfffffffffffff000,%rax
  27:	48 89 05 00 00 00 00 	mov    %rax,0x0(%rip)        # 2e <init_module+0x2e>
  2e:	77 19                	ja     49 <init_module+0x49>
  30:	48 89 c7             	mov    %rax,%rdi
  33:	be 03 00 00 00       	mov    $0x3,%esi
  38:	e8 00 00 00 00       	callq  3d <init_module+0x3d>
  3d:	48 8b 3d 00 00 00 00 	mov    0x0(%rip),%rdi        # 44 <init_module+0x44>
  44:	e8 00 00 00 00       	callq  49 <init_module+0x49>
  49:	31 c0                	xor    %eax,%eax
  4b:	5d                   	pop    %rbp
  4c:	c3                   	retq   

Disassembly of section .exit.text:

0000000000000000 <cleanup_module>:
   0:	55                   	push   %rbp
   1:	48 89 e5             	mov    %rsp,%rbp
   4:	5d                   	pop    %rbp
   5:	c3                   	retq   
