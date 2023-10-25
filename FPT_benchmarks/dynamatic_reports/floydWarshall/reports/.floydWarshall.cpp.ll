; ModuleID = 'src/floydWarshall.cpp'
source_filename = "src/floydWarshall.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z13floydWarshallPi(i32* %dist) #0 {
entry:
  %dist.addr = alloca i32*, align 8
  %k = alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32* %dist, i32** %dist.addr, align 8
  store i32 0, i32* %k, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc33, %entry
  %0 = load i32, i32* %k, align 4
  %cmp = icmp slt i32 %0, 10
  br i1 %cmp, label %for.body, label %for.end35

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %i, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc30, %for.body
  %1 = load i32, i32* %i, align 4
  %cmp2 = icmp slt i32 %1, 10
  br i1 %cmp2, label %for.body3, label %for.end32

for.body3:                                        ; preds = %for.cond1
  store i32 0, i32* %j, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %2 = load i32, i32* %j, align 4
  %cmp5 = icmp slt i32 %2, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %3 = load i32*, i32** %dist.addr, align 8
  %4 = load i32, i32* %i, align 4
  %mul = mul nsw i32 %4, 10
  %5 = load i32, i32* %j, align 4
  %add = add nsw i32 %mul, %5
  %idxprom = sext i32 %add to i64
  %arrayidx = getelementptr inbounds i32, i32* %3, i64 %idxprom
  %6 = load i32, i32* %arrayidx, align 4
  %7 = load i32*, i32** %dist.addr, align 8
  %8 = load i32, i32* %i, align 4
  %mul7 = mul nsw i32 %8, 10
  %9 = load i32, i32* %k, align 4
  %add8 = add nsw i32 %mul7, %9
  %idxprom9 = sext i32 %add8 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %7, i64 %idxprom9
  %10 = load i32, i32* %arrayidx10, align 4
  %11 = load i32*, i32** %dist.addr, align 8
  %12 = load i32, i32* %k, align 4
  %mul11 = mul nsw i32 %12, 10
  %13 = load i32, i32* %j, align 4
  %add12 = add nsw i32 %mul11, %13
  %idxprom13 = sext i32 %add12 to i64
  %arrayidx14 = getelementptr inbounds i32, i32* %11, i64 %idxprom13
  %14 = load i32, i32* %arrayidx14, align 4
  %add15 = add nsw i32 %10, %14
  %cmp16 = icmp sgt i32 %6, %add15
  br i1 %cmp16, label %if.then, label %if.end

if.then:                                          ; preds = %for.body6
  %15 = load i32*, i32** %dist.addr, align 8
  %16 = load i32, i32* %i, align 4
  %mul17 = mul nsw i32 %16, 10
  %17 = load i32, i32* %k, align 4
  %add18 = add nsw i32 %mul17, %17
  %idxprom19 = sext i32 %add18 to i64
  %arrayidx20 = getelementptr inbounds i32, i32* %15, i64 %idxprom19
  %18 = load i32, i32* %arrayidx20, align 4
  %19 = load i32*, i32** %dist.addr, align 8
  %20 = load i32, i32* %k, align 4
  %mul21 = mul nsw i32 %20, 10
  %21 = load i32, i32* %j, align 4
  %add22 = add nsw i32 %mul21, %21
  %idxprom23 = sext i32 %add22 to i64
  %arrayidx24 = getelementptr inbounds i32, i32* %19, i64 %idxprom23
  %22 = load i32, i32* %arrayidx24, align 4
  %add25 = add nsw i32 %18, %22
  %23 = load i32*, i32** %dist.addr, align 8
  %24 = load i32, i32* %i, align 4
  %mul26 = mul nsw i32 %24, 10
  %25 = load i32, i32* %j, align 4
  %add27 = add nsw i32 %mul26, %25
  %idxprom28 = sext i32 %add27 to i64
  %arrayidx29 = getelementptr inbounds i32, i32* %23, i64 %idxprom28
  store i32 %add25, i32* %arrayidx29, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body6
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %26 = load i32, i32* %j, align 4
  %inc = add nsw i32 %26, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond4

