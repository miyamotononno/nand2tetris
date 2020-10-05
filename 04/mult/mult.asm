// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Mult.asm

// Multiplies R0 and R1 and stores the result in R2.
// (R0, R1, R2 refer to RAM[0], RAM[1], and RAM[2], respectively.)

// Put your code here.

    @R2
    M=0 // R2=0
(LOOP)
    @R1
    D=M // R1の値をデータメモリにセットする
    @END
    D;JLE // データメモリが0未満でENDへ移動。この場合はR1
    @R0
    D=M // R0の値をデータメモリにセットする
    @R2
    M=D+M // R2アドレスにデータメモリ(この場合R0)とR2アドレス自身を足す
    @R1
    M=M-1 // R1アドレスに-1したものをR1アドレスに入れる
    @LOOP
    0;JMP // LOOPアドレスへ移動
(END)
    @END
    0; JMP // 無限ループ。終わりを意味する

  