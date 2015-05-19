***********************************************************************
*                                                                     *
*                 組語 Final Hw - SIC/XE Assembler                    *
*                                                                     *
*                      7102056032 顏至寬                              *
*                                                                     *
*                          version.3                                  *
*                                                                     *
***********************************************************************

支援功能 : 
1. 2-PASS Assembler
2. SIC/XE 全指令 support
3. Support Relocation

執行環境 : 
windows XP 以上

Assembler程式編譯環境 :
windows/Dev C++ 5.6.3/C

使用方法 : 
將assembler_v3.exe 放置於source code同個資料夾中執行，
產生出SIC/XE 的excutable file.

注意事項 : 
1. source code 需命名為 source_code.txt。
2. 產生出的executable file的檔名為 object_file.txt (為了方便demo和debug)
3. 若要自行compile，需要link至operations.h這個header檔
4. 內部提供兩個debug function，print_SYMTAB(印出Symbol)、print_Status(印出instruction個欄位之狀態)

預計下個版本增加 :
1. 讀取檔案優化


