INICIO_P1:  LOAD ACUM, MEM_X        ;; 1.1
            CMP ACUM, 0             
            JNZ FIN_P1              

            STORE MEM_Y, 8          ;; 1.2
            LOAD ACUM, MEM_Y        ;; 1.3.1
            ADD ACUM, 2             ;; 1.3.2
            STORE MEM_X, ACUM       ;; 1.3.3

FIN_P1:     HALT