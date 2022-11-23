	.file	"a0012test.c"
	.text
	.section	.rodata
	.type	_ZStL19piecewise_construct, @object
	.size	_ZStL19piecewise_construct, 1
_ZStL19piecewise_construct:
	.zero	1
	.local	_ZStL8__ioinit
	.comm	_ZStL8__ioinit,1,1
	.text
	.globl	_Z6test01v
	.type	_Z6test01v, @function
_Z6test01v:
.LFB1496:
	.cfi_startproc
	.cfi_personality 0x9b,DW.ref.__gxx_personality_v0
	.cfi_lsda 0x1b,.LLSDA1496
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	movq	%fs:40, %rax
	movq	%rax, -24(%rbp)
	xorl	%eax, %eax
	leaq	-25(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN4TempIiEC1Ev
	leaq	-25(%rbp), %rax
	movl	$6, %edx
	movl	$5, %esi
	movq	%rax, %rdi
	call	_ZN4TempIiE3addEii
	movl	%eax, %esi
	leaq	_ZSt4cout(%rip), %rdi
.LEHB0:
	call	_ZNSolsEi@PLT
	movq	%rax, %rdx
	movq	_ZSt4endlIcSt11char_traitsIcEERSt13basic_ostreamIT_T0_ES6_@GOTPCREL(%rip), %rax
	movq	%rax, %rsi
	movq	%rdx, %rdi
	call	_ZNSolsEPFRSoS_E@PLT
.LEHE0:
	leaq	-25(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN4TempIiED1Ev
	nop
	movq	-24(%rbp), %rax
	xorq	%fs:40, %rax
	je	.L3
	jmp	.L5
.L4:
	movq	%rax, %rbx
	leaq	-25(%rbp), %rax
	movq	%rax, %rdi
	call	_ZN4TempIiED1Ev
	movq	%rbx, %rax
	movq	%rax, %rdi
.LEHB1:
	call	_Unwind_Resume@PLT
.LEHE1:
.L5:
	call	__stack_chk_fail@PLT
.L3:
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1496:
	.globl	__gxx_personality_v0
	.section	.gcc_except_table,"a",@progbits
.LLSDA1496:
	.byte	0xff
	.byte	0xff
	.byte	0x1
	.uleb128 .LLSDACSE1496-.LLSDACSB1496
.LLSDACSB1496:
	.uleb128 .LEHB0-.LFB1496
	.uleb128 .LEHE0-.LEHB0
	.uleb128 .L4-.LFB1496
	.uleb128 0
	.uleb128 .LEHB1-.LFB1496
	.uleb128 .LEHE1-.LEHB1
	.uleb128 0
	.uleb128 0
.LLSDACSE1496:
	.text
	.size	_Z6test01v, .-_Z6test01v
	.globl	main
	.type	main, @function
main:
.LFB1497:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	call	_Z6test01v
	movl	$0, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1497:
	.size	main, .-main
	.section	.text._ZN4TempIiEC2Ev,"axG",@progbits,_ZN4TempIiEC5Ev,comdat
	.align 2
	.weak	_ZN4TempIiEC2Ev
	.type	_ZN4TempIiEC2Ev, @function
_ZN4TempIiEC2Ev:
.LFB1735:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1735:
	.size	_ZN4TempIiEC2Ev, .-_ZN4TempIiEC2Ev
	.weak	_ZN4TempIiEC1Ev
	.set	_ZN4TempIiEC1Ev,_ZN4TempIiEC2Ev
	.section	.text._ZN4TempIiED2Ev,"axG",@progbits,_ZN4TempIiED5Ev,comdat
	.align 2
	.weak	_ZN4TempIiED2Ev
	.type	_ZN4TempIiED2Ev, @function
_ZN4TempIiED2Ev:
.LFB1738:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1738:
	.size	_ZN4TempIiED2Ev, .-_ZN4TempIiED2Ev
	.weak	_ZN4TempIiED1Ev
	.set	_ZN4TempIiED1Ev,_ZN4TempIiED2Ev
	.section	.text._ZN4TempIiE3addEii,"axG",@progbits,_ZN4TempIiE3addEii,comdat
	.align 2
	.weak	_ZN4TempIiE3addEii
	.type	_ZN4TempIiE3addEii, @function
_ZN4TempIiE3addEii:
.LFB1740:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movl	%edx, -16(%rbp)
	movl	-12(%rbp), %edx
	movl	-16(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1740:
	.size	_ZN4TempIiE3addEii, .-_ZN4TempIiE3addEii
	.text
	.type	_Z41__static_initialization_and_destruction_0ii, @function
_Z41__static_initialization_and_destruction_0ii:
.LFB1990:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	%edi, -4(%rbp)
	movl	%esi, -8(%rbp)
	cmpl	$1, -4(%rbp)
	jne	.L14
	cmpl	$65535, -8(%rbp)
	jne	.L14
	leaq	_ZStL8__ioinit(%rip), %rdi
	call	_ZNSt8ios_base4InitC1Ev@PLT
	leaq	__dso_handle(%rip), %rdx
	leaq	_ZStL8__ioinit(%rip), %rsi
	movq	_ZNSt8ios_base4InitD1Ev@GOTPCREL(%rip), %rax
	movq	%rax, %rdi
	call	__cxa_atexit@PLT
.L14:
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1990:
	.size	_Z41__static_initialization_and_destruction_0ii, .-_Z41__static_initialization_and_destruction_0ii
	.type	_GLOBAL__sub_I__Z6test01v, @function
_GLOBAL__sub_I__Z6test01v:
.LFB1991:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movl	$65535, %esi
	movl	$1, %edi
	call	_Z41__static_initialization_and_destruction_0ii
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1991:
	.size	_GLOBAL__sub_I__Z6test01v, .-_GLOBAL__sub_I__Z6test01v
	.section	.init_array,"aw"
	.align 8
	.quad	_GLOBAL__sub_I__Z6test01v
	.hidden	DW.ref.__gxx_personality_v0
	.weak	DW.ref.__gxx_personality_v0
	.section	.data.rel.local.DW.ref.__gxx_personality_v0,"awG",@progbits,DW.ref.__gxx_personality_v0,comdat
	.align 8
	.type	DW.ref.__gxx_personality_v0, @object
	.size	DW.ref.__gxx_personality_v0, 8
DW.ref.__gxx_personality_v0:
	.quad	__gxx_personality_v0
	.hidden	__dso_handle
	.ident	"GCC: (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0"
	.section	.note.GNU-stack,"",@progbits
