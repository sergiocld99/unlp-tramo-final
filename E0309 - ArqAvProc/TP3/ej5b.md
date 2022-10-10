![](img10.png)

            ADD R1, R0, 3832
    
    Loop:   LDD RF0, (R1)           ;; cargo
            LDD RF1, -8(R1)         
            ADDF RF4, RF0, RF2      ;; sumo s
            ADDF RF5, RF1, RF2
            STD (R1), RF4           ;; guardo
            STD -8(R1), RF5

            SUB R1, R1, 16           ;; i -=2
            BGEZ R1, Loop

Para VLIW:

    C   MEM     MEM     PF      PF      INT
    1   LDD     LDD
    2   LDD     LDD
    3                   ADDF    ADDF
    4                   ADDF    ADDF
    5                   ADDF    ADDF    SUB
    6   STD     STD                     BGEZ


Se completan 2 iteraciones en 6 ciclos. S = 2