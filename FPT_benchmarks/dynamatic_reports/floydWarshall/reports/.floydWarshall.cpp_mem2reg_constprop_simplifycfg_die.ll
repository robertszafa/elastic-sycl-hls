; ModuleID = '.floydWarshall.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/floydWarshall.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z13floydWarshallPi(i32* %dist) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.inc33, %entry
  %k.03 = phi i32 [ 0, %entry ], [ %inc34, %for.inc33 ]
  br label %for.body3

for.body3:                                        ; preds = %for.inc30, %for.body
  %i.02 = phi i32 [ 0, %for.body ], [ %inc31, %for.inc30 ]
  br label %for.body6

for.body6:                                        ; preds = %for.inc, %for.body3
  %j.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %mul = mul nsw i32 %i.02, 10
  %add = add nuw nsw i32 %mul, %j.01
  %0 = zext i32 %add to i64
  %arrayidx = getelementptr inbounds i32, i32* %dist, i64 %0
  %1 = load i32, i32* %arrayidx, align 4
  %mul7 = mul nsw i32 %i.02, 10
  %add8 = add nuw nsw i32 %mul7, %k.03
  %2 = zext i32 %add8 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %dist, i64 %2
  %3 = load i32, i32* %arrayidx10, align 4
  %mul11 = mul nsw i32 %k.03, 10
  %add12 = add nuw nsw i32 %mul11, %j.01
  %4 = zext i32 %add12 to i64
  %arrayidx14 = getelementptr inbounds i32, i32* %dist, i64 %4
  %5 = load i32, i32* %arrayidx14, align 4
  %add15 = add nsw i32 %3, %5
  %cmp16 = icmp sgt i32 %1, %add15
  br i1 %cmp16, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body6
  %add18 = add nuw nsw i32 %mul, %k.03
  %6 = zext i32 %add18 to i64
  %arrayidx20 = getelementptr inbounds i32, i32* %dist, i64 %6
  %7 = load i32, i32* %arrayidx20, align 4
  %mul21 = mul nsw i32 %k.03, 10
  %add22 = add nuw nsw i32 %mul21, %j.01
  %8 = zext i32 %add22 to i64
  %arrayidx24 = getelementptr inbounds i32, i32* %dist, i64 %8
  %9 = load i32, i32* %arrayidx24, align 4
  %add25 = add nsw i32 %7, %9
  %mul26 = mul nsw i32 %i.02, 10
  %add27 = add nuw nsw i32 %mul26, %j.01
  %10 = zext i32 %add27 to i64
  %arrayidx29 = getelementptr inbounds i32, i32* %dist, i64 %10
  store i32 %add25, i32* %arrayidx29, align 4
  br label %for.inc

for.inc:                                          ; preds = %if.then, %for.body6
  %inc = add nuw nsw i32 %j.01, 1
  %cmp5 = icmp ult i32 %inc, 10
  br i1 %cmp5, label %for.body6, label %for.inc30

for.inc30:                                        ; preds = %for.inc
  %inc31 = add nuw nsw i32 %i.02, 1
  %cmp2 = icmp ult i32 %inc31, 10
  br i1 %cmp2, label %for.body3, label %for.inc33

for.inc33:                                        ; preds = %for.inc30
  %inc34 = add nuw nsw i32 %k.03, 1
  %cmp = icmp ult i32 %inc34, 10
  br i1 %cmp, label %for.body, label %for.end35

for.end35:                                        ; preds = %for.inc33
  ret i32 %inc34
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %dist = alloca [1 x [100 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %for.inc19, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.end, %for.body
  %j.02 = phi i32 [ 0, %for.body ], [ %inc17, %for.end ]
  br label %for.body6

for.body6:                                        ; preds = %cond.end, %for.body3
  %k.01 = phi i32 [ 0, %for.body3 ], [ %inc, %cond.end ]
  %cmp7 = icmp eq i32 %j.02, %k.01
  br i1 %cmp7, label %cond.end, label %cond.false

cond.false:                                       ; preds = %for.body6
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 10
  br label %cond.end

cond.end:                                         ; preds = %cond.false, %for.body6
  %cond = phi i32 [ %rem, %cond.false ], [ 0, %for.body6 ]
  %mul = mul nsw i32 %j.02, 10
  %add = add nuw nsw i32 %mul, %k.01
  %0 = zext i32 %add to i64
  %1 = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 0, i64 %0
  store i32 %cond, i32* %1, align 4
  %inc = add nuw nsw i32 %k.01, 1
  %cmp5 = icmp ult i32 %inc, 10
  br i1 %cmp5, label %for.body6, label %for.end

for.end:                                          ; preds = %cond.end
  %call10 = call i32 @rand() #3
  %inc17 = add nuw nsw i32 %j.02, 1
  %cmp2 = icmp ult i32 %inc17, 10
  br i1 %cmp2, label %for.body3, label %for.inc19

for.inc19:                                        ; preds = %for.end
  br i1 false, label %for.body, label %for.end21

for.end21:                                        ; preds = %for.inc19
  %arraydecay = getelementptr inbounds [1 x [100 x i32]], [1 x [100 x i32]]* %dist, i64 0, i64 0, i64 0
  %call25 = call i32 @_Z13floydWarshallPi(i32* nonnull %arraydecay)
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
