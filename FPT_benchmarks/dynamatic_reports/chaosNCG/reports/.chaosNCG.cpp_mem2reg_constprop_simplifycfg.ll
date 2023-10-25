; ModuleID = '.chaosNCG.cpp_mem2reg_constprop.ll'
source_filename = "src/chaosNCG.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z8chaosNCGPiS_iii(i32* %A, i32* %M, i32 %I, i32 %Y, i32 %X) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.body, %entry
  %i.01 = phi i32 [ 0, %entry ], [ %add30, %for.body ]
  %add = add nsw i32 %I, %i.01
  %add1 = add nsw i32 %add, 2
  %idxprom = sext i32 %add1 to i64
  %arrayidx = getelementptr inbounds i32, i32* %M, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %add2 = add nsw i32 %I, %i.01
  %sub = sub nsw i32 %add2, 2
  %idxprom3 = sext i32 %sub to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %M, i64 %idxprom3
  %1 = load i32, i32* %arrayidx4, align 4
  %idxprom5 = sext i32 %0 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %A, i64 %idxprom5
  %2 = load i32, i32* %arrayidx6, align 4
  %idxprom7 = sext i32 %1 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %A, i64 %idxprom7
  %3 = load i32, i32* %arrayidx8, align 4
  %xor = xor i32 %2, %3
  %and = and i32 %xor, 15
  %shl = shl i32 %X, %and
  %and9 = and i32 %xor, 15
  %sub10 = sub nsw i32 16, %and9
  %shr = ashr i32 %X, %sub10
  %or = or i32 %shl, %shr
  %xor11 = xor i32 %xor, %or
  %xor12 = xor i32 %3, %xor11
  %and13 = and i32 %xor12, 15
  %shl14 = shl i32 %Y, %and13
  %and15 = and i32 %xor12, 15
  %sub16 = sub nsw i32 16, %and15
  %shr17 = ashr i32 %Y, %sub16
  %or18 = or i32 %shl14, %shr17
  %add19 = add nsw i32 %xor12, %or18
  %add20 = add nsw i32 %i.01, 0
  %idxprom21 = sext i32 %add20 to i64
  %arrayidx22 = getelementptr inbounds i32, i32* %M, i64 %idxprom21
  %4 = load i32, i32* %arrayidx22, align 4
  %idxprom23 = sext i32 %4 to i64
  %arrayidx24 = getelementptr inbounds i32, i32* %A, i64 %idxprom23
  store i32 %xor11, i32* %arrayidx24, align 4
  %add25 = add nsw i32 %i.01, 1
  %idxprom26 = sext i32 %add25 to i64
  %arrayidx27 = getelementptr inbounds i32, i32* %M, i64 %idxprom26
  %5 = load i32, i32* %arrayidx27, align 4
  %idxprom28 = sext i32 %5 to i64
  %arrayidx29 = getelementptr inbounds i32, i32* %A, i64 %idxprom28
  store i32 %add19, i32* %arrayidx29, align 4
  %add30 = add nsw i32 %i.01, 2
  %cmp = icmp slt i32 %add30, 2000
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.body
  %i.0.lcssa = phi i32 [ %add30, %for.body ]
  ret i32 %i.0.lcssa
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %M = alloca [1 x [3000 x i32]], align 16
  %A = alloca [1 x [3000 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %for.inc10, %entry
  %i.02 = phi i32 [ 0, %entry ], [ %inc11, %for.inc10 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %idxprom = sext i32 %i.02 to i64
  %arrayidx = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %M, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %j.01 to i64
  %arrayidx5 = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %j.01, i32* %arrayidx5, align 4
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %idxprom6 = sext i32 %i.02 to i64
  %arrayidx7 = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %A, i64 0, i64 %idxprom6
  %idxprom8 = sext i32 %j.01 to i64
  %arrayidx9 = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 %rem, i32* %arrayidx9, align 4
  %inc = add nsw i32 %j.01, 1
  %cmp2 = icmp slt i32 %inc, 3000
  br i1 %cmp2, label %for.body3, label %for.inc10

for.inc10:                                        ; preds = %for.body3
  %inc11 = add nsw i32 %i.02, 1
  %cmp = icmp slt i32 %inc11, 1
  br i1 %cmp, label %for.body, label %for.end12

for.end12:                                        ; preds = %for.inc10
  %arrayidx15 = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %A, i64 0, i64 0
  %arraydecay = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx15, i32 0, i32 0
  %arrayidx17 = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %M, i64 0, i64 0
  %arraydecay18 = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx17, i32 0, i32 0
  %call19 = call i32 @_Z8chaosNCGPiS_iii(i32* %arraydecay, i32* %arraydecay18, i32 2, i32 1, i32 1)
  ret i32 0
}

; Function Attrs: nounwind
declare void @srand(i32) #2

; Function Attrs: nounwind
declare i32 @rand() #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
