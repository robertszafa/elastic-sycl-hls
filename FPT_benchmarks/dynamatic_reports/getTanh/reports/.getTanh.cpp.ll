; ModuleID = 'src/getTanh.cpp'
source_filename = "src/getTanh.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZZ4mainE5atanh = private unnamed_addr constant [1 x [12 x i32]] [[12 x i32] [i32 2249, i32 1046, i32 514, i32 256, i32 128, i32 100, i32 50, i32 16, i32 8, i32 4, i32 2, i32 1]], align 16
@_ZZ4mainE4cosh = private unnamed_addr constant [1 x [5 x i32]] [[5 x i32] [i32 4096, i32 6320, i32 15409, i32 41237, i32 111854]], align 16
@_ZZ4mainE4sinh = private unnamed_addr constant [1 x [5 x i32]] [[5 x i32] [i32 0, i32 4813, i32 14855, i32 41033, i32 111779]], align 16

; Function Attrs: noinline nounwind uwtable
define i32 @_Z7getTanhPiS_(i32* %A, i32* %addr) #0 {
entry:
  %A.addr = alloca i32*, align 8
  %addr.addr = alloca i32*, align 8
  %result = alloca i32, align 4
  %i = alloca i32, align 4
  %beta = alloca i32, align 4
  %x = alloca i32, align 4
  %y = alloca i32, align 4
  %x_new = alloca i32, align 4
  %index_trigo = alloca i32, align 4
  %result_cosh = alloca i32, align 4
  %result_sinh = alloca i32, align 4
  %outputcosh = alloca i32, align 4
  %outputsinh = alloca i32, align 4
  %k = alloca i32, align 4
  store i32* %A, i32** %A.addr, align 8
  store i32* %addr, i32** %addr.addr, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc40, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1000
  br i1 %cmp, label %for.body, label %for.end42

for.body:                                         ; preds = %for.cond
  store i32 4096, i32* %result, align 4
  %1 = load i32*, i32** %A.addr, align 8
  %2 = load i32*, i32** %addr.addr, align 8
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds i32, i32* %2, i64 %idxprom
  %4 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %4 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %1, i64 %idxprom1
  %5 = load i32, i32* %arrayidx2, align 4
  store i32 %5, i32* %beta, align 4
  store i32 4945, i32* %x, align 4
  store i32 1, i32* %y, align 4
  store i32 0, i32* %x_new, align 4
  store i32 0, i32* %index_trigo, align 4
  store i32 0, i32* %result_cosh, align 4
  store i32 0, i32* %result_sinh, align 4
  store i32 0, i32* %outputcosh, align 4
  store i32 0, i32* %outputsinh, align 4
  %6 = load i32, i32* %beta, align 4
  %cmp3 = icmp sge i32 %6, 8192
  br i1 %cmp3, label %if.then, label %if.else

if.then:                                          ; preds = %for.body
  store i32 4, i32* %index_trigo, align 4
  br label %if.end15

if.else:                                          ; preds = %for.body
  %7 = load i32, i32* %beta, align 4
  %cmp4 = icmp sge i32 %7, 12288
  br i1 %cmp4, label %if.then5, label %if.else6

if.then5:                                         ; preds = %if.else
  store i32 3, i32* %index_trigo, align 4
  br label %if.end14

if.else6:                                         ; preds = %if.else
  %8 = load i32, i32* %beta, align 4
  %cmp7 = icmp sge i32 %8, 8192
  br i1 %cmp7, label %if.then8, label %if.else9

if.then8:                                         ; preds = %if.else6
  store i32 2, i32* %index_trigo, align 4
  br label %if.end13

if.else9:                                         ; preds = %if.else6
  %9 = load i32, i32* %beta, align 4
  %cmp10 = icmp sge i32 %9, 4096
  br i1 %cmp10, label %if.then11, label %if.else12

if.then11:                                        ; preds = %if.else9
  store i32 5, i32* %index_trigo, align 4
  br label %if.end

if.else12:                                        ; preds = %if.else9
  store i32 6, i32* %index_trigo, align 4
  br label %if.end

if.end:                                           ; preds = %if.else12, %if.then11
  br label %if.end13

if.end13:                                         ; preds = %if.end, %if.then8
  br label %if.end14

