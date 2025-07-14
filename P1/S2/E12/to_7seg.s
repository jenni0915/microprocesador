        .section .rodata
.sseg:
        .byte 0x3F,0x06,0x5B,0x4F
        .byte 0x66,0x6D,0x7D,0x07
        .byte 0x7F,0x6F,0x77,0x40
        .byte 0x50,0x54,0x79,0x71

        .section .text
        .balign 4
        .type to_7seg, %function
        .global to_7seg
to_7seg:
        LDR R1, =.sseg   //R1 apunta a sseg[0]
        CMP R0, #0x0F   //if(code < 16)
        BHI .num_0      //si no verifica el if va al .num_0
        LDRB R0,[R1,R0] //verificado el if. Guardar en R0 sseg[0+R0]
        B   .salida     //salir
.num_0:
        MOVS R0, #0x00  //R0=0
        B   .salida
.salida: 
        BX 	LR
        .size to_7seg, . - to_7seg