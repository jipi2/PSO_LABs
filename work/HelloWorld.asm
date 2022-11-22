BITS 32
          global    _start
 
          section   .text
_start:   mov       eax, 0x04         ; stocam in eax valoarea 4
          mov       ebx, 1
          mov       ecx, message     ; stocam in esi adresa lui message
          mov       edx, 13          ; lungimea mesajului
          int 0x80
          mov       eax, 0x1
          xor       ebx, ebx
          int 0x80
 
          section   .data
message:  db        "Hello, World", 10  ; 10 = newline
