// Don't edit this file, it is machine generated


#ifndef ELIB_Ex86_TAB_H
#define ELIB_Ex86_TAB_H


int Ex86_size[]={
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1
};

int Ex86_time[]={
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1
};

const char *Ex86_name[]={
"add","add","add","add","add","add","push","pop",
"or","or","or","or","or","or","push","special",
"adc","adc","adc","adc","adc","adc","push","pop",
"sbb","sbb","sbb","sbb","sbb","sbb","push","pop",
"and","and","and","and","and","and","es","daa",
"sub","sub","sub","sub","sub","sub","cs","das",
"xor","xor","xor","xor","xor","xor","ss","aaa",
"cmp","cmp","cmp","cmp","cmp","cmp","ds","aas",
"inc","inc","inc","inc","inc","inc","inc","inc",
"special","dec","dec","dec","dec","dec","dec","dec",
"push","push","push","push","push","push","push","push",
"pop","pop","pop","pop","pop","pop","pop","pop",
"pusha","popa","bound","arpl","fs","gs","op_size","addr_size",
"push","imul","push","imul","insb","insw","outsb","outsw",
"jo","jno","jc","jnc","jz","jnz","jbe","jnbe",
"js","jns","jpo","jpe","jnge","jge","jng","jg",
"special","special","illegal","special","test","test","xchg","xchg",
"mov","mov","mov","mov","mov","lea","mov","special",
"xchg","xchg","xchg","xchg","xchg","xchg","xchg","xchg",
"cbw","cwd","call","wait","pushf","popf","sahf","lahf",
"mov","mov","mov","mov","movsb","movsw","cmpsb","cmpsw",
"test","test","stosb","stosw","lodsb","lodsw","scasb","scasw",
"mov","mov","mov","mov","mov","mov","mov","mov",
"mov","mov","mov","mov","mov","mov","mov","mov",
"special","special","ret","ret","les","lds","special","special",
"enter","leave","ret","ret","int","int","into","iret",
"special","special","special","special","special","special","illegal","xlat",
"illegal","illegal","illegal","illegal","illegal","illegal","illegal","illegal",
"loopnz","loopz","loop","jcxz","in","in","out","out",
"call","jmp","jmp","jmp","in","in","out","out",
"lock","illegal","repnz","repz","hlt","cmc","special","special",
"clc","stc","cli","sti","cld","std","special","special"
};

int Ex86_format[]={
Ex86_MODRM,Ex86_MODRM,Ex86_MODREG,Ex86_MODREG,
Ex86_AX,Ex86_AX,Ex86_SEG,Ex86_SEG,
Ex86_MODRM,Ex86_MODRM,Ex86_REG,Ex86_REG,
Ex86_AX,Ex86_AX,Ex86_SEG,Ex86_SPECIAL,
Ex86_MODRM,Ex86_MODRM,Ex86_MODREG,Ex86_MODREG,
Ex86_AX,Ex86_AX,Ex86_SEG,Ex86_SEG,
Ex86_MODRM,Ex86_MODRM,Ex86_MODREG,Ex86_MODREG,
Ex86_AX,Ex86_AX,Ex86_SEG,Ex86_SEG,
Ex86_MODRM,Ex86_MODRM,Ex86_MODREG,Ex86_MODREG,
Ex86_AX,Ex86_AX,Ex86_NONE,Ex86_NONE,
Ex86_MODRM,Ex86_MODRM,Ex86_MODREG,Ex86_MODREG,
Ex86_AX,Ex86_AX,Ex86_NONE,Ex86_NONE,
Ex86_MODRM,Ex86_MODRM,Ex86_MODREG,Ex86_MODREG,
Ex86_AX,Ex86_AX,Ex86_NONE,Ex86_NONE,
Ex86_MODRM,Ex86_MODRM,Ex86_MODREG,Ex86_MODREG,
Ex86_AX,Ex86_AX,Ex86_NONE,Ex86_NONE,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_SPECIAL,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_IMM,Ex86_NONE,Ex86_IMM,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,
Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,
Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,
Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_NONE,Ex86_SPECIAL,
Ex86_MODRM,Ex86_MODRM,Ex86_MODRM,Ex86_MODRM,
Ex86_MODRM8,Ex86_MODRM,Ex86_MODREG8,Ex86_MODREG,
Ex86_MODRM,Ex86_MODREG,Ex86_SEG,Ex86_SPECIAL,
Ex86_AX,Ex86_AX,Ex86_AX,Ex86_AX,
Ex86_AX,Ex86_AX,Ex86_AX,Ex86_AX,
Ex86_NONE,Ex86_NONE,Ex86_BRANCH,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_AX,Ex86_AX,Ex86_DISP,Ex86_DISP,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SPECIAL,Ex86_SPECIAL,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_SPECIAL,Ex86_SPECIAL,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,Ex86_BRANCH,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SPECIAL,Ex86_SPECIAL,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SPECIAL,Ex86_SPECIAL
};

int Ex86_subformat[]={
Ex86_MODREG,Ex86_MODREG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_REG,Ex86_REG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_SPECIAL,
Ex86_MODREG,Ex86_MODREG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_MODREG,Ex86_MODREG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_MODREG,Ex86_MODREG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_MODREG,Ex86_MODREG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_MODREG,Ex86_MODREG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_MODREG,Ex86_MODREG,Ex86_MODRM,Ex86_MODRM,
Ex86_IMM,Ex86_IMM,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SPECIAL,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_NONE,Ex86_SPECIAL,
Ex86_MODREG,Ex86_MODREG,Ex86_REG,Ex86_REG,
Ex86_MODREG8,Ex86_MODREG,Ex86_MODRM8,Ex86_MODRM,
Ex86_SEG,Ex86_MODRM,Ex86_MODRM,Ex86_SPECIAL,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_REG,Ex86_REG,Ex86_REG,Ex86_REG,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_DISP,Ex86_DISP,Ex86_AX,Ex86_AX,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_IMM,Ex86_IMM,Ex86_IMM,Ex86_IMM,
Ex86_IMM,Ex86_IMM,Ex86_IMM,Ex86_IMM,
Ex86_IMM,Ex86_IMM,Ex86_IMM,Ex86_IMM,
Ex86_IMM,Ex86_IMM,Ex86_IMM,Ex86_IMM,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SPECIAL,Ex86_SPECIAL,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_SPECIAL,Ex86_SPECIAL,
Ex86_SPECIAL,Ex86_SPECIAL,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SPECIAL,Ex86_SPECIAL,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SPECIAL,Ex86_SPECIAL
};

int Ex86_flags[]={
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,
Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,
Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,
Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_NONE,Ex86_SIZE_BYTE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,
Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,Ex86_SIZE_BYTE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_BRANCH,Ex86_BRANCH,
Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE,Ex86_BRANCH,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_SIZE_BYTE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_NONE,Ex86_NONE,
Ex86_NONE,Ex86_NONE,Ex86_SIZE_BYTE,Ex86_NONE
};

#endif // ELIB_Ex86_TAB_H