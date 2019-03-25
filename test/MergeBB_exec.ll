; RUN: %clang -S -emit-llvm %S/../inputs/input_for_mba.c -o - \
; RUN:   | opt --enable-new-pm=0 -load  %shlibdir/libMergeBB%shlibext -legacy-merge-bb -S -o %t.ll
; RUN: %clang %t.ll -o %t.bin

; Verify that after applying MergeBB the output generated by the input
; module (input_for_mba.c) doesn't change.

; The program implemented in input_for_mba.c takes for inputs and adds them up,
; and returns the result. So if they add up to 0, then the binary returns `0`
; (aka success). Verify that the obfuscation didn't violate this invariant.
; RUN: %t.bin 0 0 0 0
; RUN: %t.bin 1 2 3 -6
; RUN: %t.bin -13 13 -13 13
; RUN: %t.bin -11100 100 1000 10000

; If the input values don't add up to 0, then the result shouldn't add to `0`.
; Use `not` to negate the result so that we still test for `success`.
; RUN: not %t.bin 0 0 0 1
; RUN: not %t.bin 1 2 3 -7
; RUN: not %t.bin 13 13 -13 13
; RUN: not %t.bin -11101 100 1000 10000
