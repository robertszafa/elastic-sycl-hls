; ModuleID = '.histogramIf.cpp.ll'
source_filename = "src/histogramIf.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z11histogramIfPiS_S_i(i32* %feature, i32* %weight, i32* %hist, i32 %n) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %for.inc ]
  %cmp = icmp slt i32 %i.0, %n
  br i1 %cmp, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds i32, i32* %feature, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %i.0 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %weight, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %idxprom3 = sext i32 %0 to i64
  %arrayidx4 = getelementptr inbounds i32, i32* %hist, i64 %idxprom3
  %2 = load i32, i32* %arrayidx4, align 4
  %cmp5 = icmp sgt i32 %2, 0
  br i1 %cmp5, label %if.then, label %if.end

if.then:                                          ; preds = %for.body
  %add = add nsw i32 %2, %1
  %idxprom6 = sext i32 %0 to i64
  %arrayidx7 = getelementptr inbounds i32, i32* %hist, i64 %idxprom6
  store i32 %add, i32* %arrayidx7, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %for.body
  br label %for.inc

for.inc:                                          ; preds = %if.end
  %inc = add nsw i32 %i.0, 1
  br label %for.cond

for.end:                                          ; preds = %for.cond
  ret i32 %i.0
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %feature = alloca [1 x [1000 x i32]], align 16
  %weight = alloca [1 x [1000 x i32]], align 16
  %hist = alloca [1 x [1000 x i32]], align 16
  %n = alloca [1 x i32], align 4
  call void @srand(i32 13) #3
  br label %for.cond

for.cond:                                         ; preds = %for.inc16, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc17, %for.inc16 ]
  %cmp = icmp slt i32 %i.0, 1
  br i1 %cmp, label %for.body, label %for.end18

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom
  store i32 1000, i32* %arrayidx, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %j.0, 1000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %idxprom4 = sext i32 %i.0 to i64
  %arrayidx5 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %feature, i64 0, i64 %idxprom4
  %idxprom6 = sext i32 %j.0 to i64
  %arrayidx7 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx5, i64 0, i64 %idxprom6
  store i32 1, i32* %arrayidx7, align 4
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %idxprom8 = sext i32 %i.0 to i64
  %arrayidx9 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %weight, i64 0, i64 %idxprom8
  %idxprom10 = sext i32 %j.0 to i64
  %arrayidx11 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx9, i64 0, i64 %idxprom10
  store i32 %rem, i32* %arrayidx11, align 4
  %idxprom12 = sext i32 %i.0 to i64
  %arrayidx13 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %hist, i64 0, i64 %idxprom12
  %idxprom14 = sext i32 %j.0 to i64
  %arrayidx15 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx13, i64 0, i64 %idxprom14
  store i32 1, i32* %arrayidx15, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.0, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc16

for.inc16:                                        ; preds = %for.end
  %inc17 = add nsw i32 %i.0, 1
  br label %for.cond

for.end18:                                        ; preds = %for.cond
  %idxprom20 = sext i32 0 to i64
  %arrayidx21 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %feature, i64 0, i64 %idxprom20
  %arraydecay = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx21, i32 0, i32 0
  %idxprom22 = sext i32 0 to i64
  %arrayidx23 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %weight, i64 0, i64 %idxprom22
  %arraydecay24 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx23, i32 0, i32 0
  %idxprom25 = sext i32 0 to i64
  %arrayidx26 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %hist, i64 0, i64 %idxprom25
  %arraydecay27 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx26, i32 0, i32 0
  %idxprom28 = sext i32 0 to i64
  %arrayidx29 = getelementptr inbounds [1 x i32], [1 x i32]* %n, i64 0, i64 %idxprom28
  %0 = load i32, i32* %arrayidx29, align 4
  %call30 = call i32 @_Z11histogramIfPiS_S_i(i32* %arraydecay, i32* %arraydecay24, i32* %arraydecay27, i32 %0)
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