for.end:                                          ; preds = %for.cond4
  br label %for.inc30

for.inc30:                                        ; preds = %for.end
  %27 = load i32, i32* %i, align 4
  %inc31 = add nsw i32 %27, 1
  store i32 %inc31, i32* %i, align 4
  br label %for.cond1

for.end32:                                        ; preds = %for.cond1
  br label %for.inc33

for.inc33:                                        ; preds = %for.end32
  %28 = load i32, i32* %k, align 4
  %inc34 = add nsw i32 %28, 1
  store i32 %inc34, i32* %k, align 4
  br label %for.cond

for.end35:                                        ; preds = %for.cond
  %29 = load i32, i32* %k, align 4
  ret i32 %29
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %vertices = alloca [1 x [10 x i32]], align 16
  %dist = alloca [1 x [100 x i32]], align 16
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  %i22 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @srand(i32 13) #3
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc19, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end21

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc16, %for.body
  %1 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %1, 10
  br i1 %cmp2, label %for.body3, label %for.end18

for.body3:                                        ; preds = %for.cond1
  store i32 0, i32* %k, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %2 = load i32, i32* %k, align 4
  %cmp5 = icmp slt i32 %2, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %3 = load i32, i32* %j, align 4
  %4 = load i32, i32* %k, align 4
  %cmp7 = icmp eq i32 %3, %4
  br i1 %cmp7, label %cond.true, label %cond.false

cond.true:                                        ; preds = %for.body6
  br label %cond.end

cond.false:                                       ; preds = %for.body6
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 10
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %cond.true
  %cond = phi i32 [ 0, %cond.true ], [ %rem, %cond.false ]
  %5 = load i32, i32* %i, align 4
  %idxprom = sext i32 %5 to i64
  %arrayidx = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 %idxprom
  %6 = load i32, i32* %j, align 4
  %mul = mul nsw i32 %6, 10
  %7 = load i32, i32* %k, align 4
  %add = add nsw i32 %mul, %7
  %idxprom8 = sext i32 %add to i64
  %arrayidx9 = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx, i64 0, i64 %idxprom8
  store i32 %cond, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %cond.end
  %8 = load i32, i32* %k, align 4
  %inc = add nsw i32 %8, 1
  store i32 %inc, i32* %k, align 4
  br label %for.cond4

for.end:                                          ; preds = %for.cond4
  %call10 = call i32 @rand() #3
  %rem11 = srem i32 %call10, 100
  %9 = load i32, i32* %i, align 4
  %idxprom12 = sext i32 %9 to i64
  %arrayidx13 = getelementptr inbounds [1 x [10 x i32]], [1 x [10 x i32]]* %vertices, i64 0, i64 %idxprom12
  %10 = load i32, i32* %j, align 4
  %idxprom14 = sext i32 %10 to i64
  %arrayidx15 = getelementptr inbounds [10 x i32], [10 x i32]* %arrayidx13, i64 0, i64 %idxprom14
  store i32 %rem11, i32* %arrayidx15, align 4
  br label %for.inc16

for.inc16:                                        ; preds = %for.end
  %11 = load i32, i32* %j, align 4
  %inc17 = add nsw i32 %11, 1
  store i32 %inc17, i32* %j, align 4
  br label %for.cond1

for.end18:                                        ; preds = %for.cond1
  br label %for.inc19

for.inc19:                                        ; preds = %for.end18
  %12 = load i32, i32* %i, align 4
  %inc20 = add nsw i32 %12, 1
  store i32 %inc20, i32* %i, align 4
  br label %for.cond

for.end21:                                        ; preds = %for.cond
  store i32 0, i32* %i22, align 4
  %13 = load i32, i32* %i22, align 4
  %idxprom23 = sext i32 %13 to i64
  %arrayidx24 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 %idxprom23
  %arraydecay = getelementptr inbounds [100 x i32], [100 x i32]* %arrayidx24, i32 0, i32 0
  %call25 = call i32 @_Z13floydWarshallPi(i32* %arraydecay)
  %14 = load i32, i32* %retval, align 4
  ret i32 %14
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
