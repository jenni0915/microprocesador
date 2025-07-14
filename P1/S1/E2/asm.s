          .section  .isr_vector
.Lvectors:
          .word     Image$$STACK$$ZI$$Limit     // initial SP, symbol from the scatter file
          .word     Reset_Handler               // reset vector
          // in a Cortex-M0 there are as much as 15 + 32 vectors + initial SP = 48
          //    entries in the vector table. Fill the remaining with 0
          .fill     (48 - (. - .Lvectors) / 4), 4, 0

          .section  .text
          .balign   4
          .global   Reset_Handler
          .type     Reset_Handler, %function

Reset_Handler:
          LDR       R0, =0x20000008
          MOVS      R1, #8
          EORS      R2,R2

.Lloop:
          SUBS      R1,#1
          BMI       .Lexit
          LDRB      R3,[R0,R1]
          LSRS      R3,#1
          BCC       .Lloop
          ADDS      R2,#1
          B         .Lloop
          
.Lexit:
          MOVS      R1,#0x08
          STRB      R2,[R0,R1]
          
.Lforever:
          B         .Lforever

          .size     Reset_Handler, . - Reset_Handler
