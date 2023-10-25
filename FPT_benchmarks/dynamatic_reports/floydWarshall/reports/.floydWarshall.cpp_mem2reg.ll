; ModuleID = '.floydWarshall.cpp.ll'
source_filename = "src/floydWarshall.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z13floydWarshallPi(i32* %dist) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc33, %entry
  %k.0 = phi i32 [ 0, %entry ], [ %inc34, %for.inc33 ]
  %cmp = icmp slt i32 %k.0, 10
  br i1 %cmp, label %for.body, label %for.end35

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc30, %for.body
  %i.0 = phi i32 [ 0, %for.body ], [ %inc31, %for.inc30 ]
  %cmp2 = icmp slt i32 %i.0, 10
  br i1 %cmp2, label %for.body3, label %for.end32

for.body3:                                        ; preds = %for.cond1
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %j.0 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %cmp5 = icmp slt i32 %j.0, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %mul = mul nsw i32 %i.0, 10
  %add = add nsw i32 %mul, %j.0
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds i32, i32* %dist, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %mul7 = mul nsw i32 %i.0, 10
  %add8 = add nsw i32 %mul7, %k.0
  %idxprom9 = sext i32 %add8 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %dist, i64 %idxprom9
  %1 = load i32, i32* %arrayidx10, align 4
  %mul11 = mul nsw i32 %k.0, 10
  %add12 = add nsw i32 %mul11, %j.0
  %idxprom13 = sext i32 %add12 to i64
  %arrayidx14 = getelementptr inbounds i32, i32* %dist, i64 %idxprom13
  %2 = load i32, i32* %arrayidx14, align 4
  %add15 = add nsw i32 %1, %2
  %cmp16 = icmp sgt i32 %0, %add15
  br i1 %cmp16, label %if.then, label %if.end

if.then:                                          ; preds = %for.body6
  %mul17 = mul nsw i32 %i.0, 10
  %add18 = add nsw i32 %mul17, %k.0
  %idxprom19 = sext i32 %add18 to i64
  %arrayidx20 = getelementptr inbounds i32, i32* %dist, i64 %idxprom19
  %3 = load i32, i32* %arrayidx20, align 4
  %mul21 = mul nsw i32 %k.0, 10
  %add22 = add nsw i32 %mul21, %j.0
  %idxprom23 = sext i32 %add22 to i64
  %arrayidx24 = getelementptr inbounds i32, i32* %dist, i64 %idxprom23
  %4 = load i32, i32* %arrayidx24, align 4
  %add25 = add nsw i32 %3, %4
  %mul26 = mul nsw i32 %i.0, 10
  %add27 = add nsw i32 %mul26, %j.0
  %idxprom28 = sext i32 %add27 to i64
  %arrayidx29 = getelementptr inbounds i32, i32* %dist, i64 %idxprom28
  store i32 %add25, i32* %arrayidx29, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body6
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %inc = add nsw i32 %j.0, 1
  br label %for.cond4

for.end:                                          ; preds = %for.cond4
  br label %for.inc30

for.inc30:                                        ; preds = %for.end
  %inc31 = add nsw i32 %i.0, 1
  br label %for.cond1

for.end32:                                        ; preds = %for.cond1
  br label %for.inc33

for.inc33:                                        ; preds = %for.end32
  %inc34 = add nsw i32 %k.0, 1
  br label %for.cond

for.end35:                                        ; preds = %for.cond
  ret i32 %k.0
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %vertices = alloca [1 x [10 x i32]], align 16
  %dist = alloca [1 x [100 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc20, %for.inc19 ]
  %cmp = icmp slt i32 %i.0, 1
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc16, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc17, %for.inc16 ]
  %cmp2 = icmp slt i32 %j.0, 10
  br i1 %cmp2, label %for.body3, label %for.end18

for.body3:                                        ; preds = %for.cond1
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %k.0 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %cmp5 = icmp slt i32 %k.0, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %cmp7 = icmp eq i32 %j.0, %k.0
  br i1 %cmp7, label %cond.true, label %cond.false

cond.true:                                        ; preds = %for.body6
  br label %cond.end

cond.false:                                       ; preds = %for.body6
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 10
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i32 [ 0, %cond.true ], [ %rem, %cond.false ]
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 %idxprom
  %mul = mul nsw i32 %j.0, 10
  %add = add nsw i32 %mul, %k.0
  %idxprom8 = sext i32 %add to i64
  %arrayidx9 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx, i64 0, i64 %idxprom8
  store i32 %cond, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %cond.end
  %inc = add nsw i32 %k.0, 1
  br label %for.cond4

for.end:                                          ; preds = %for.cond4
  %call10 = call i32 @rand() #3
  %rem11 = srem i32 %call10, 100
  %idxprom12 = sext i32 %i.0 to i64
  %arrayidx13 = getelementptr inbounds [1 x [10 x i32]], [1 x [10 x i32]]* %vertices, i64 0, i64 %idxprom12
  %idxprom14 = sext i32 %j.0 to i64
  %arrayidx15 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx13, i64 0, i64 %idxprom14
  store i32 %rem11, i32* %arrayidx15, align 4
  br label %for.inc16

for.inc16:                                        ; preds = %for.end
  %inc17 = add nsw i32 %j.0, 1
  br label %for.cond1

for.end18:                                        ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end18
  %inc20 = add nsw i32 %i.0, 1
  br label %for.cond

for.end21:                                        ; preds = %for.cond
  %idxprom23 = sext i32 0 to i64
  %arrayidx24 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 %idxprom23
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
