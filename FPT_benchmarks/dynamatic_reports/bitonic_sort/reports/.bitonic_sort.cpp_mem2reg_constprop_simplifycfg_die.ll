; ModuleID = '.bitonic_sort.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/bitonic_sort.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z12bitonic_sortPi(i32* %A) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.inc29, %entry
  %k.04 = phi i32 [ 2, %entry ], [ %shl, %for.inc29 ]
  %cmp22 = icmp sgt i32 %k.04, 0
  br i1 %cmp22, label %for.body3.lr.ph, label %for.inc29

for.body3.lr.ph:                                  ; preds = %for.body
  br label %for.body3

for.body3:                                        ; preds = %for.inc26, %for.body3.lr.ph
  %j.03.in = phi i32 [ %k.04, %for.body3.lr.ph ], [ %j.03, %for.inc26 ]
  %j.03 = ashr i32 %j.03.in, 1
  br label %for.body6

for.body6:                                        ; preds = %for.inc, %for.body3
  %i.01 = phi i32 [ 0, %for.body3 ], [ %inc, %for.inc ]
  %xor = xor i32 %i.01, %j.03
  %0 = zext i32 %i.01 to i64
  %arrayidx = getelementptr inbounds i32, i32* %A, i64 %0
  %1 = load i32, i32* %arrayidx, align 4
  %idxprom7 = sext i32 %xor to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %A, i64 %idxprom7
  %2 = load i32, i32* %arrayidx8, align 4
  %cmp9 = icmp sgt i32 %xor, %i.01
  br i1 %cmp9, label %if.then, label %for.inc

if.then:                                          ; preds = %for.body6
  %and = and i32 %i.01, %k.04
  %cmp10 = icmp eq i32 %and, 0
  %cmp12 = icmp sgt i32 %1, %2
  %cmp14 = icmp slt i32 %1, %2
  %toSwap.2.v = select i1 %cmp10, i1 %cmp12, i1 %cmp14
  br i1 %toSwap.2.v, label %if.then19, label %for.inc

if.then19:                                        ; preds = %if.then
  %3 = zext i32 %i.01 to i64
  %arrayidx21 = getelementptr inbounds i32, i32* %A, i64 %3
  store i32 %2, i32* %arrayidx21, align 4
  %idxprom22 = sext i32 %xor to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %A, i64 %idxprom22
  store i32 %1, i32* %arrayidx23, align 4
  br label %for.inc

for.inc:                                          ; preds = %if.then19, %if.then, %for.body6
  %inc = add nuw nsw i32 %i.01, 1
  %cmp5 = icmp ult i32 %inc, 64
  br i1 %cmp5, label %for.body6, label %for.inc26

for.inc26:                                        ; preds = %for.inc
  %cmp2 = icmp sgt i32 %j.03.in, 3
  br i1 %cmp2, label %for.body3, label %for.inc29

for.inc29:                                        ; preds = %for.inc26, %for.body
  %shl = shl i32 %k.04, 1
  %cmp = icmp slt i32 %shl, 65
  br i1 %cmp, label %for.body, label %for.end30

for.end30:                                        ; preds = %for.inc29
  ret i32 %shl
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %A = alloca [1 x [64 x i32]], align 16
  call void @srand(i32 13) #3
  br label %for.body

for.body:                                         ; preds = %for.inc6, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %0 = zext i32 %j.01 to i64
  %1 = getelementptr inbounds [1 x [64 x i32]], [1 x [64 x i32]]* %A, i64 0, i64 0, i64 %0
  store i32 %j.01, i32* %1, align 4
  %inc = add nuw nsw i32 %j.01, 1
  %cmp2 = icmp ult i32 %inc, 64
  br i1 %cmp2, label %for.body3, label %for.inc6

for.inc6:                                         ; preds = %for.body3
  br i1 false, label %for.body, label %for.end8

for.end8:                                         ; preds = %for.inc6
  %arraydecay = getelementptr inbounds [1 x [64 x i32]], [1 x [64 x i32]]* %A, i64 0, i64 0, i64 0
  %call = call i32 @_Z12bitonic_sortPi(i32* nonnull %arraydecay)
  ret i32 0
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
