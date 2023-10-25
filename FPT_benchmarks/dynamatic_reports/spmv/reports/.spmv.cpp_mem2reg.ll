; ModuleID = '.spmv.cpp.ll'
source_filename = "src/spmv.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z4spmvPiiS_S_(i32* %A, i32 %alpha, i32* %row, i32* %col) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc11, %entry
  %k.0 = phi i32 [ 1, %entry ], [ %inc12, %for.inc11 ]
  %ptr.0 = phi i32 [ 0, %entry ], [ %ptr.1, %for.inc11 ]
  %cmp = icmp slt i32 %k.0, 20
  br i1 %cmp, label %for.body, label %for.end13

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %ptr.1 = phi i32 [ %ptr.0, %for.body ], [ %inc, %for.inc ]
  %p.0 = phi i32 [ 0, %for.body ], [ %inc10, %for.inc ]
  %cmp2 = icmp slt i32 %p.0, 20
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %idxprom = sext i32 %ptr.1 to i64
  %arrayidx = getelementptr inbounds i32, i32* %col, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom4 = sext i32 %0 to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %A, i64 %idxprom4
  %1 = load i32, i32* %arrayidx5, align 4
  %mul = mul nsw i32 %alpha, %1
  %idxprom6 = sext i32 %ptr.1 to i64
  %arrayidx7 = getelementptr inbounds i32, i32* %row, i64 %idxprom6
  %2 = load i32, i32* %arrayidx7, align 4
  %idxprom8 = sext i32 %2 to i64
  %arrayidx9 = getelementptr inbounds i32, i32* %A, i64 %idxprom8
  %3 = load i32, i32* %arrayidx9, align 4
  %add = add nsw i32 %3, %mul
  store i32 %add, i32* %arrayidx9, align 4
  %inc = add nsw i32 %ptr.1, 1
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc10 = add nsw i32 %p.0, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc11

for.inc11:                                        ; preds = %for.end
  %inc12 = add nsw i32 %k.0, 1
  br label %for.cond

for.end13:                                        ; preds = %for.cond
  ret i32 %k.0
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %alpha = alloca [1 x i32], align 4
  %row = alloca [1 x [400 x i32]], align 16
  %col = alloca [1 x [400 x i32]], align 16
  %A = alloca [1 x [400 x i32]], align 16
  br label %for.cond

for.cond:                                         ; preds = %for.inc26, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc27, %for.inc26 ]
  %cmp = icmp slt i32 %i.0, 1
  br i1 %cmp, label %for.body, label %for.end28

for.body:                                         ; preds = %for.cond
  %call = call i32 @rand() #3
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %alpha, i64 0, i64 %idxprom
  store i32 %call, i32* %arrayidx, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc23, %for.body
  %y.0 = phi i32 [ 0, %for.body ], [ %inc24, %for.inc23 ]
  %ptr.0 = phi i32 [ 0, %for.body ], [ %ptr.1, %for.inc23 ]
  %cmp2 = icmp slt i32 %y.0, 20
  br i1 %cmp2, label %for.body3, label %for.end25

for.body3:                                        ; preds = %for.cond1
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %ptr.1 = phi i32 [ %ptr.0, %for.body3 ], [ %inc, %for.inc ]
  %x.0 = phi i32 [ 0, %for.body3 ], [ %inc22, %for.inc ]
  %cmp5 = icmp slt i32 %x.0, 20
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %call7 = call i32 @rand() #3
  %rem = srem i32 %call7, 100
  %idxprom8 = sext i32 %i.0 to i64
  %arrayidx9 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %A, i64 0, i64 %idxprom8
  %idxprom10 = sext i32 %ptr.1 to i64
  %arrayidx11 = getelementptr inbounds [400 x i32], [400 x i32]* %arrayidx9, i64 0, i64 %idxprom10
  store i32 %rem, i32* %arrayidx11, align 4
  %rem12 = srem i32 %ptr.1, 3
  %idxprom13 = sext i32 %i.0 to i64
  %arrayidx14 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %row, i64 0, i64 %idxprom13
  %idxprom15 = sext i32 %ptr.1 to i64
  %arrayidx16 = getelementptr inbounds [400 x i32], [400 x i32]* %arrayidx14, i64 0, i64 %idxprom15
  store i32 %rem12, i32* %arrayidx16, align 4
  %add = add nsw i32 %ptr.1, 1
  %rem17 = srem i32 %add, 3
  %idxprom18 = sext i32 %i.0 to i64
  %arrayidx19 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %col, i64 0, i64 %idxprom18
  %idxprom20 = sext i32 %ptr.1 to i64
  %arrayidx21 = getelementptr inbounds [400 x i32], [400 x i32]* %arrayidx19, i64 0, i64 %idxprom20
  store i32 %rem17, i32* %arrayidx21, align 4
  %inc = add nsw i32 %ptr.1, 1
  br label %for.inc

for.inc:                                          ; preds = %for.body6
  %inc22 = add nsw i32 %x.0, 1
  br label %for.cond4

for.end:                                          ; preds = %for.cond4
  br label %for.inc23

for.inc23:                                        ; preds = %for.end
  %inc24 = add nsw i32 %y.0, 1
  br label %for.cond1

for.end25:                                        ; preds = %for.cond1
  br label %for.inc26

for.inc26:                                        ; preds = %for.end25
  %inc27 = add nsw i32 %i.0, 1
  br label %for.cond

for.end28:                                        ; preds = %for.cond
  %arrayidx29 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %A, i64 0, i64 0
  %arraydecay = getelementptr inbounds [400 x i32], [400 x i32]* %arrayidx29, i32 0, i32 0
  %arrayidx30 = getelementptr inbounds [1 x i32], [1 x i32]* %alpha, i64 0, i64 0
  %0 = load i32, i32* %arrayidx30, align 4
  %arrayidx31 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %row, i64 0, i64 0
  %arraydecay32 = getelementptr inbounds [400 x i32], [400 x i32]* %arrayidx31, i32 0, i32 0
  %arrayidx33 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %col, i64 0, i64 0
  %arraydecay34 = getelementptr inbounds [400 x i32], [400 x i32]* %arrayidx33, i32 0, i32 0
  %call35 = call i32 @_Z4spmvPiiS_S_(i32* %arraydecay, i32 %0, i32* %arraydecay32, i32* %arraydecay34)
  ret i32 0
}

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
