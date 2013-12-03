/*
 * purgatory:  setup code
 *
 * Copyright (C) 2003,2004  Eric Biederman (ebiederm@xmission.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation (version 2 of the License).
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */


#undef i386

	.text
	.globl dummy_purgatory_start, hack_address, hack_value
	.balign 16
dummy_purgatory_start:
	.code64

	leaq hack_address(%rip), %rax
	movq (%rax), %rax
	leaq hack_value(%rip), %rbx
	movq (%rbx), %rbx
	movb %bl, (%rax)
	xor %rax, %rax
	xor %rbx, %rbx	
	ret

	.data
	.balign 4
hack_address:	.quad 0x00000000
		.size hack_address, . - hack_address
hack_value:	.quad 0x00000000
		.size hack_value, . - hack_value

