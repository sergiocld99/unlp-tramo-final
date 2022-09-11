INICIO_P2:  LOAD ACUM, MEM_X    ;; 2.1
            CMP ACUM, 0         
            JNS FIN_P2          

            LOAD ACUM, MEM_X    ;; 2.2.1
            ADD ACUM, 1         ;; 2.2.2
            STORE MEM_X, ACUM   ;; 2.2.3

FIN_P2:     HALT