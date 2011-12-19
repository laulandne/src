// Don't edit this file, it is machine generated


#ifndef ELIB_Em65c02_TAB_H
#define ELIB_Em65c02_TAB_H


int Em65c02_size[]={
1,2,0,0,2,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,2,2,2,0,
1,3,1,0,3,3,3,0,
3,2,0,0,2,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,2,2,2,0,
1,3,1,0,3,3,3,0,
1,2,0,0,0,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,0,2,2,0,
1,3,1,0,0,3,3,0,
1,2,0,0,2,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,2,2,2,0,
1,3,1,0,3,3,3,0,
2,2,0,0,2,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,2,2,2,0,
1,3,1,0,3,3,3,0,
2,2,2,0,2,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,2,2,2,0,
1,3,1,0,3,3,3,0,
2,2,0,0,2,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,0,2,2,0,
1,3,1,0,0,3,3,0,
2,2,0,0,2,2,2,0,
1,2,1,0,3,3,3,0,
2,2,2,0,0,2,2,0,
1,3,1,0,0,3,3,0
};

int Em65c02_time[]={
7,2,0,0,3,3,5,0,
3,4,2,0,4,4,6,0,
2,4,4,0,3,4,6,0,
2,5,2,0,4,6,7,0,
6,4,0,0,3,3,5,0,
4,2,2,0,4,4,6,0,
2,4,4,0,4,4,6,0,
2,5,2,0,6,6,7,0,
6,4,0,0,0,3,5,0,
3,2,2,0,3,4,6,0,
2,4,4,0,0,4,6,0,
2,5,3,0,0,6,7,0,
6,4,0,0,3,3,5,0,
4,2,2,0,5,4,6,0,
2,4,4,0,4,4,6,0,
2,5,4,0,6,6,7,0,
2,6,0,0,3,3,3,0,
2,2,2,0,4,4,4,0,
2,6,4,0,4,4,4,0,
2,5,2,0,6,5,7,0,
2,4,2,0,3,3,3,0,
2,2,2,0,4,4,4,0,
2,4,4,0,4,4,4,0,
2,5,2,0,4,5,4,0,
2,6,0,0,3,3,5,0,
2,2,2,0,4,4,6,0,
2,5,4,0,0,4,6,0,
2,4,3,0,0,4,7,0,
2,6,0,0,3,3,5,0,
2,2,2,0,4,4,6,0,
2,5,4,0,0,4,6,0,
2,4,4,0,0,4,7,0
};

const char *Em65c02_name[]={
"brk","ora","illegal","illegal","tsb","ora","asl","illegal",
"php","ora","asl a","illegal","tsb","ora","asl","illegal",
"bpl","ora","ora","illegal","trb","ora","asl","illegal",
"clc","ora","ina","illegal","trb","ora","asl","illegal",
"jsr","and","illegal","illegal","bit","and","rol","illegal",
"plp","and","rol a","illegal","bit","and","rol","illegal",
"bmi","and","and","illegal","bit","and","rol","illegal",
"sec","and","dea","illegal","bit","and","rol","illegal",
"rti","eor","illegal","illegal","illegal","eor","lsr","illegal",
"pha","eor","lsr a","illegal","jmp","eor","lsr","illegal",
"bvc","eor","eor","illegal","illegal","eor","lsr","illegal",
"cli","eor","phy","illegal","illegal","eor","lsr","illegal",
"rts","adc","illegal","illegal","stz","adc","ror","illegal",
"pla","adc","ror a","illegal","jmp","adc","ror","illegal",
"bvs","adc","adc","illegal","stz","adc","ror","illegal",
"sei","adc","ply","illegal","jmp","adc","ror","illegal",
"bra","sta","illegal","illegal","sty","sta","stx","illegal",
"dey","bit","txa","illegal","sty","sta","stx","illegal",
"bcc","sta","sta","illegal","sty","sta","stx","illegal",
"tya","sta","txs","illegal","stz","sta","stz","illegal",
"ldy","lda","ldx","illegal","ldy","lda","ldx","illegal",
"tay","lda","tax","illegal","ldy","lda","ldx","illegal",
"bcs","lda","lda","illegal","ldy","lda","ldx","illegal",
"clv","lda","tsx","illegal","ldy","lda","ldx","illegal",
"cpy","cmp","illegal","illegal","cpy","cmp","dec","illegal",
"iny","cmp","dex","illegal","cpy","cmp","dec","illegal",
"bne","cmp","cmp","illegal","illegal","cmp","dec","illegal",
"cld","cmp","phx","illegal","illegal","cmp","dec","illegal",
"cpx","sbc","illegal","illegal","cpx","sbc","inc","illegal",
"inx","sbc","nop","illegal","cpx","sbc","inc","illegal",
"beq","sbc","sbc","illegal","illegal","sbc","inc","illegal",
"sed","sbc","plx","illegal","illegal","sbc","inc","illegal"
};

int Em65c02_format[]={
E6502_NONE,E6502_INDX,E6502_NONE,E6502_NONE,
E6502_ZP,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_ZP,E6502_ZPX,E6502_ZPX,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABSX,E6502_ABSX,E6502_NONE,
E6502_JABS,E6502_INDX,E6502_NONE,E6502_NONE,
E6502_ZP,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_ZPX,E6502_ZPX,E6502_ZPX,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_ABSX,E6502_ABSX,E6502_ABSX,E6502_NONE,
E6502_NONE,E6502_INDX,E6502_NONE,E6502_NONE,
E6502_NONE,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_JABS,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_NONE,E6502_ZPX,E6502_ZPX,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_NONE,E6502_ABSX,E6502_ABSX,E6502_NONE,
E6502_NONE,E6502_INDX,E6502_NONE,E6502_NONE,
E6502_ZP,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_JIND,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_ZPX,E6502_ZPX,E6502_ZPX,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_ABSINDX,E6502_ABSX,E6502_ABSX,E6502_NONE,
E6502_BXX,E6502_INDX,E6502_NONE,E6502_NONE,
E6502_ZP,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_ZPX,E6502_ZPX,E6502_ZPY,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABSX,E6502_ABSX,E6502_NONE,
E6502_IMM,E6502_INDX,E6502_IMM,E6502_NONE,
E6502_ZP,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_ZPX,E6502_ZPX,E6502_ZPX,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_ABSX,E6502_ABSX,E6502_ABSY,E6502_NONE,
E6502_IMM,E6502_INDX,E6502_NONE,E6502_NONE,
E6502_ZP,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_NONE,E6502_ZPX,E6502_ZPX,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_NONE,E6502_ABSX,E6502_ABSX,E6502_NONE,
E6502_IMM,E6502_INDX,E6502_NONE,E6502_NONE,
E6502_ZP,E6502_ZP,E6502_ZP,E6502_NONE,
E6502_NONE,E6502_IMM,E6502_NONE,E6502_NONE,
E6502_ABS,E6502_ABS,E6502_ABS,E6502_NONE,
E6502_BXX,E6502_INDY,E6502_ZPZ,E6502_NONE,
E6502_NONE,E6502_ZPX,E6502_ZPX,E6502_NONE,
E6502_NONE,E6502_ABSY,E6502_NONE,E6502_NONE,
E6502_NONE,E6502_ABSX,E6502_ABSX,E6502_NONE
};

int Em65c02_subformat[]={
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false
};

int Em65c02_flags[]={
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false
};

#endif // ELIB_Em65c02_TAB_H
