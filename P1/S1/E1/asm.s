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
          LDR       R0, =0x20000000
          MOVS      R1, #0
          LDRB      R2, [R0, R1]
          MOVS      R1, #4
          LDRB      R3, [R0, R1]
          ANDS      R3, R2
          MOVS      R1, #0x10
          STRB      R3, [R0, R1]
.Lforever:
          B         .Lforever

          .size     Reset_Handler, . - Reset_Handler
