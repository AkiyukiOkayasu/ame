# nxplib  
i.MXRT10xx用のヘッダーオンリーDSPライブラリ  
constexprなどのC++14の機能を使用したより安全で依存の少ないライブラリを目指しています  

## dependencies  
- C++14 on newlib-nano (or later)  
- [ARM CMSIS-DSP](https://arm-software.github.io/CMSIS_5/DSP/html/index.html)  

## TODO  
- buffer作成（モノ、インターリーブ）と各種変換関数
- Single writer/reader FIFO  
- WAV writer/reader
- MIDI
- CMSIS-DSPを使ったサンプルレートコンバーター
- EQ ([RBJ cookbook](https://www.g200kg.com/jp/docs/makingeffects/78743dea3f70c8c2f081b7d5187402ec75e6a6b8.html))の実装 
