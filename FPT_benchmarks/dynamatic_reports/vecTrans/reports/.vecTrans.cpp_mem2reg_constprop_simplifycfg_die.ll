; ModuleID = '.vecTrans.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/vecTrans.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z8vecTransPiS_(i32* %A, i32* %b) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.body, %entry
  %i.01 = phi i32 [ 0, %entry ], [ %inc, %for.body ]
  %0 = zext i32 %i.01 to i64
  %arrayidx = getelementptr inbounds i32, i32* %A, i64 %0
  %1 = load i32, i32* %arrayidx, align 4
  %add = add nsw i32 %1, 112
  %mul = mul nsw i32 %add, %1
  %add1 = add nsw i32 %mul, 23
  %mul2 = mul nsw i32 %add1, %1
  %add3 = add nsw i32 %mul2, 36
  %mul4 = mul nsw i32 %add3, %1
  %add5 = add nsw i32 %mul4, 82
  %mul6 = mul nsw i32 %add5, %1
  %add7 = add nsw i32 %mul6, 127
  %mul8 = mul nsw i32 %add7, %1
  %add9 = add nsw i32 %mul8, 2
  %mul10 = mul nsw i32 %add9, %1
  %add11 = add nsw i32 %mul10, 20
  %mul12 = mul nsw i32 %add11, %1
  %add13 = add nsw i32 %mul12, 100
  %2 = zext i32 %i.01 to i64
  %arrayidx15 = getelementptr inbounds i32, i32* %b, i64 %2
  %3 = load i32, i32* %arrayidx15, align 4
  %idxprom16 = sext i32 %3 to i64
  %arrayidx17 = getelementptr inbounds i32, i32* %A, i64 %idxprom16
  store i32 %add13, i32* %arrayidx17, align 4
  %inc = add nuw nsw i32 %i.01, 1
  %cmp = icmp ult i32 %inc, 1000
  br i1 %cmp, label %for.body, label %for.end

for.end:                                          ; preds = %for.body
  ret i32 %inc
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %a = alloca [1 x [1000 x i32]], align 16
  %b = alloca [1 x [1000 x i32]], align 16
  br label %for.body

for.body:                                         ; preds = %for.inc16, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.inc, %for.body
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %0 = zext i32 %j.01 to i64
  %1 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 %0
  store i32 %j.01, i32* %1, align 4
  %add = add nuw nsw i32 %j.01, 1
  %rem = urem i32 %add, 1000
  %2 = zext i32 %j.01 to i64
  %3 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 0, i64 %2
  store i32 %rem, i32* %3, align 4
  %rem10 = urem i32 %j.01, 100
  %cmp11 = icmp eq i32 %rem10, 0
  br i1 %cmp11, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body3
  %4 = zext i32 %j.01 to i64
  %5 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 %4
  store i32 0, i32* %5, align 4
  br label %for.inc

for.inc:                                          ; preds = %if.then, %for.body3
  %inc = add nuw nsw i32 %j.01, 1
  %cmp2 = icmp ult i32 %inc, 1000
  br i1 %cmp2, label %for.body3, label %for.inc16

for.inc16:                                        ; preds = %for.inc
  br i1 false, label %for.body, label %for.end18

for.end18:                                        ; preds = %for.inc16
  %arraydecay = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 0
  %arraydecay24 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 0, i64 0
  %call = call i32 @_Z8vecTransPiS_(i32* nonnull %arraydecay, i32* nonnull %arraydecay24)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
