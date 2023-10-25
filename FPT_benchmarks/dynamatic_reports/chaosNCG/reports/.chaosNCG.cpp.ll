; ModuleID = 'src/chaosNCG.cpp'
source_filename = "src/chaosNCG.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z8chaosNCGPiS_iii(i32* %A, i32* %M, i32 %I, i32 %Y, i32 %X) #0 {
entry:
  %A.addr = alloca i32*, align 8
  %M.addr = alloca i32*, align 8
  %I.addr = alloca i32, align 4
  %Y.addr = alloca i32, align 4
  %X.addr = alloca i32, align 4
  %bound = alloca i32, align 4
  %i = alloca i32, align 4
  %a = alloca i32, align 4
  %b = alloca i32, align 4
  %b0 = alloca i32, align 4
  %b1 = alloca i32, align 4
  store i32* %A, i32** %A.addr, align 8
  store i32* %M, i32** %M.addr, align 8
  store i32 %I, i32* %I.addr, align 4
  store i32 %Y, i32* %Y.addr, align 4
  store i32 %X, i32* %X.addr, align 4
  store i32 2000, i32* %bound, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %0 = load i32, i32* %i, align 4
  %1 = load i32, i32* %bound, align 4
  %cmp = icmp slt i32 %0, %1
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %2 = load i32*, i32** %M.addr, align 8
  %3 = load i32, i32* %I.addr, align 4
  %4 = load i32, i32* %i, align 4
  %add = add nsw i32 %3, %4
  %add1 = add nsw i32 %add, 2
  %idxprom = sext i32 %add1 to i64
  %arrayidx = getelementptr inbounds i32, i32* %2, i64 %idxprom
  %5 = load i32, i32* %arrayidx, align 4
  store i32 %5, i32* %a, align 4
  %6 = load i32*, i32** %M.addr, align 8
  %7 = load i32, i32* %I.addr, align 4
  %8 = load i32, i32* %i, align 4
  %add2 = add nsw i32 %7, %8
  %sub = sub nsw i32 %add2, 2
  %idxprom3 = sext i32 %sub to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %6, i64 %idxprom3
  %9 = load i32, i32* %arrayidx4, align 4
  store i32 %9, i32* %b, align 4
  %10 = load i32*, i32** %A.addr, align 8
  %11 = load i32, i32* %a, align 4
  %idxprom5 = sext i32 %11 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %10, i64 %idxprom5
  %12 = load i32, i32* %arrayidx6, align 4
  store i32 %12, i32* %b0, align 4
  %13 = load i32*, i32** %A.addr, align 8
  %14 = load i32, i32* %b, align 4
  %idxprom7 = sext i32 %14 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %13, i64 %idxprom7
  %15 = load i32, i32* %arrayidx8, align 4
  store i32 %15, i32* %b1, align 4
  %16 = load i32, i32* %b1, align 4
  %17 = load i32, i32* %b0, align 4
  %xor = xor i32 %17, %16
  store i32 %xor, i32* %b0, align 4
  %18 = load i32, i32* %X.addr, align 4
  %19 = load i32, i32* %b0, align 4
  %and = and i32 %19, 15
  %shl = shl i32 %18, %and
  %20 = load i32, i32* %X.addr, align 4
  %21 = load i32, i32* %b0, align 4
  %and9 = and i32 %21, 15
  %sub10 = sub nsw i32 16, %and9
  %shr = ashr i32 %20, %sub10
  %or = or i32 %shl, %shr
  %22 = load i32, i32* %b0, align 4
  %xor11 = xor i32 %22, %or
  store i32 %xor11, i32* %b0, align 4
  %23 = load i32, i32* %b0, align 4
  %24 = load i32, i32* %b1, align 4
  %xor12 = xor i32 %24, %23
  store i32 %xor12, i32* %b1, align 4
  %25 = load i32, i32* %Y.addr, align 4
  %26 = load i32, i32* %b1, align 4
  %and13 = and i32 %26, 15
  %shl14 = shl i32 %25, %and13
  %27 = load i32, i32* %Y.addr, align 4
  %28 = load i32, i32* %b1, align 4
  %and15 = and i32 %28, 15
  %sub16 = sub nsw i32 16, %and15
  %shr17 = ashr i32 %27, %sub16
  %or18 = or i32 %shl14, %shr17
  %29 = load i32, i32* %b1, align 4
  %add19 = add nsw i32 %29, %or18
  store i32 %add19, i32* %b1, align 4
  %30 = load i32, i32* %b0, align 4
  %31 = load i32*, i32** %A.addr, align 8
  %32 = load i32*, i32** %M.addr, align 8
  %33 = load i32, i32* %i, align 4
  %add20 = add nsw i32 %33, 0
  %idxprom21 = sext i32 %add20 to i64
  %arrayidx22 = getelementptr inbounds i32, i32* %32, i64 %idxprom21
  %34 = load i32, i32* %arrayidx22, align 4
  %idxprom23 = sext i32 %34 to i64
  %arrayidx24 = getelementptr inbounds i32, i32* %31, i64 %idxprom23
  store i32 %30, i32* %arrayidx24, align 4
  %35 = load i32, i32* %b1, align 4
  %36 = load i32*, i32** %A.addr, align 8
  %37 = load i32*, i32** %M.addr, align 8
  %38 = load i32, i32* %i, align 4
  %add25 = add nsw i32 %38, 1
  %idxprom26 = sext i32 %add25 to i64
  %arrayidx27 = getelementptr inbounds i32, i32* %37, i64 %idxprom26
  %39 = load i32, i32* %arrayidx27, align 4
  %idxprom28 = sext i32 %39 to i64
  %arrayidx29 = getelementptr inbounds i32, i32* %36, i64 %idxprom28
  store i32 %35, i32* %arrayidx29, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %40 = load i32, i32* %i, align 4
  %add30 = add nsw i32 %40, 2
  store i32 %add30, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  %41 = load i32, i32* %i, align 4
  ret i32 %41
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %M = alloca [1 x [3000 x i32]], align 16
  %A = alloca [1 x [3000 x i32]], align 16
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %i13 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @srand(i32 13) #3
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc10, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end12

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %1, 3000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %2 = load i32, i32* %j, align 4
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %M, i64 0, i64 %idxprom
  %4 = load i32, i32* %j, align 4
  %idxprom4 = sext i32 %4 to i64
  %arrayidx5 = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %2, i32* %arrayidx5, align 4
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %5 = load i32, i32* %i, align 4
  %idxprom6 = sext i32 %5 to i64
  %arrayidx7 = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %A, i64 0, i64 %idxprom6
  %6 = load i32, i32* %j, align 4
  %idxprom8 = sext i32 %6 to i64
  %arrayidx9 = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 %rem, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %7 = load i32, i32* %j, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc10

for.inc10:                                        ; preds = %for.end
  %8 = load i32, i32* %i, align 4
  %inc11 = add nsw i32 %8, 1
  store i32 %inc11, i32* %i, align 4
  br label %for.cond

for.end12:                                        ; preds = %for.cond
  store i32 0, i32* %i13, align 4
  %9 = load i32, i32* %i13, align 4
  %idxprom14 = sext i32 %9 to i64
  %arrayidx15 = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %A, i64 0, i64 %idxprom14
  %arraydecay = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx15, i32 0, i32 0
  %10 = load i32, i32* %i13, align 4
  %idxprom16 = sext i32 %10 to i64
  %arrayidx17 = getelementptr inbounds [1 x [3000 x i32]], [1 x [3000 x i32]]* %M, i64 0, i64 %idxprom16
  %arraydecay18 = getelementptr inbounds [3000 x i32], [3000 x i32]* %arrayidx17, i32 0, i32 0
  %call19 = call i32 @_Z8chaosNCGPiS_iii(i32* %arraydecay, i32* %arraydecay18, i32 2, i32 1, i32 1)
  %11 = load i32, i32* %retval, align 4
  ret i32 %11
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
