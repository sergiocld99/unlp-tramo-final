INICIO_P3:  LOAD R0, MEM_X      ;; 3.1.1
            MUL R0, 3
            LOAD R1, MEM_X      ;; 3.1.2
            MUL R1, 2
            ADD ACUM, R0, R1    ;; 3.1.3
            ADD ACUM, 1         ;; 3.1.4
            STORE MEM_X, ACUM   ;; 3.1.5
            HALT