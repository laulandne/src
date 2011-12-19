// Don't edit this file, it is machine generated


#ifndef ELIB_ESh_TAB_H
#define ELIB_ESh_TAB_H


int ESh_size[]={
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
};

int ESh_time[]={
1,1,1,1,1,1,1,1,
1,1,1,1,1,1,1,1,
};

const char *ESh_name[]={
"special0","mov_l","mov_l","illegal","special4","mov","mov","add",
"bcc","mov_w","illegal","bra","bsr","mov_l","mov","illegal",
};

int ESh_format[]={
ESH_SPECIAL,ESH_REG_DISPREG4,ESH_MOV,ECPU_ADDR_NONE,
ESH_SPECIAL,ESH_DISPREG4_REG,ESH_MOV,ESH_IMM_REG,
ESH_DISPPC,ESH_DISPPC2_REG,ECPU_ADDR_NONE,ESH_DISPPC,
ESH_DISPPC,ESH_DISPPC4_REG,ESH_IMM_REG,ECPU_ADDR_NONE,
};

int ESh_subformat[]={
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
};

int ESh_flags[]={
false,false,false,false,
false,false,false,false,
false,false,false,false,
false,false,false,false,
};

#endif // ELIB_ESh_TAB_H
