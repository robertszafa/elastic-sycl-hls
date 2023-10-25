; ModuleID = 'src/matching.cpp'
source_filename = "src/matching.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z8matchingPiS_(i32* %vertices, i32* %edges) #0 {
entry:
  %vertices.addr = alloca i32*, align 8
  %edges.addr = alloca i32*, align 8
  %i = alloca i32, align 4
  %out_scalar = alloca i32, align 4
  %j = alloca i32, align 4
  %e1 = alloca i32, align 4
  %e2 = alloca i32, align 4
  %v1 = alloca i32, align 4
  %v2 = alloca i32, align 4
  store i32* %vertices, i32** %vertices.addr, align 8
  store i32* %edges, i32** %edges.addr, align 8
  store i32 0, i32* %i, align 4
  store i32 0, i32* %out_scalar, align 4
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1000
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %1 = load i32, i32* %i, align 4
  %mul = mul nsw i32 %1, 2
  store i32 %mul, i32* %j, align 4
  %2 = load i32*, i32** %edges.addr, align 8
  %3 = load i32, i32* %j, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds i32, i32* %2, i64 %idxprom
  %4 = load i32, i32* %arrayidx, align 4
  store i32 %4, i32* %e1, align 4
  %5 = load i32*, i32** %edges.addr, align 8
  %6 = load i32, i32* %j, align 4
  %add = add nsw i32 %6, 1
  %idxprom1 = sext i32 %add to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %5, i64 %idxprom1
  %7 = load i32, i32* %arrayidx2, align 4
  store i32 %7, i32* %e2, align 4
  %8 = load i32*, i32** %vertices.addr, align 8
  %9 = load i32, i32* %e1, align 4
  %idxprom3 = sext i32 %9 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %8, i64 %idxprom3
  %10 = load i32, i32* %arrayidx4, align 4
  store i32 %10, i32* %v1, align 4
  %11 = load i32*, i32** %vertices.addr, align 8
  %12 = load i32, i32* %e2, align 4
  %idxprom5 = sext i32 %12 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %11, i64 %idxprom5
  %13 = load i32, i32* %arrayidx6, align 4
  store i32 %13, i32* %v2, align 4
  %14 = load i32, i32* %v1, align 4
  %cmp7 = icmp eq i32 %14, 0
  br i1 %cmp7, label %land.lhs.true, label %if.end

land.lhs.true:                                    ; preds = %while.body
  %15 = load i32, i32* %v2, align 4
  %cmp8 = icmp eq i32 %15, 0
  br i1 %cmp8, label %if.then, label %if.end

if.then:                                          ; preds = %land.lhs.true
  %16 = load i32*, i32** %vertices.addr, align 8
  %17 = load i32, i32* %e1, align 4
  %idxprom9 = sext i32 %17 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %16, i64 %idxprom9
  store i32 1, i32* %arrayidx10, align 4
  %18 = load i32*, i32** %vertices.addr, align 8
  %19 = load i32, i32* %e2, align 4
  %idxprom11 = sext i32 %19 to i64
  %arrayidx12 = getelementptr inbounds i32, i32* %18, i64 %idxprom11
  store i32 1, i32* %arrayidx12, align 4
  %20 = load i32, i32* %out_scalar, align 4
  %add13 = add nsw i32 %20, 1
  store i32 %add13, i32* %out_scalar, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %land.lhs.true, %while.body
  %21 = load i32, i32* %i, align 4
  %add14 = add nsw i32 %21, 1
  store i32 %add14, i32* %i, align 4
  br label %while.cond

while.end:                                        ; preds = %while.cond
  %22 = load i32, i32* %out_scalar, align 4
  ret i32 %22
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %edges = alloca [1 x [2000 x i32]], align 16
  %vertices = alloca [1 x [2000 x i32]], align 16
  %i = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc10, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end12

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %y, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, i32* %y, align 4
  %cmp2 = icmp slt i32 %1, 2000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %2 = load i32, i32* %y, align 4
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %edges, i64 0, i64 %idxprom
  %4 = load i32, i32* %y, align 4
  %idxprom4 = sext i32 %4 to i64
  %arrayidx5 = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %2, i32* %arrayidx5, align 4
  %5 = load i32, i32* %i, align 4
  %idxprom6 = sext i32 %5 to i64
  %arrayidx7 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %vertices, i64 0, i64 %idxprom6
  %6 = load i32, i32* %y, align 4
  %idxprom8 = sext i32 %6 to i64
  %arrayidx9 = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 0, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %7 = load i32, i32* %y, align 4
  %inc = add nsw i32 %7, 1
  store i32 %inc, i32* %y, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc10

for.inc10:                                        ; preds = %for.end
  %8 = load i32, i32* %i, align 4
  %inc11 = add nsw i32 %8, 1
  store i32 %inc11, i32* %i, align 4
  br label %for.cond

for.end12:                                        ; preds = %for.cond
  %arrayidx13 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %vertices, i64 0, i64 0
  %arraydecay = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx13, i32 0, i32 0
  %arrayidx14 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %edges, i64 0, i64 0
  %arraydecay15 = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx14, i32 0, i32 0
  %call = call i32 @_Z8matchingPiS_(i32* %arraydecay, i32* %arraydecay15)
  %9 = load i32, i32* %retval, align 4
  ret i32 %9
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
