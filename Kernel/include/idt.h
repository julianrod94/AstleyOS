#ifndef _IDT_H_
#define _IDT_H_

#include <kdefine.h>

#define _IDT_ENTRIES 256
#define _GDT_CODE_SEGMENT 0x08

// Modified from: Arquitectura de las Computadoras - TP6 @ ITBA
#define _IDT_ACCESS_PRESENT     	0x80            // present segment at memory
#define _IDT_ACCESS_CODE_SEGMENT    0x18            // code segment
#define _IDT_ACCESS_DATA_SEGMENT    0x10            // data segment
#define _IDT_ACCESS_READ        	0x02            // read segment
#define _IDT_ACCESS_WRITE       	0x02            // write segment
#define _IDT_ACCESS_IDT         	_IDT_ACCESS_DATA_SEGMENT
#define _IDT_ACCESS_INT_IA_32e 		0x0E			// Interrupt GATE 64 bits // TODO:
#define _IDT_ACCESS_INT         	( _IDT_ACCESS_PRESENT | _IDT_ACCESS_INT_IA_32e ) // DPL == 0x00 // TODO:

#define _IDT_ACCESS_CODE        	(_IDT_ACCESS_PRESENT | _IDT_ACCESS_CODE_SEGMENT | _IDT_ACCESS_READ)
#define _IDT_ACCESS_DATA        	(_IDT_ACCESS_PRESENT | _IDT_ACCESS_DATA_SEGMENT | _IDT_ACCESS_WRITE)
#define _IDT_ACCESS_STACK       	(_IDT_ACCESS_PRESENT | _IDT_ACCESS_DATA_SEGMENT | _IDT_ACCESS_WRITE)

#define _IDT_ENTRY_EXCEPTION(NUMBER, PERMISSION) idt_entry(0x ## NUMBER, (qword_t) &exception_ ## NUMBER, PERMISSION)
#define _IDT_ENTRY_INTERRUPT(NUMBER, PERMISSION) idt_entry(0x ## NUMBER, (qword_t) &_interrupt_ ## NUMBER, PERMISSION)

int idt_init(void);

/**
 * Sets a new IDT entry at the given index, with the corresponding pointer to
 * the interrupt service routine and with the detailed access permissions.
 * @param index  -  the index of the new IDT entry, relative to
 *               	the beginning of the IDT
 * @param offset - the pointer to the interrupt service routine
 * @param access - the permissions for the entry being set
 */
int idt_entry(unsigned int index, qword_t offset, byte_t access);

#endif