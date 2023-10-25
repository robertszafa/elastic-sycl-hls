; ModuleID = '.matching.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/matching.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z8matchingPiS_(i32* %vertices, i32* %edges) #0 {
entry:
  br label %while.body

while.body:                                       ; preds = %if.end, %entry
  %out_scalar.02 = phi i32 [ 0, %entry ], [ %out_scalar.1, %if.end ]
  %i.01 = phi i32 [ 0, %entry ], [ %add14, %if.end ]
  %mul = shl nuw nsw i32 %i.01, 1
  %0 = zext i32 %mul to i64
  %arrayidx = getelementptr inbounds i32, i32* %edges, i64 %0
  %1 = load i32, i32* %arrayidx, align 4
  %add = or i32 %mul, 1
  %2 = zext i32 %add to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %edges, i64 %2
  %3 = load i32, i32* %arrayidx2, align 4
  %idxprom3 = sext i32 %1 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom3
  %4 = load i32, i32* %arrayidx4, align 4
  %idxprom5 = sext i32 %3 to i64
  %arrayidx6 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom5
  %5 = load i32, i32* %arrayidx6, align 4
  %6 = or i32 %4, %5
  %7 = icmp eq i32 %6, 0
  br i1 %7, label %if.then, label %if.end

if.then:                                          ; preds = %while.body
  %idxprom9 = sext i32 %1 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom9
  store i32 1, i32* %arrayidx10, align 4
  %idxprom11 = sext i32 %3 to i64
  %arrayidx12 = getelementptr inbounds i32, i32* %vertices, i64 %idxprom11
  store i32 1, i32* %arrayidx12, align 4
  %add13 = add nsw i32 %out_scalar.02, 1
  br label %if.end

if.end:                                           ; preds = %if.then, %while.body
  %out_scalar.1 = phi i32 [ %add13, %if.then ], [ %out_scalar.02, %while.body ]
  %add14 = add nuw nsw i32 %i.01, 1
  %cmp = icmp ult i32 %add14, 1000
  br i1 %cmp, label %while.body, label %while.end

while.end:                                        ; preds = %if.end
  ret i32 %out_scalar.1
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %edges = alloca [1 x [2000 x i32]], align 16
  %vertices = alloca [1 x [2000 x i32]], align 16
  br label %for.body

for.body:                                         ; preds = %for.inc10, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %y.01 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %0 = zext i32 %y.01 to i64
  %1 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %edges, i64 0, i64 0, i64 %0
  store i32 %y.01, i32* %1, align 4
  %2 = zext i32 %y.01 to i64
  %3 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %vertices, i64 0, i64 0, i64 %2
  store i32 0, i32* %3, align 4
  %inc = add nuw nsw i32 %y.01, 1
  %cmp2 = icmp ult i32 %inc, 2000
  br i1 %cmp2, label %for.body3, label %for.inc10

for.inc10:                                        ; preds = %for.body3
  br i1 false, label %for.body, label %for.end12

for.end12:                                        ; preds = %for.inc10
  %arraydecay = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %vertices, i64 0, i64 0, i64 0
  %arraydecay15 = getelementptr inbounds [1 x [2000 x i32]], [1 x [2000 x i32]]* %edges, i64 0, i64 0, i64 0
  %call = call i32 @_Z8matchingPiS_(i32* nonnull %arraydecay, i32* nonnull %arraydecay15)
  ret i32 0
}

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
