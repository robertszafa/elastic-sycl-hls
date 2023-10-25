; ModuleID = 'src/bitonic_sort.cpp'
source_filename = "src/bitonic_sort.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z12bitonic_sortPi(i32* %A) #0 {
entry:
  %A.addr = alloca i32*, align 8
  %k = alloca i32, align 4
  %j = alloca i32, align 4
  %i = alloca i32, align 4
  %l = alloca i32, align 4
  %Ai = alloca i32, align 4
  %Al = alloca i32, align 4
  %toSwap = alloca i32, align 4
  store i32* %A, i32** %A.addr, align 8
  store i32 2, i32* %k, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc29, %entry
  %0 = load i32, i32* %k, align 4
  %cmp = icmp sle i32 %0, 64
  br i1 %cmp, label %for.body, label %for.end30

for.body:                                         ; preds = %for.cond
  %1 = load i32, i32* %k, align 4
  %shr = ashr i32 %1, 1
  store i32 %shr, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc26, %for.body
  %2 = load i32, i32* %j, align 4
  %cmp2 = icmp sgt i32 %2, 0
  br i1 %cmp2, label %for.body3, label %for.end28

for.body3:                                        ; preds = %for.cond1
  store i32 0, i32* %i, align 4
  br label %for.cond4

for.cond4:                                        ; preds = %for.inc, %for.body3
  %3 = load i32, i32* %i, align 4
  %cmp5 = icmp slt i32 %3, 64
  br i1 %cmp5, label %for.body6, label %for.end

for.body6:                                        ; preds = %for.cond4
  %4 = load i32, i32* %i, align 4
  %5 = load i32, i32* %j, align 4
  %xor = xor i32 %4, %5
  store i32 %xor, i32* %l, align 4
  %6 = load i32*, i32** %A.addr, align 8
  %7 = load i32, i32* %i, align 4
  %idxprom = sext i32 %7 to i64
  %arrayidx = getelementptr inbounds i32, i32* %6, i64 %idxprom
  %8 = load i32, i32* %arrayidx, align 4
  store i32 %8, i32* %Ai, align 4
  %9 = load i32*, i32** %A.addr, align 8
  %10 = load i32, i32* %l, align 4
  %idxprom7 = sext i32 %10 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %9, i64 %idxprom7
  %11 = load i32, i32* %arrayidx8, align 4
  store i32 %11, i32* %Al, align 4
  %12 = load i32, i32* %l, align 4
  %13 = load i32, i32* %i, align 4
  %cmp9 = icmp sgt i32 %12, %13
  br i1 %cmp9, label %if.then, label %if.end25

if.then:                                          ; preds = %for.body6
  store i32 0, i32* %toSwap, align 4
  %14 = load i32, i32* %i, align 4
  %15 = load i32, i32* %k, align 4
  %and = and i32 %14, %15
  %cmp10 = icmp eq i32 %and, 0
  br i1 %cmp10, label %if.then11, label %if.else

if.then11:                                        ; preds = %if.then
  %16 = load i32, i32* %Ai, align 4
  %17 = load i32, i32* %Al, align 4
  %cmp12 = icmp sgt i32 %16, %17
  br i1 %cmp12, label %if.then13, label %if.end

if.then13:                                        ; preds = %if.then11
  store i32 1, i32* %toSwap, align 4
  br label %if.end

if.end:                                           ; preds = %if.then13, %if.then11
  br label %if.end17

if.else:                                          ; preds = %if.then
  %18 = load i32, i32* %Ai, align 4
  %19 = load i32, i32* %Al, align 4
  %cmp14 = icmp slt i32 %18, %19
  br i1 %cmp14, label %if.then15, label %if.end16

if.then15:                                        ; preds = %if.else
  store i32 1, i32* %toSwap, align 4
  br label %if.end16

if.end16:                                         ; preds = %if.then15, %if.else
  br label %if.end17

if.end17:                                         ; preds = %if.end16, %if.end
  %20 = load i32, i32* %toSwap, align 4
  %cmp18 = icmp eq i32 %20, 1
  br i1 %cmp18, label %if.then19, label %if.end24

if.then19:                                        ; preds = %if.end17
  %21 = load i32, i32* %Al, align 4
  %22 = load i32*, i32** %A.addr, align 8
  %23 = load i32, i32* %i, align 4
  %idxprom20 = sext i32 %23 to i64
  %arrayidx21 = getelementptr inbounds i32, i32* %22, i64 %idxprom20
  store i32 %21, i32* %arrayidx21, align 4
  %24 = load i32, i32* %Ai, align 4
  %25 = load i32*, i32** %A.addr, align 8
  %26 = load i32, i32* %l, align 4
  %idxprom22 = sext i32 %26 to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %25, i64 %idxprom22
  store i32 %24, i32* %arrayidx23, align 4
  br label %if.end24

if.end24:                                         ; preds = %if.then19, %if.end17
  br label %if.end25

if.end25:                                         ; preds = %if.end24, %for.body6
  br label %for.inc

for.inc:                                          ; preds = %if.end25
  %27 = load i32, i32* %i, align 4
  %inc = add nsw i32 %27, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond4

for.end:                                          ; preds = %for.cond4
  br label %for.inc26

for.inc26:                                        ; preds = %for.end
  %28 = load i32, i32* %j, align 4
  %shr27 = ashr i32 %28, 1
  store i32 %shr27, i32* %j, align 4
  br label %for.cond1

for.end28:                                        ; preds = %for.cond1
  br label %for.inc29

for.inc29:                                        ; preds = %for.end28
  %29 = load i32, i32* %k, align 4
  %shl = shl i32 %29, 1
  store i32 %shl, i32* %k, align 4
  br label %for.cond

for.end30:                                        ; preds = %for.cond
  %30 = load i32, i32* %k, align 4
  ret i32 %30
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %A = alloca [1 x [64 x i32]], align 16
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %i9 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @srand(i32 13) #3
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc6, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end8

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %1, 64
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %2 = load i32, i32* %j, align 4
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1 x [64 x i32]], [1 x [64 x i32]]* %A, i64 0, i64 %idxprom
  %4 = load i32, i32* %j, align 4
  %idxprom4 = sext i32 %4 to i64
  %arrayidx5 = getelementptr inbounds [64 x i32], [64 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %2, i32* %arrayidx5, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %5 = load i32, i32* %j, align 4
  %inc = add nsw i32 %5, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc6

for.inc6:                                         ; preds = %for.end
  %6 = load i32, i32* %i, align 4
  %inc7 = add nsw i32 %6, 1
  store i32 %inc7, i32* %i, align 4
  br label %for.cond

for.end8:                                         ; preds = %for.cond
  store i32 0, i32* %i9, align 4
  %7 = load i32, i32* %i9, align 4
  %idxprom10 = sext i32 %7 to i64
  %arrayidx11 = getelementptr inbounds [1 x [64 x i32]], [1 x [64 x i32]]* %A, i64 0, i64 %idxprom10
  %arraydecay = getelementptr inbounds [64 x i32], [64 x i32]* %arrayidx11, i32 0, i32 0
  %call = call i32 @_Z12bitonic_sortPi(i32* %arraydecay)
  %8 = load i32, i32* %retval, align 4
  ret i32 %8
}

; Function Attrs: nounwind
declare void @srand(i32) #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
