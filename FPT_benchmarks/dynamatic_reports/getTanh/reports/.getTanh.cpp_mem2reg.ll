; ModuleID = '.getTanh.cpp.ll'
source_filename = "src/getTanh.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZZ4mainE5atanh = private unnamed_addr constant [1 x [12 x i32]] [[12 x i32] [i32 2249, i32 1046, i32 514, i32 256, i32 128, i32 100, i32 50, i32 16, i32 8, i32 4, i32 2, i32 1]], align 16
@_ZZ4mainE4cosh = private unnamed_addr constant [1 x [5 x i32]] [[5 x i32] [i32 4096, i32 6320, i32 15409, i32 41237, i32 111854]], align 16
@_ZZ4mainE4sinh = private unnamed_addr constant [1 x [5 x i32]] [[5 x i32] [i32 0, i32 4813, i32 14855, i32 41033, i32 111779]], align 16

; Function Attrs: noinline nounwind uwtable
define i32 @_Z7getTanhPiS_(i32* %A, i32* %addr) #0 {
entry:
  br label %for.cond

for.cond:                                         ; preds = %for.inc40, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc41, %for.inc40 ]
  %result.0 = phi i32 [ undef, %entry ], [ %mul35, %for.inc40 ]
  %cmp = icmp slt i32 %i.0, 1000
  br i1 %cmp, label %for.body, label %for.end42

for.body:                                         ; preds = %for.cond
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds i32, i32* %addr, i64 %idxprom
  %0 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %0 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %A, i64 %idxprom1
  %1 = load i32, i32* %arrayidx2, align 4
  %cmp3 = icmp sge i32 %1, 8192
  br i1 %cmp3, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  br label %if.end15

if.else:                                          ; preds = %for.body
  %cmp4 = icmp sge i32 %1, 12288
  br i1 %cmp4, label %if.then5, label %if.else6

if.then5:                                         ; preds = %if.else
  br label %if.end14

if.else6:                                         ; preds = %if.else
  %cmp7 = icmp sge i32 %1, 8192
  br i1 %cmp7, label %if.then8, label %if.else9

if.then8:                                         ; preds = %if.else6
  br label %if.end13

if.else9:                                         ; preds = %if.else6
  %cmp10 = icmp sge i32 %1, 4096
  br i1 %cmp10, label %if.then11, label %if.else12

if.then11:                                        ; preds = %if.else9
  br label %if.end

if.else12:                                        ; preds = %if.else9
  br label %if.end

if.end:                                           ; preds = %if.else12, %if.then11
  %index_trigo.0 = phi i32 [ 5, %if.then11 ], [ 6, %if.else12 ]
  br label %if.end13

if.end13:                                         ; preds = %if.end, %if.then8
  %index_trigo.1 = phi i32 [ 2, %if.then8 ], [ %index_trigo.0, %if.end ]
  br label %if.end14

if.end14:                                         ; preds = %if.end13, %if.then5
  %index_trigo.2 = phi i32 [ 3, %if.then5 ], [ %index_trigo.1, %if.end13 ]
  br label %if.end15

if.end15:                                         ; preds = %if.end14, %if.then
  %index_trigo.3 = phi i32 [ 4, %if.then ], [ %index_trigo.2, %if.end14 ]
  %mul = mul nsw i32 %index_trigo.3, 4096
  %sub = sub nsw i32 %1, %mul
  br label %for.cond16

for.cond16:                                       ; preds = %for.inc, %if.end15
  %y.0 = phi i32 [ 1, %if.end15 ], [ %sub25, %for.inc ]
  %x.0 = phi i32 [ 4945, %if.end15 ], [ %sub23, %for.inc ]
  %beta.0 = phi i32 [ %sub, %if.end15 ], [ %add26, %for.inc ]
  %k.0 = phi i32 [ 1, %if.end15 ], [ %inc, %for.inc ]
  %cmp17 = icmp sle i32 %k.0, 12
  br i1 %cmp17, label %for.body18, label %for.end

for.body18:                                       ; preds = %for.cond16
  %shr = ashr i32 %y.0, %k.0
  %sub19 = sub nsw i32 %x.0, %shr
  %shr20 = ashr i32 %x.0, %k.0
  %sub21 = sub nsw i32 %y.0, %shr20
  %add = add nsw i32 %beta.0, %index_trigo.3
  %shr22 = ashr i32 %sub21, %k.0
  %sub23 = sub nsw i32 %sub19, %shr22
  %shr24 = ashr i32 %sub19, %k.0
  %sub25 = sub nsw i32 %sub21, %shr24
  %add26 = add nsw i32 %add, %index_trigo.3
  br label %for.inc

