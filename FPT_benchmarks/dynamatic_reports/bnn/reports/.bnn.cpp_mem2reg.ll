; ModuleID = '.bnn.cpp.ll'
source_filename = "src/bnn.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z3bnnPiS_S_S_S_S_(i32* %w, i32* %in, i32* %mean, i32* %addr_in, i32* %addr_out, i32* %data) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc12, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc13, %for.inc12 ]
  %cmp = icmp slt i32 %i.0, 100
  br i1 %cmp, label %for.body, label %for.end14

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %j.0, 100
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %mul = mul nsw i32 %i.0, 100
  %add = add nsw i32 %mul, %j.0
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds i32, i32* %in, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom4 = sext i32 %add to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %w, i64 %idxprom4
  %1 = load i32, i32* %arrayidx5, align 4
  %xor = xor i32 %0, %1
  %mul6 = mul nsw i32 %xor, 2
  %idxprom7 = sext i32 %add to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %addr_in, i64 %idxprom7
  %2 = load i32, i32* %arrayidx8, align 4
  %idxprom9 = sext i32 %2 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %data, i64 %idxprom9
  %3 = load i32, i32* %arrayidx10, align 4
  %add11 = add nsw i32 %3, %mul6
  store i32 %add11, i32* %arrayidx10, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.0, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc12

for.inc12:                                        ; preds = %for.end
  %inc13 = add nsw i32 %i.0, 1
  br label %for.cond

for.end14:                                        ; preds = %for.cond
  br label %for.cond15

for.cond15:                                       ; preds = %for.inc33, %for.end14
  %k.0 = phi i32 [ 0, %for.end14 ], [ %inc34, %for.inc33 ]
  %cmp16 = icmp slt i32 %k.0, 100
  br i1 %cmp16, label %for.body17, label %for.end35

for.body17:                                       ; preds = %for.cond15
  %sub = sub nsw i32 %i.0, 1
  %mul18 = mul nsw i32 %sub, 100
  %add19 = add nsw i32 %mul18, %k.0
  %idxprom20 = sext i32 %add19 to i64
  %arrayidx21 = getelementptr inbounds i32, i32* %mean, i64 %idxprom20
  %4 = load i32, i32* %arrayidx21, align 4
  %idxprom22 = sext i32 %add19 to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %addr_out, i64 %idxprom22
  %5 = load i32, i32* %arrayidx23, align 4
  %idxprom24 = sext i32 %5 to i64
  %arrayidx25 = getelementptr inbounds i32, i32* %data, i64 %idxprom24
  %6 = load i32, i32* %arrayidx25, align 4
  %cmp26 = icmp sgt i32 %6, 0
  br i1 %cmp26, label %if.then, label %if.else

if.then:                                          ; preds = %for.body17
  %sub27 = sub nsw i32 %6, %4
  br label %if.end

if.else:                                          ; preds = %for.body17
  %add28 = add nsw i32 %6, %4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %z.0 = phi i32 [ %sub27, %if.then ], [ %add28, %if.else ]
  %idxprom29 = sext i32 %add19 to i64
  %arrayidx30 = getelementptr inbounds i32, i32* %addr_out, i64 %idxprom29
  %7 = load i32, i32* %arrayidx30, align 4
  %idxprom31 = sext i32 %7 to i64
  %arrayidx32 = getelementptr inbounds i32, i32* %data, i64 %idxprom31
  store i32 %z.0, i32* %arrayidx32, align 4
  br label %for.inc33

for.inc33:                                        ; preds = %if.end
  %inc34 = add nsw i32 %k.0, 1
  br label %for.cond15

for.end35:                                        ; preds = %for.cond15
  ret i32 %i.0
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
  br label %for.cond

for.cond:                                         ; preds = %for.inc26, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc27, %for.inc26 ]
  %cmp = icmp slt i32 %i.0, 1
  br i1 %cmp, label %for.body, label %for.end28

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %j.0, 10000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_in, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %j.0 to i64
  %arrayidx5 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %j.0, i32* %arrayidx5, align 4
  %idxprom6 = sext i32 %i.0 to i64
  %arrayidx7 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_out, i64 0, i64 %idxprom6
  %idxprom8 = sext i32 %j.0 to i64
  %arrayidx9 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 %j.0, i32* %arrayidx9, align 4
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %idxprom10 = sext i32 %i.0 to i64
  %arrayidx11 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %data, i64 0, i64 %idxprom10
  %idxprom12 = sext i32 %j.0 to i64
  %arrayidx13 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 %rem, i32* %arrayidx13, align 4
  %idxprom14 = sext i32 %i.0 to i64
  %arrayidx15 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %w, i64 0, i64 %idxprom14
  %idxprom16 = sext i32 %j.0 to i64
  %arrayidx17 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx15, i64 0, i64 %idxprom16
  store i32 7, i32* %arrayidx17, align 4
  %idxprom18 = sext i32 %i.0 to i64
  %arrayidx19 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %in, i64 0, i64 %idxprom18
  %idxprom20 = sext i32 %j.0 to i64
  %arrayidx21 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx19, i64 0, i64 %idxprom20
  store i32 5, i32* %arrayidx21, align 4
  %idxprom22 = sext i32 %i.0 to i64
  %arrayidx23 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %mean, i64 0, i64 %idxprom22
  %idxprom24 = sext i32 %j.0 to i64
  %arrayidx25 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx23, i64 0, i64 %idxprom24
  store i32 3, i32* %arrayidx25, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.0, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc26

for.inc26:                                        ; preds = %for.end
  %inc27 = add nsw i32 %i.0, 1
  br label %for.cond

for.end28:                                        ; preds = %for.cond
  %idxprom30 = sext i32 0 to i64
  %arrayidx31 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %w, i64 0, i64 %idxprom30
  %arraydecay = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx31, i32 0, i32 0
  %idxprom32 = sext i32 0 to i64
  %arrayidx33 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %in, i64 0, i64 %idxprom32
  %arraydecay34 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx33, i32 0, i32 0
  %idxprom35 = sext i32 0 to i64
  %arrayidx36 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %mean, i64 0, i64 %idxprom35
  %arraydecay37 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx36, i32 0, i32 0
  %idxprom38 = sext i32 0 to i64
  %arrayidx39 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_in, i64 0, i64 %idxprom38
  %arraydecay40 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx39, i32 0, i32 0
  %idxprom41 = sext i32 0 to i64
  %arrayidx42 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_out, i64 0, i64 %idxprom41
  %arraydecay43 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx42, i32 0, i32 0
  %idxprom44 = sext i32 0 to i64
  %arrayidx45 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %data, i64 0, i64 %idxprom44
  %arraydecay46 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx45, i32 0, i32 0
  %call47 = call i32 @_Z3bnnPiS_S_S_S_S_(i32* %arraydecay, i32* %arraydecay34, i32* %arraydecay37, i32* %arraydecay40, i32* %arraydecay43, i32* %arraydecay46)
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
