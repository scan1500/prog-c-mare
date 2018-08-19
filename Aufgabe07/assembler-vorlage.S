        .global _start

        .text
_start:
        popq    %rax    // Beim Start steht die Anzahl der 
                        // auf der Kommandozeile uebergebenen
                        // Parameter auf dem Stack; popq %rax
                        // kopiert den Wert in Register rax

        // hier geht es mit selbst geschriebenem
        // Assemblercode weiter...

        // exit(0)
        mov     $60, %rax               // Systemaufruf #60 ist "exit"
        xor     %rdi, %rdi              // Rueckgabewert 0
                                        // Achtung: Aufgabenstellung verlangt
                                        // anderen Rueckgabewert!
        syscall                         // Systemaufruf ausfuehren