for.inc:                                          ; preds = %for.body18
  %inc = add nsw i32 %k.0, 1
  br label %for.cond16

for.end:                                          ; preds = %for.cond16
  %mul27 = mul nsw i32 %index_trigo.3, %x.0
  %mul28 = mul nsw i32 %index_trigo.3, %y.0
  %add29 = add nsw i32 %mul27, %mul28
  %mul30 = mul nsw i32 %index_trigo.3, %x.0
  %mul31 = mul nsw i32 %index_trigo.3, %y.0
  %add32 = add nsw i32 %mul30, %mul31
  %shr33 = ashr i32 %add32, 12
  %or = or i32 %add29, 1
  %or34 = or i32 %shr33, 1
  %mul35 = mul nsw i32 %or, %or34
  %idxprom36 = sext i32 %i.0 to i64
  %arrayidx37 = getelementptr inbounds i32, i32* %addr, i64 %idxprom36
  %2 = load i32, i32* %arrayidx37, align 4
  %idxprom38 = sext i32 %2 to i64
  %arrayidx39 = getelementptr inbounds i32, i32* %A, i64 %idxprom38
  store i32 %mul35, i32* %arrayidx39, align 4
  br label %for.inc40

for.inc40:                                        ; preds = %for.end
  %inc41 = add nsw i32 %i.0, 1
  br label %for.cond

for.end42:                                        ; preds = %for.cond
  ret i32 %result.0
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %a = alloca [1 x [1000 x i32]], align 16
  %b = alloca [1 x [1000 x i32]], align 16
  %atanh = alloca [1 x [12 x i32]], align 16
  %cosh = alloca [1 x [5 x i32]], align 16
  %sinh = alloca [1 x [5 x i32]], align 16
  %0 = bitcast [1 x [12 x i32]]* %atanh to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([1 x [12 x i32]]* @_ZZ4mainE5atanh to i8*), i64 48, i32 16, i1 false)
  %1 = bitcast [1 x [5 x i32]]* %cosh to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* bitcast ([1 x [5 x i32]]* @_ZZ4mainE4cosh to i8*), i64 20, i32 16, i1 false)
  %2 = bitcast [1 x [5 x i32]]* %sinh to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* bitcast ([1 x [5 x i32]]* @_ZZ4mainE4sinh to i8*), i64 20, i32 16, i1 false)
  br label %for.cond

for.cond:                                         ; preds = %for.inc14, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc15, %for.inc14 ]
  %cmp = icmp slt i32 %i.0, 1
  br i1 %cmp, label %for.body, label %for.end16

for.body:                                         ; preds = %for.cond
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %j.0 = phi i32 [ 0, %for.body ], [ %inc, %for.inc ]
  %cmp2 = icmp slt i32 %j.0, 1000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %idxprom = sext i32 %i.0 to i64
  %arrayidx = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom
  %idxprom4 = sext i32 %j.0 to i64
  %arrayidx5 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %j.0, i32* %arrayidx5, align 4
  %idxprom6 = sext i32 %i.0 to i64
  %arrayidx7 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 %idxprom6
  %idxprom8 = sext i32 %j.0 to i64
  %arrayidx9 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 %j.0, i32* %arrayidx9, align 4
  %idxprom10 = sext i32 %i.0 to i64
  %arrayidx11 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom10
  %idxprom12 = sext i32 %j.0 to i64
  %arrayidx13 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 0, i32* %arrayidx13, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %inc = add nsw i32 %j.0, 1
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %inc15 = add nsw i32 %i.0, 1
  br label %for.cond

for.end16:                                        ; preds = %for.cond
  %idxprom18 = sext i32 0 to i64
  %arrayidx19 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom18
  %arraydecay = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx19, i32 0, i32 0
  %idxprom20 = sext i32 0 to i64
  %arrayidx21 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 %idxprom20
  %arraydecay22 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx21, i32 0, i32 0
  %call = call i32 @_Z7getTanhPiS_(i32* %arraydecay, i32* %arraydecay22)
  ret i32 0
}

; Function Attrs: argmemonly nounwind
declare void @llvm.memcpy.p0i8.p0i8.i64(i8* nocapture writeonly, i8* nocapture readonly, i64, i32, i1) #2

attributes #0 = { noinline nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { noinline norecurse nounwind uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { argmemonly nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 6.0.1 (http://llvm.org/git/clang.git 2f27999df400d17b33cdd412fdd606a88208dfcc) (http://llvm.org/git/llvm.git 5136df4d089a086b70d452160ad5451861269498)"}
