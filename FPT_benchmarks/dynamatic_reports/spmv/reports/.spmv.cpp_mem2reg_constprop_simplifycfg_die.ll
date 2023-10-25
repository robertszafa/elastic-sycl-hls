; ModuleID = '.spmv.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/spmv.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z4spmvPiiS_S_(i32* %A, i32 %alpha, i32* %row, i32* %col) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.inc11, %entry
  %ptr.04 = phi i32 [ 0, %entry ], [ %inc, %for.inc11 ]
  %k.03 = phi i32 [ 1, %entry ], [ %inc12, %for.inc11 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %p.02 = phi i32 [ 0, %for.body ], [ %inc10, %for.body3 ]
  %ptr.11 = phi i32 [ %ptr.04, %for.body ], [ %inc, %for.body3 ]
  %idxprom = sext i32 %ptr.11 to i64
  %arrayidx = getelementptr inbounds i32, i32* %col, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom4 = sext i32 %0 to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %A, i64 %idxprom4
  %1 = load i32, i32* %arrayidx5, align 4
  %mul = mul nsw i32 %1, %alpha
  %idxprom6 = sext i32 %ptr.11 to i64
  %arrayidx7 = getelementptr inbounds i32, i32* %row, i64 %idxprom6
  %2 = load i32, i32* %arrayidx7, align 4
  %idxprom8 = sext i32 %2 to i64
  %arrayidx9 = getelementptr inbounds i32, i32* %A, i64 %idxprom8
  %3 = load i32, i32* %arrayidx9, align 4
  %add = add nsw i32 %3, %mul
  store i32 %add, i32* %arrayidx9, align 4
  %inc = add nsw i32 %ptr.11, 1
  %inc10 = add nuw nsw i32 %p.02, 1
  %cmp2 = icmp ult i32 %inc10, 20
  br i1 %cmp2, label %for.body3, label %for.inc11

for.inc11:                                        ; preds = %for.body3
  %inc12 = add nuw nsw i32 %k.03, 1
  %cmp = icmp ult i32 %inc12, 20
  br i1 %cmp, label %for.body, label %for.end13

for.end13:                                        ; preds = %for.inc11
  ret i32 %inc12
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %alpha = alloca [1 x i32], align 4
  %row = alloca [1 x [400 x i32]], align 16
  %col = alloca [1 x [400 x i32]], align 16
  %A = alloca [1 x [400 x i32]], align 16
  br label %for.body

for.body:                                         ; preds = %for.inc26, %entry
  %i.05 = phi i32 [ 0, %entry ], [ %inc27, %for.inc26 ]
  %call = call i32 @rand() #3
  %0 = getelementptr inbounds [1 x i32], [1 x i32]* %alpha, i64 0, i64 0
  store i32 %call, i32* %0, align 4
  br label %for.body3

for.body3:                                        ; preds = %for.inc23, %for.body
  %ptr.04 = phi i32 [ 0, %for.body ], [ %inc, %for.inc23 ]
  %y.03 = phi i32 [ 0, %for.body ], [ %inc24, %for.inc23 ]
  br label %for.body6

for.body6:                                        ; preds = %for.body6, %for.body3
  %x.02 = phi i32 [ 0, %for.body3 ], [ %inc22, %for.body6 ]
  %ptr.11 = phi i32 [ %ptr.04, %for.body3 ], [ %inc, %for.body6 ]
  %call7 = call i32 @rand() #3
  %rem = srem i32 %call7, 100
  %1 = zext i32 %i.05 to i64
  %idxprom10 = sext i32 %ptr.11 to i64
  %arrayidx11 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %A, i64 0, i64 %1, i64 %idxprom10
  store i32 %rem, i32* %arrayidx11, align 4
  %rem12 = srem i32 %ptr.11, 3
  %2 = zext i32 %i.05 to i64
  %idxprom15 = sext i32 %ptr.11 to i64
  %arrayidx16 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %row, i64 0, i64 %2, i64 %idxprom15
  store i32 %rem12, i32* %arrayidx16, align 4
  %add = add nsw i32 %ptr.11, 1
  %rem17 = srem i32 %add, 3
  %3 = zext i32 %i.05 to i64
  %idxprom20 = sext i32 %ptr.11 to i64
  %arrayidx21 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %col, i64 0, i64 %3, i64 %idxprom20
  store i32 %rem17, i32* %arrayidx21, align 4
  %inc = add nsw i32 %ptr.11, 1
  %inc22 = add nuw nsw i32 %x.02, 1
  %cmp5 = icmp ult i32 %inc22, 20
  br i1 %cmp5, label %for.body6, label %for.inc23

for.inc23:                                        ; preds = %for.body6
  %inc24 = add nuw nsw i32 %y.03, 1
  %cmp2 = icmp ult i32 %inc24, 20
  br i1 %cmp2, label %for.body3, label %for.inc26

for.inc26:                                        ; preds = %for.inc23
  %inc27 = add nuw nsw i32 %i.05, 1
  br i1 false, label %for.body, label %for.end28

for.end28:                                        ; preds = %for.inc26
  %arraydecay = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %A, i64 0, i64 0, i64 0
  %arrayidx30 = getelementptr inbounds [1 x i32], [1 x i32]* %alpha, i64 0, i64 0
  %4 = load i32, i32* %arrayidx30, align 4
  %arraydecay32 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %row, i64 0, i64 0, i64 0
  %arraydecay34 = getelementptr inbounds [1 x [400 x i32]], [1 x [400 x i32]]* %col, i64 0, i64 0, i64 0
  %call35 = call i32 @_Z4spmvPiiS_S_(i32* nonnull %arraydecay, i32 %4, i32* nonnull %arraydecay32, i32* nonnull %arraydecay34)
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
