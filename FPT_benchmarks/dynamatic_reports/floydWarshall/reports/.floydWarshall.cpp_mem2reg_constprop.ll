; ModuleID = '.floydWarshall.cpp_mem2reg.ll'
source_filename = "src/floydWarshall.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z13floydWarshallPi(i32* %dist) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc33
  %k.03 = phi i32 [ 0, %entry ], [ %inc34, %for.inc33 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc30
  %i.02 = phi i32 [ 0, %for.body ], [ %inc31, %for.inc30 ]
  br label %for.body6

for.body6:                                        ; preds = %for.body3, %for.inc
  %j.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %mul = mul nsw i32 %i.02, 10
  %add = add nsw i32 %mul, %j.01
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds i32, i32* %dist, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %mul7 = mul nsw i32 %i.02, 10
  %add8 = add nsw i32 %mul7, %k.03
  %idxprom9 = sext i32 %add8 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %dist, i64 %idxprom9
  %1 = load i32, i32* %arrayidx10, align 4
  %mul11 = mul nsw i32 %k.03, 10
  %add12 = add nsw i32 %mul11, %j.01
  %idxprom13 = sext i32 %add12 to i64
  %arrayidx14 = getelementptr inbounds i32, i32* %dist, i64 %idxprom13
  %2 = load i32, i32* %arrayidx14, align 4
  %add15 = add nsw i32 %1, %2
  %cmp16 = icmp sgt i32 %0, %add15
  br i1 %cmp16, label %if.then, label %if.end

if.then:                                          ; preds = %for.body6
  %mul17 = mul nsw i32 %i.02, 10
  %add18 = add nsw i32 %mul17, %k.03
  %idxprom19 = sext i32 %add18 to i64
  %arrayidx20 = getelementptr inbounds i32, i32* %dist, i64 %idxprom19
  %3 = load i32, i32* %arrayidx20, align 4
  %mul21 = mul nsw i32 %k.03, 10
  %add22 = add nsw i32 %mul21, %j.01
  %idxprom23 = sext i32 %add22 to i64
  %arrayidx24 = getelementptr inbounds i32, i32* %dist, i64 %idxprom23
  %4 = load i32, i32* %arrayidx24, align 4
  %add25 = add nsw i32 %3, %4
  %mul26 = mul nsw i32 %i.02, 10
  %add27 = add nsw i32 %mul26, %j.01
  %idxprom28 = sext i32 %add27 to i64
  %arrayidx29 = getelementptr inbounds i32, i32* %dist, i64 %idxprom28
  store i32 %add25, i32* %arrayidx29, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body6
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %inc = add nsw i32 %j.01, 1
  %cmp5 = icmp slt i32 %inc, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.end:                                          ; preds = %for.inc
  br label %for.inc30

for.inc30:                                        ; preds = %for.end
  %inc31 = add nsw i32 %i.02, 1
  %cmp2 = icmp slt i32 %inc31, 10
  br i1 %cmp2, label %for.body3, label %for.end32

for.end32:                                        ; preds = %for.inc30
  br label %for.inc33

for.inc33:                                        ; preds = %for.end32
  %inc34 = add nsw i32 %k.03, 1
  %cmp = icmp slt i32 %inc34, 10
  br i1 %cmp, label %for.body, label %for.end35

for.end35:                                        ; preds = %for.inc33
  %k.0.lcssa = phi i32 [ %inc34, %for.inc33 ]
  ret i32 %k.0.lcssa
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %vertices = alloca [1 x [10 x i32]], align 16
  %dist = alloca [1 x [100 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %entry, %for.inc19
  %i.03 = phi i32 [ 0, %entry ], [ %inc20, %for.inc19 ]
  br label %for.body3

for.body3:                                        ; preds = %for.body, %for.inc16
  %j.02 = phi i32 [ 0, %for.body ], [ %inc17, %for.inc16 ]
  br label %for.body6

for.body6:                                        ; preds = %for.body3, %for.inc
  %k.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %cmp7 = icmp eq i32 %j.02, %k.01
  br i1 %cmp7, label %cond.true, label %cond.false

cond.true:                                        ; preds = %for.body6
  br label %cond.end

cond.false:                                       ; preds = %for.body6
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 10
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i32 [ 0, %cond.true ], [ %rem, %cond.false ]
  %idxprom = sext i32 %i.03 to i64
  %arrayidx = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 %idxprom
  %mul = mul nsw i32 %j.02, 10
  %add = add nsw i32 %mul, %k.01
  %idxprom8 = sext i32 %add to i64
  %arrayidx9 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx, i64 0, i64 %idxprom8
  store i32 %cond, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %cond.end
  %inc = add nsw i32 %k.01, 1
  %cmp5 = icmp slt i32 %inc, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.end:                                          ; preds = %for.inc
  %call10 = call i32 @rand() #3
  %rem11 = srem i32 %call10, 100
  %idxprom12 = sext i32 %i.03 to i64
  %arrayidx13 = getelementptr inbounds [1 x [10 x i32]], [1 x [10 x i32]]* %vertices, i64 0, i64 %idxprom12
  %idxprom14 = sext i32 %j.02 to i64
  %arrayidx15 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx13, i64 0, i64 %idxprom14
  store i32 %rem11, i32* %arrayidx15, align 4
  br label %for.inc16

for.inc16:                                        ; preds = %for.end
  %inc17 = add nsw i32 %j.02, 1
  %cmp2 = icmp slt i32 %inc17, 10
  br i1 %cmp2, label %for.body3, label %for.end18

for.end18:                                        ; preds = %for.inc16
  br label %for.inc19

for.inc19:                                        ; preds = %for.end18
  %inc20 = add nsw i32 %i.03, 1
  %cmp = icmp slt i32 %inc20, 1
  br i1 %cmp, label %for.body, label %for.end21

for.end21:                                        ; preds = %for.inc19
  %arrayidx24 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 0
  %arraydecay = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx24, i32 0, i32 0
  %call25 = call i32 @_Z13floydWarshallPi(i32* %arraydecay)
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
