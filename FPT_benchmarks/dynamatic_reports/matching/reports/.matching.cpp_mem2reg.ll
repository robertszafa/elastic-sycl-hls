; ModuleID = '.matching.cpp.ll'
source_filename = "src/matching.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z8matchingPiS_(i32* %vertices, i32* %edges) #0 {
entry:
  br label %while.cond

while.cond:                                       ; preds = %if.end, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %add14, %if.end ]
  %out_scalar.0 = phi i32 [ 0, %entry ], [ %out_scalar.1, %if.end ]
  %cmp = icmp slt i32 %i.0, 1000
  br i1 %cmp, label %while.body, label %while.end

while.body:                                       ; preds = %while.cond
  %mul = mul nsw i32 %i.0, 2
  %idxprom = sext i32 %mul to i64
  %arrayidx = getelementptr inbounds i32, i32* %edges, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %add = add nsw i32 %mul, 1
  %idxprom1 = sext i32 %add to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %edges, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %idxprom3 = sext i32 %0 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom3
  %2 = load i32, i32* %arrayidx4, align 4
  %idxprom5 = sext i32 %1 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom5
  %3 = load i32, i32* %arrayidx6, align 4
  %cmp7 = icmp eq i32 %2, 0
  br i1 %cmp7, label %land.lhs.true, label %if.end

land.lhs.true:                                    ; preds = %while.body
  %cmp8 = icmp eq i32 %3, 0
  br i1 %cmp8, label %if.then, label %if.end

if.then:                                          ; preds = %land.lhs.true
  %idxprom9 = sext i32 %0 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom9
  store i32 1, i32* %arrayidx10, align 4
  %idxprom11 = sext i32 %1 to i64
  %arrayidx12 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom11
  store i32 1, i32* %arrayidx12, align 4
  %add13 = add nsw i32 %out_scalar.0, 1
  br label %if.end

if.end:                                           ; preds = %if.then, %land.lhs.true, %while.body
  %out_scalar.1 = phi i32 [ %add13, %if.then ], [ %out_scalar.0, %land.lhs.true ], [ %out_scalar.0, %while.body ]
  %add14 = add nsw i32 %i.0, 1
  br label %while.cond

while.end:                                        ; preds = %while.cond
  ret i32 %out_scalar.0
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %edges = alloca [1 x [2000 x i32]], align 16
  %vertices = alloca [1 x [2000 x i32]], align 16
  br label %for.cond

for.cond:                                         ; preds = %for.inc10, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc11, %for.inc10 ]
  %cmp = icmp slt i32 %i.0, 1
  br i1 %cmp, label %for.body, label %for.end12

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %y.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %y.0, 2000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %edges, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %y.0 to i64
  %arrayidx5 = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %y.0, i32* %arrayidx5, align 4
  %idxprom6 = sext i32 %i.0 to i64
  %arrayidx7 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %vertices, i64 0, i64 %idxprom6
  %idxprom8 = sext i32 %y.0 to i64
  %arrayidx9 = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 0, i32* %arrayidx9, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %y.0, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc10

for.inc10:                                        ; preds = %for.end
  %inc11 = add nsw i32 %i.0, 1
  br label %for.cond

for.end12:                                        ; preds = %for.cond
  %arrayidx13 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %vertices, i64 0, i64 0
  %arraydecay = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx13, i32 0, i32 0
  %arrayidx14 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %edges, i64 0, i64 0
  %arraydecay15 = getelementptr inbounds [2000 x i32], [2000 x i32]* %arrayidx14, i32 0, i32 0
  %call = call i32 @_Z8matchingPiS_(i32* %arraydecay, i32* %arraydecay15)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
