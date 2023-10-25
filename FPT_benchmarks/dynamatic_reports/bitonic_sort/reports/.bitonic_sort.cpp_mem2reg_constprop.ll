; ModuleID = '.bitonic_sort.cpp_mem2reg.ll'
source_filename = "src/bitonic_sort.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z12bitonic_sortPi(i32* %A) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc29
  %k.04 = phi i32 [ 2, %entry ], [ %shl, %for.inc29 ]
  %shr = ashr i32 %k.04, 1
  %cmp22 = icmp sgt i32 %shr, 0
  br i1 %cmp22, label %for.body3.lr.ph, label %for.end28

for.body3.lr.ph:                                  ; preds = %for.body
  br label %for.body3

for.body3:                                        ; preds = %for.body3.lr.ph, %for.inc26
  %j.03 = phi i32 [ %shr, %for.body3.lr.ph ], [ %shr27, %for.inc26 ]
  br label %for.body6

for.body6:                                        ; preds = %for.body3, %for.inc
  %i.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %xor = xor i32 %i.01, %j.03
  %idxprom = sext i32 %i.01 to i64
  %arrayidx = getelementptr inbounds i32, i32* %A, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom7 = sext i32 %xor to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %A, i64 %idxprom7
  %1 = load i32, i32* %arrayidx8, align 4
  %cmp9 = icmp sgt i32 %xor, %i.01
  br i1 %cmp9, label %if.then, label %if.end25

if.then:                                          ; preds = %for.body6
  %and = and i32 %i.01, %k.04
  %cmp10 = icmp eq i32 %and, 0
  br i1 %cmp10, label %if.then11, label %if.else

if.then11:                                        ; preds = %if.then
  %cmp12 = icmp sgt i32 %0, %1
  br i1 %cmp12, label %if.then13, label %if.end

if.then13:                                        ; preds = %if.then11
  br label %if.end

if.end:                                           ; preds = %if.then13, %if.then11
  %toSwap.0 = phi i32 [ 1, %if.then13 ], [ 0, %if.then11 ]
  br label %if.end17

if.else:                                          ; preds = %if.then
  %cmp14 = icmp slt i32 %0, %1
  br i1 %cmp14, label %if.then15, label %if.end16

if.then15:                                        ; preds = %if.else
  br label %if.end16

if.end16:                                         ; preds = %if.then15, %if.else
  %toSwap.1 = phi i32 [ 1, %if.then15 ], [ 0, %if.else ]
  br label %if.end17

if.end17:                                         ; preds = %if.end16, %if.end
  %toSwap.2 = phi i32 [ %toSwap.0, %if.end ], [ %toSwap.1, %if.end16 ]
  %cmp18 = icmp eq i32 %toSwap.2, 1
  br i1 %cmp18, label %if.then19, label %if.end24

if.then19:                                        ; preds = %if.end17
  %idxprom20 = sext i32 %i.01 to i64
  %arrayidx21 = getelementptr inbounds i32, i32* %A, i64 %idxprom20
  store i32 %1, i32* %arrayidx21, align 4
  %idxprom22 = sext i32 %xor to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %A, i64 %idxprom22
  store i32 %0, i32* %arrayidx23, align 4
  br label %if.end24

if.end24:                                         ; preds = %if.then19, %if.end17
  br label %if.end25

if.end25:                                         ; preds = %if.end24, %for.body6
  br label %for.inc

for.inc:                                          ; preds = %if.end25
  %inc = add nsw i32 %i.01, 1
  %cmp5 = icmp slt i32 %inc, 64
  br i1 %cmp5, label %for.body6, label %for.end

for.end:                                          ; preds = %for.inc
  br label %for.inc26

for.inc26:                                        ; preds = %for.end
  %shr27 = ashr i32 %j.03, 1
  %cmp2 = icmp sgt i32 %shr27, 0
  br i1 %cmp2, label %for.body3, label %for.cond1.for.end28_crit_edge

for.cond1.for.end28_crit_edge:                    ; preds = %for.inc26
  br label %for.end28

for.end28:                                        ; preds = %for.cond1.for.end28_crit_edge, %for.body
  br label %for.inc29

for.inc29:                                        ; preds = %for.end28
  %shl = shl i32 %k.04, 1
  %cmp = icmp sle i32 %shl, 64
  br i1 %cmp, label %for.body, label %for.end30

for.end30:                                        ; preds = %for.inc29
  %k.0.lcssa = phi i32 [ %shl, %for.inc29 ]
  ret i32 %k.0.lcssa
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %A = alloca [1 x [64 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc6
  %i.02 = phi i32 [ 0, %entry ], [ %inc7, %for.inc6 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %idxprom = sext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds [1 x [64 x i32]], [1 x [64 x i32]]* %A, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %j.01 to i64
  %arrayidx5 = getelementptr inbounds [64 x i32], [64 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %j.01, i32* %arrayidx5, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.01, 1
  %cmp2 = icmp slt i32 %inc, 64
  br i1 %cmp2, label %for.body3, label %for.end

for.end:                                          ; preds = %for.inc
  br label %for.inc6

for.inc6:                                         ; preds = %for.end
  %inc7 = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc7, 1
  br i1 %cmp, label %for.body, label %for.end8

for.end8:                                         ; preds = %for.inc6
  %arrayidx11 = getelementptr inbounds [1 x [64 x i32]], [1 x [64 x i32]]* %A, i64 0, i64 0
  %arraydecay = getelementptr inbounds [64 x i32], [64 x i32]* %arrayidx11, i32 0, i32 0
  %call = call i32 @_Z12bitonic_sortPi(i32* %arraydecay)
  ret i32 0
}

; Function Attrs: nounwind
declare void @srand(i32) #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