if.end14:                                         ; preds = %if.end13, %if.then5
  br label %if.end15

if.end15:                                         ; preds = %if.end14, %if.then
  %10 = load i32, i32* %beta, align 4
  %11 = load i32, i32* %index_trigo, align 4
  %mul = mul nsw i32 %11, 4096
  %sub = sub nsw i32 %10, %mul
  store i32 %sub, i32* %beta, align 4
  store i32 1, i32* %k, align 4
  br label %for.cond16

for.cond16:                                       ; preds = %for.inc, %if.end15
  %12 = load i32, i32* %k, align 4
  %cmp17 = icmp sle i32 %12, 12
  br i1 %cmp17, label %for.body18, label %for.end

for.body18:                                       ; preds = %for.cond16
  %13 = load i32, i32* %x, align 4
  %14 = load i32, i32* %y, align 4
  %15 = load i32, i32* %k, align 4
  %shr = ashr i32 %14, %15
  %sub19 = sub nsw i32 %13, %shr
  store i32 %sub19, i32* %x_new, align 4
  %16 = load i32, i32* %x, align 4
  %17 = load i32, i32* %k, align 4
  %shr20 = ashr i32 %16, %17
  %18 = load i32, i32* %y, align 4
  %sub21 = sub nsw i32 %18, %shr20
  store i32 %sub21, i32* %y, align 4
  %19 = load i32, i32* %index_trigo, align 4
  %20 = load i32, i32* %beta, align 4
  %add = add nsw i32 %20, %19
  store i32 %add, i32* %beta, align 4
  %21 = load i32, i32* %x_new, align 4
  store i32 %21, i32* %x, align 4
  %22 = load i32, i32* %x, align 4
  %23 = load i32, i32* %y, align 4
  %24 = load i32, i32* %k, align 4
  %shr22 = ashr i32 %23, %24
  %sub23 = sub nsw i32 %22, %shr22
  store i32 %sub23, i32* %x_new, align 4
  %25 = load i32, i32* %x, align 4
  %26 = load i32, i32* %k, align 4
  %shr24 = ashr i32 %25, %26
  %27 = load i32, i32* %y, align 4
  %sub25 = sub nsw i32 %27, %shr24
  store i32 %sub25, i32* %y, align 4
  %28 = load i32, i32* %index_trigo, align 4
  %29 = load i32, i32* %beta, align 4
  %add26 = add nsw i32 %29, %28
  store i32 %add26, i32* %beta, align 4
  %30 = load i32, i32* %x_new, align 4
  store i32 %30, i32* %x, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body18
  %31 = load i32, i32* %k, align 4
  %inc = add nsw i32 %31, 1
  store i32 %inc, i32* %k, align 4
  br label %for.cond16

for.end:                                          ; preds = %for.cond16
  %32 = load i32, i32* %x, align 4
  store i32 %32, i32* %outputcosh, align 4
  %33 = load i32, i32* %y, align 4
  store i32 %33, i32* %outputsinh, align 4
  %34 = load i32, i32* %index_trigo, align 4
  %35 = load i32, i32* %outputcosh, align 4
  %mul27 = mul nsw i32 %34, %35
  %36 = load i32, i32* %index_trigo, align 4
  %37 = load i32, i32* %outputsinh, align 4
  %mul28 = mul nsw i32 %36, %37
  %add29 = add nsw i32 %mul27, %mul28
  store i32 %add29, i32* %result_cosh, align 4
  %38 = load i32, i32* %index_trigo, align 4
  %39 = load i32, i32* %outputcosh, align 4
  %mul30 = mul nsw i32 %38, %39
  %40 = load i32, i32* %index_trigo, align 4
  %41 = load i32, i32* %outputsinh, align 4
  %mul31 = mul nsw i32 %40, %41
  %add32 = add nsw i32 %mul30, %mul31
  %shr33 = ashr i32 %add32, 12
  store i32 %shr33, i32* %result_sinh, align 4
  %42 = load i32, i32* %result_cosh, align 4
  %or = or i32 %42, 1
  %43 = load i32, i32* %result_sinh, align 4
  %or34 = or i32 %43, 1
  %mul35 = mul nsw i32 %or, %or34
  store i32 %mul35, i32* %result, align 4
  %44 = load i32, i32* %result, align 4
  %45 = load i32*, i32** %A.addr, align 8
  %46 = load i32*, i32** %addr.addr, align 8
  %47 = load i32, i32* %i, align 4
  %idxprom36 = sext i32 %47 to i64
  %arrayidx37 = getelementptr inbounds i32, i32* %46, i64 %idxprom36
  %48 = load i32, i32* %arrayidx37, align 4
  %idxprom38 = sext i32 %48 to i64
  %arrayidx39 = getelementptr inbounds i32, i32* %45, i64 %idxprom38
  store i32 %44, i32* %arrayidx39, align 4
  br label %for.inc40

