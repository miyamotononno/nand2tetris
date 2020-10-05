// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

(INIT)
    // キーボードの状態を見て、塗りつぶす色を選択
    @KBD
    D=M // データメモリにKBDのアドレスに入っている値を代入
    @IF
    D;JNE // 0以外なら(keyboard操作がなされたら)IFに飛ぶ
    @ELSE
    0;JMP // それ以外ならELSEに飛ぶ
(IF)
    @color
    M=-1   // colorアドレスに1を代入(黒くする) 
    @ENDIF  
    0;JMP // IF文終了
(ELSE)
    @color
    M=0      // colorアドレスに0を代入(白くする)
(ENDIF)
    @SCREEN
    D=A      // D <- SCREENのアドレス
    @addr    // 塗りつぶしたいSCREEN上のワードを表す
    M=D      // "addr"というアドレスにSCREENアドレスを入れる
(LOOP)       //塗りつぶすループ
    @color
    D=M      // データメモリにcolorアドレスに入っているものを代入
    @addr
    A=M // addrをAレジスタに入れる
    M=D      // addr <- color
    @addr
    M=M+1

    @8192
    D=A
    @SCREEN
    D=D+A
    @addr
    D=D-M  // D= 8192 + SCREEN - addr
    // 塗り残しがあれば塗りつぶすまで続ける
    @LOOP
    D;JGT

    // 全部塗り終わったら、最初に戻る
    @INIT
    0;JMP
