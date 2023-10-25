; ModuleID = '.bnn.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/bnn.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z3bnnPiS_S_S_S_S_(i32* %w, i32* %in, i32* %mean, i32* %addr_in, i32* %addr_out, i32* %data) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.inc12, %entry
  %i.03 = phi i32 [ 0, %entry ], [ %inc13, %for.inc12 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %j.02 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %mul = mul nsw i32 %i.03, 100
  %add = add nuw nsw i32 %mul, %j.02
  %0 = zext i32 %add to i64
  %arrayidx = getelementptr inbounds i32, i32* %in, i64 %0
  %1 = load i32, i32* %arrayidx, align 4
  %2 = zext i32 %add to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %w, i64 %2
  %3 = load i32, i32* %arrayidx5, align 4
  %xor = xor i32 %1, %3
  %mul6 = shl nsw i32 %xor, 1
  %4 = zext i32 %add to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %addr_in, i64 %4
  %5 = load i32, i32* %arrayidx8, align 4
  %idxprom9 = sext i32 %5 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %data, i64 %idxprom9
  %6 = load i32, i32* %arrayidx10, align 4
  %add11 = add nsw i32 %6, %mul6
  store i32 %add11, i32* %arrayidx10, align 4
  %inc = add nuw nsw i32 %j.02, 1
  %cmp2 = icmp ult i32 %inc, 100
  br i1 %cmp2, label %for.body3, label %for.inc12

for.inc12:                                        ; preds = %for.body3
  %inc13 = add nuw nsw i32 %i.03, 1
  %cmp = icmp ult i32 %inc13, 100
  br i1 %cmp, label %for.body, label %for.end14

for.end14:                                        ; preds = %for.inc12
  br label %for.body17

for.body17:                                       ; preds = %for.body17, %for.end14
  %k.01 = phi i32 [ 0, %for.end14 ], [ %inc34, %for.body17 ]
  %mul18 = mul nsw i32 %i.03, 100
  %add19 = add nuw nsw i32 %mul18, %k.01
  %7 = zext i32 %add19 to i64
  %arrayidx21 = getelementptr inbounds i32, i32* %mean, i64 %7
  %8 = load i32, i32* %arrayidx21, align 4
  %9 = zext i32 %add19 to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %addr_out, i64 %9
  %10 = load i32, i32* %arrayidx23, align 4
  %idxprom24 = sext i32 %10 to i64
  %arrayidx25 = getelementptr inbounds i32, i32* %data, i64 %idxprom24
  %11 = load i32, i32* %arrayidx25, align 4
  %cmp26 = icmp sgt i32 %11, 0
  %12 = sub i32 0, %8
  %z.0.p = select i1 %cmp26, i32 %12, i32 %8
  %z.0 = add i32 %11, %z.0.p
  %13 = zext i32 %add19 to i64
  %arrayidx30 = getelementptr inbounds i32, i32* %addr_out, i64 %13
  %14 = load i32, i32* %arrayidx30, align 4
  %idxprom31 = sext i32 %14 to i64
  %arrayidx32 = getelementptr inbounds i32, i32* %data, i64 %idxprom31
  store i32 %z.0, i32* %arrayidx32, align 4
  %inc34 = add nuw nsw i32 %k.01, 1
  %cmp16 = icmp ult i32 %inc34, 100
  br i1 %cmp16, label %for.body17, label %for.end35

for.end35:                                        ; preds = %for.body17
  ret i32 %inc13
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %addr_in = alloca [1 x [10000 x i32]], align 16
  %addr_out = alloca [1 x [10000 x i32]], align 16
  %data = alloca [1 x [10000 x i32]], align 16
  %w = alloca [1 x [10000 x i32]], align 16
  %in = alloca [1 x [10000 x i32]], align 16
  %mean = alloca [1 x [10000 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %for.inc26, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %0 = zext i32 %j.01 to i64
  %1 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_in, i64 0, i64 0, i64 %0
  store i32 %j.01, i32* %1, align 4
  %2 = zext i32 %j.01 to i64
  %3 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_out, i64 0, i64 0, i64 %2
  store i32 %j.01, i32* %3, align 4
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %4 = zext i32 %j.01 to i64
  %5 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %data, i64 0, i64 0, i64 %4
  store i32 %rem, i32* %5, align 4
  %6 = zext i32 %j.01 to i64
  %7 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %w, i64 0, i64 0, i64 %6
  store i32 7, i32* %7, align 4
  %8 = zext i32 %j.01 to i64
  %9 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %in, i64 0, i64 0, i64 %8
  store i32 5, i32* %9, align 4
  %10 = zext i32 %j.01 to i64
  %11 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %mean, i64 0, i64 0, i64 %10
  store i32 3, i32* %11, align 4
  %inc = add nuw nsw i32 %j.01, 1
  %cmp2 = icmp ult i32 %inc, 10000
  br i1 %cmp2, label %for.body3, label %for.inc26

for.inc26:                                        ; preds = %for.body3
  br i1 false, label %for.body, label %for.end28

for.end28:                                        ; preds = %for.inc26
  %arraydecay = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %w, i64 0, i64 0, i64 0
  %arraydecay34 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %in, i64 0, i64 0, i64 0
  %arraydecay37 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %mean, i64 0, i64 0, i64 0
  %arraydecay40 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_in, i64 0, i64 0, i64 0
  %arraydecay43 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_out, i64 0, i64 0, i64 0
  %arraydecay46 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %data, i64 0, i64 0, i64 0
  %call47 = call i32 @_Z3bnnPiS_S_S_S_S_(i32* nonnull %arraydecay, i32* nonnull %arraydecay34, i32* nonnull %arraydecay37, i32* nonnull %arraydecay40, i32* nonnull %arraydecay43, i32* nonnull %arraydecay46)
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
