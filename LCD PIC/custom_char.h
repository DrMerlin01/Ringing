/* 
 * File:   custom_char.h
 * Author: Avinash
 *
 * Created on 8 June, 2013, 12:23 PM
 */

#ifndef CUSTOM_CHAR_H
#define	CUSTOM_CHAR_H

#ifdef	__cplusplus
extern "C" {
#endif
#define _XTAL_FREQ 4000000
unsigned char __cgram[]=
{
	0x0C, 0x12, 0x12, 0x0C, 0x00, 0x00, 0x00, 0x00, //Char0 dergee symbol
	0x00, 0x04, 0x0E, 0x1F, 0x0E, 0x04, 0x00, 0x00, //Char1
	0x04, 0x04, 0x04, 0x04, 0x1F, 0x0E, 0x04, 0x00, //Char2
	0x0A, 0x15, 0x11, 0x0A, 0x04, 0x00, 0x00, 0x00, //Char3
	0x0A, 0x1F, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00, //Char4
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char5
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char6
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, //Char7
};


#ifdef	__cplusplus
}
#endif

#endif	/* CUSTOM_CHAR_H */