for.inc40:                                        ; preds = %for.end
  %49 = load i32, i32* %i, align 4
  %inc41 = add nsw i32 %49, 1
  store i32 %inc41, i32* %i, align 4
  br label %for.cond

for.end42:                                        ; preds = %for.cond
  %50 = load i32, i32* %result, align 4
  ret i32 %50
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %a = alloca [1 x [1000 x i32]], align 16
  %b = alloca [1 x [1000 x i32]], align 16
  %atanh = alloca [1 x [12 x i32]], align 16
  %cosh = alloca [1 x [5 x i32]], align 16
  %sinh = alloca [1 x [5 x i32]], align 16
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %i17 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  %0 = bitcast [1 x [12 x i32]]* %atanh to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %0, i8* bitcast ([1 x [12 x i32]]* @_ZZ4mainE5atanh to i8*), i64 48, i32 16, i1 false)
  %1 = bitcast [1 x [5 x i32]]* %cosh to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %1, i8* bitcast ([1 x [5 x i32]]* @_ZZ4mainE4cosh to i8*), i64 20, i32 16, i1 false)
  %2 = bitcast [1 x [5 x i32]]* %sinh to i8*
  call void @llvm.memcpy.p0i8.p0i8.i64(i8* %2, i8* bitcast ([1 x [5 x i32]]* @_ZZ4mainE4sinh to i8*), i64 20, i32 16, i1 false)
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc14, %entry
  %3 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %3, 1
  br i1 %cmp, label %for.body, label %for.end16

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %4 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %4, 1000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %5 = load i32, i32* %j, align 4
  %6 = load i32, i32* %i, align 4
  %idxprom = sext i32 %6 to i64
  %arrayidx = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom
  %7 = load i32, i32* %j, align 4
  %idxprom4 = sext i32 %7 to i64
  %arrayidx5 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %5, i32* %arrayidx5, align 4
  %8 = load i32, i32* %j, align 4
  %9 = load i32, i32* %i, align 4
  %idxprom6 = sext i32 %9 to i64
  %arrayidx7 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 %idxprom6
  %10 = load i32, i32* %j, align 4
  %idxprom8 = sext i32 %10 to i64
  %arrayidx9 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 %8, i32* %arrayidx9, align 4
  %11 = load i32, i32* %i, align 4
  %idxprom10 = sext i32 %11 to i64
  %arrayidx11 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom10
  %12 = load i32, i32* %j, align 4
  %idxprom12 = sext i32 %12 to i64
  %arrayidx13 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 0, i32* %arrayidx13, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %13 = load i32, i32* %j, align 4
  %inc = add nsw i32 %13, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc14

for.inc14:                                        ; preds = %for.end
  %14 = load i32, i32* %i, align 4
  %inc15 = add nsw i32 %14, 1
  store i32 %inc15, i32* %i, align 4
  br label %for.cond

for.end16:                                        ; preds = %for.cond
  store i32 0, i32* %i17, align 4
  %15 = load i32, i32* %i17, align 4
  %idxprom18 = sext i32 %15 to i64
  %arrayidx19 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 %idxprom18
  %arraydecay = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx19, i32 0, i32 0
  %16 = load i32, i32* %i17, align 4
  %idxprom20 = sext i32 %16 to i64
  %arrayidx21 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 %idxprom20
  %arraydecay22 = getelementptr inbounds [1000 x i32], [1000 x i32]* %arrayidx21, i32 0, i32 0
  %call = call i32 @_Z7getTanhPiS_(i32* %arraydecay, i32* %arraydecay22)
  %17 = load i32, i32* %retval, align 4
  ret i32 %17
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
