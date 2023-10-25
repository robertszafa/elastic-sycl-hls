; ModuleID = '.getTanh_double.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/getTanh_double.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z14getTanh_doublePiS_(i32* %A, i32* %addr) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.body, %entry
  %i.01 = phi i32 [ 0, %entry ], [ %inc, %for.body ]
  %0 = zext i32 %i.01 to i64
  %arrayidx = getelementptr inbounds i32, i32* %addr, i64 %0
  %1 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %1 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %A, i64 %idxprom1
  %2 = load i32, i32* %arrayidx2, align 4
  %mul = mul nsw i32 %2, %2
  %add = add nuw nsw i32 %mul, 19
  %mul3 = mul nsw i32 %add, %2
  %mul4 = mul nsw i32 %mul3, %2
  %add5 = add nsw i32 %mul4, 3
  %mul6 = mul nsw i32 %add5, %2
  %idxprom7 = sext i32 %1 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %A, i64 %idxprom7
  store i32 %mul6, i32* %arrayidx8, align 4
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

for.body:                                         ; preds = %for.inc15, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.inc, %for.body
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %0 = zext i32 %j.01 to i64
  %1 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 %0
  store i32 %j.01, i32* %1, align 4
  %2 = zext i32 %j.01 to i64
  %3 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 0, i64 %2
  store i32 2, i32* %3, align 4
  %rem = urem i32 %j.01, 100
  %cmp10 = icmp eq i32 %rem, 0
  br i1 %cmp10, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body3
  %4 = zext i32 %j.01 to i64
  %5 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 %4
  store i32 0, i32* %5, align 4
  br label %for.inc

for.inc:                                          ; preds = %if.then, %for.body3
  %inc = add nuw nsw i32 %j.01, 1
  %cmp2 = icmp ult i32 %inc, 1000
  br i1 %cmp2, label %for.body3, label %for.inc15

for.inc15:                                        ; preds = %for.inc
  br i1 false, label %for.body, label %for.end17

for.end17:                                        ; preds = %for.inc15
  %arraydecay = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 0
  %arraydecay23 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 0, i64 0
  %call = call i32 @_Z14getTanh_doublePiS_(i32* nonnull %arraydecay, i32* nonnull %arraydecay23)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
