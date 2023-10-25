; ModuleID = 'src/bnn.cpp'
source_filename = "src/bnn.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

; Function Attrs: noinline nounwind uwtable
define i32 @_Z3bnnPiS_S_S_S_S_(i32* %w, i32* %in, i32* %mean, i32* %addr_in, i32* %addr_out, i32* %data) #0 {
entry:
  %w.addr = alloca i32*, align 8
  %in.addr = alloca i32*, align 8
  %mean.addr = alloca i32*, align 8
  %addr_in.addr = alloca i32*, align 8
  %addr_out.addr = alloca i32*, align 8
  %data.addr = alloca i32*, align 8
  %alpha = alloca i32, align 4
  %i = alloca i32, align 4
  %k = alloca i32, align 4
  %j = alloca i32, align 4
  %x = alloca i32, align 4
  %lut = alloca i32, align 4
  %y = alloca i32, align 4
  %m = alloca i32, align 4
  %temp = alloca i32, align 4
  %z = alloca i32, align 4
  store i32* %w, i32** %w.addr, align 8
  store i32* %in, i32** %in.addr, align 8
  store i32* %mean, i32** %mean.addr, align 8
  store i32* %addr_in, i32** %addr_in.addr, align 8
  store i32* %addr_out, i32** %addr_out.addr, align 8
  store i32* %data, i32** %data.addr, align 8
  store i32 2, i32* %alpha, align 4
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc12, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 100
  br i1 %cmp, label %for.body, label %for.end14

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %1, 100
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %2 = load i32, i32* %i, align 4
  %mul = mul nsw i32 %2, 100
  %3 = load i32, i32* %j, align 4
  %add = add nsw i32 %mul, %3
  store i32 %add, i32* %x, align 4
  %4 = load i32*, i32** %in.addr, align 8
  %5 = load i32, i32* %x, align 4
  %idxprom = sext i32 %5 to i64
  %arrayidx = getelementptr inbounds i32, i32* %4, i64 %idxprom
  %6 = load i32, i32* %arrayidx, align 4
  %7 = load i32*, i32** %w.addr, align 8
  %8 = load i32, i32* %x, align 4
  %idxprom4 = sext i32 %8 to i64
  %arrayidx5 = getelementptr inbounds i32, i32* %7, i64 %idxprom4
  %9 = load i32, i32* %arrayidx5, align 4
  %xor = xor i32 %6, %9
  store i32 %xor, i32* %lut, align 4
  %10 = load i32, i32* %lut, align 4
  %11 = load i32, i32* %alpha, align 4
  %mul6 = mul nsw i32 %10, %11
  %12 = load i32*, i32** %data.addr, align 8
  %13 = load i32*, i32** %addr_in.addr, align 8
  %14 = load i32, i32* %x, align 4
  %idxprom7 = sext i32 %14 to i64
  %arrayidx8 = getelementptr inbounds i32, i32* %13, i64 %idxprom7
  %15 = load i32, i32* %arrayidx8, align 4
  %idxprom9 = sext i32 %15 to i64
  %arrayidx10 = getelementptr inbounds i32, i32* %12, i64 %idxprom9
  %16 = load i32, i32* %arrayidx10, align 4
  %add11 = add nsw i32 %16, %mul6
  store i32 %add11, i32* %arrayidx10, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %17 = load i32, i32* %j, align 4
  %inc = add nsw i32 %17, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc12

for.inc12:                                        ; preds = %for.end
  %18 = load i32, i32* %i, align 4
  %inc13 = add nsw i32 %18, 1
  store i32 %inc13, i32* %i, align 4
  br label %for.cond

for.end14:                                        ; preds = %for.cond
  store i32 0, i32* %k, align 4
  br label %for.cond15

for.cond15:                                       ; preds = %for.inc33, %for.end14
  %19 = load i32, i32* %k, align 4
  %cmp16 = icmp slt i32 %19, 100
  br i1 %cmp16, label %for.body17, label %for.end35

for.body17:                                       ; preds = %for.cond15
  %20 = load i32, i32* %i, align 4
  %sub = sub nsw i32 %20, 1
  %mul18 = mul nsw i32 %sub, 100
  %21 = load i32, i32* %k, align 4
  %add19 = add nsw i32 %mul18, %21
  store i32 %add19, i32* %y, align 4
  %22 = load i32*, i32** %mean.addr, align 8
  %23 = load i32, i32* %y, align 4
  %idxprom20 = sext i32 %23 to i64
  %arrayidx21 = getelementptr inbounds i32, i32* %22, i64 %idxprom20
  %24 = load i32, i32* %arrayidx21, align 4
  store i32 %24, i32* %m, align 4
  %25 = load i32*, i32** %data.addr, align 8
  %26 = load i32*, i32** %addr_out.addr, align 8
  %27 = load i32, i32* %y, align 4
  %idxprom22 = sext i32 %27 to i64
  %arrayidx23 = getelementptr inbounds i32, i32* %26, i64 %idxprom22
  %28 = load i32, i32* %arrayidx23, align 4
  %idxprom24 = sext i32 %28 to i64
  %arrayidx25 = getelementptr inbounds i32, i32* %25, i64 %idxprom24
  %29 = load i32, i32* %arrayidx25, align 4
  store i32 %29, i32* %temp, align 4
  %30 = load i32, i32* %temp, align 4
  %cmp26 = icmp sgt i32 %30, 0
  br i1 %cmp26, label %if.then, label %if.else

if.then:                                          ; preds = %for.body17
  %31 = load i32, i32* %temp, align 4
  %32 = load i32, i32* %m, align 4
  %sub27 = sub nsw i32 %31, %32
  store i32 %sub27, i32* %z, align 4
  br label %if.end

if.else:                                          ; preds = %for.body17
  %33 = load i32, i32* %temp, align 4
  %34 = load i32, i32* %m, align 4
  %add28 = add nsw i32 %33, %34
  store i32 %add28, i32* %z, align 4
  br label %if.end

if.end:                                           ; preds = %if.else, %if.then
  %35 = load i32, i32* %z, align 4
  %36 = load i32*, i32** %data.addr, align 8
  %37 = load i32*, i32** %addr_out.addr, align 8
  %38 = load i32, i32* %y, align 4
  %idxprom29 = sext i32 %38 to i64
  %arrayidx30 = getelementptr inbounds i32, i32* %37, i64 %idxprom29
  %39 = load i32, i32* %arrayidx30, align 4
  %idxprom31 = sext i32 %39 to i64
  %arrayidx32 = getelementptr inbounds i32, i32* %36, i64 %idxprom31
  store i32 %35, i32* %arrayidx32, align 4
  br label %for.inc33

for.inc33:                                        ; preds = %if.end
  %40 = load i32, i32* %k, align 4
  %inc34 = add nsw i32 %40, 1
  store i32 %inc34, i32* %k, align 4
  br label %for.cond15

for.end35:                                        ; preds = %for.cond15
  %41 = load i32, i32* %i, align 4
  ret i32 %41
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %retval = alloca i32, align 4
  %addr_in = alloca [1 x [10000 x i32]], align 16
  %addr_out = alloca [1 x [10000 x i32]], align 16
  %data = alloca [1 x [10000 x i32]], align 16
  %w = alloca [1 x [10000 x i32]], align 16
  %in = alloca [1 x [10000 x i32]], align 16
  %mean = alloca [1 x [10000 x i32]], align 16
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %i29 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  call void @srand(i32 13) #3
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc26, %entry
  %0 = load i32, i32* %i, align 4
  %cmp = icmp slt i32 %0, 1
  br i1 %cmp, label %for.body, label %for.end28

for.body:                                         ; preds = %for.cond
  store i32 0, i32* %j, align 4
  br label %for.cond1

for.cond1:                                        ; preds = %for.inc, %for.body
  %1 = load i32, i32* %j, align 4
  %cmp2 = icmp slt i32 %1, 10000
  br i1 %cmp2, label %for.body3, label %for.end

for.body3:                                        ; preds = %for.cond1
  %2 = load i32, i32* %j, align 4
  %3 = load i32, i32* %i, align 4
  %idxprom = sext i32 %3 to i64
  %arrayidx = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_in, i64 0, i64 %idxprom
  %4 = load i32, i32* %j, align 4
  %idxprom4 = sext i32 %4 to i64
  %arrayidx5 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx, i64 0, i64 %idxprom4
  store i32 %2, i32* %arrayidx5, align 4
  %5 = load i32, i32* %j, align 4
  %6 = load i32, i32* %i, align 4
  %idxprom6 = sext i32 %6 to i64
  %arrayidx7 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_out, i64 0, i64 %idxprom6
  %7 = load i32, i32* %j, align 4
  %idxprom8 = sext i32 %7 to i64
  %arrayidx9 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx7, i64 0, i64 %idxprom8
  store i32 %5, i32* %arrayidx9, align 4
  %call = call i32 @rand() #3
  %rem = srem i32 %call, 100
  %8 = load i32, i32* %i, align 4
  %idxprom10 = sext i32 %8 to i64
  %arrayidx11 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %data, i64 0, i64 %idxprom10
  %9 = load i32, i32* %j, align 4
  %idxprom12 = sext i32 %9 to i64
  %arrayidx13 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx11, i64 0, i64 %idxprom12
  store i32 %rem, i32* %arrayidx13, align 4
  %10 = load i32, i32* %i, align 4
  %idxprom14 = sext i32 %10 to i64
  %arrayidx15 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %w, i64 0, i64 %idxprom14
  %11 = load i32, i32* %j, align 4
  %idxprom16 = sext i32 %11 to i64
  %arrayidx17 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx15, i64 0, i64 %idxprom16
  store i32 7, i32* %arrayidx17, align 4
  %12 = load i32, i32* %i, align 4
  %idxprom18 = sext i32 %12 to i64
  %arrayidx19 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %in, i64 0, i64 %idxprom18
  %13 = load i32, i32* %j, align 4
  %idxprom20 = sext i32 %13 to i64
  %arrayidx21 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx19, i64 0, i64 %idxprom20
  store i32 5, i32* %arrayidx21, align 4
  %14 = load i32, i32* %i, align 4
  %idxprom22 = sext i32 %14 to i64
  %arrayidx23 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %mean, i64 0, i64 %idxprom22
  %15 = load i32, i32* %j, align 4
  %idxprom24 = sext i32 %15 to i64
  %arrayidx25 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx23, i64 0, i64 %idxprom24
  store i32 3, i32* %arrayidx25, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body3
  %16 = load i32, i32* %j, align 4
  %inc = add nsw i32 %16, 1
  store i32 %inc, i32* %j, align 4
  br label %for.cond1

for.end:                                          ; preds = %for.cond1
  br label %for.inc26

for.inc26:                                        ; preds = %for.end
  %17 = load i32, i32* %i, align 4
  %inc27 = add nsw i32 %17, 1
  store i32 %inc27, i32* %i, align 4
  br label %for.cond

for.end28:                                        ; preds = %for.cond
  store i32 0, i32* %i29, align 4
  %18 = load i32, i32* %i29, align 4
  %idxprom30 = sext i32 %18 to i64
  %arrayidx31 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %w, i64 0, i64 %idxprom30
  %arraydecay = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx31, i32 0, i32 0
  %19 = load i32, i32* %i29, align 4
  %idxprom32 = sext i32 %19 to i64
  %arrayidx33 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %in, i64 0, i64 %idxprom32
  %arraydecay34 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx33, i32 0, i32 0
  %20 = load i32, i32* %i29, align 4
  %idxprom35 = sext i32 %20 to i64
  %arrayidx36 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %mean, i64 0, i64 %idxprom35
  %arraydecay37 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx36, i32 0, i32 0
  %21 = load i32, i32* %i29, align 4
  %idxprom38 = sext i32 %21 to i64
  %arrayidx39 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_in, i64 0, i64 %idxprom38
  %arraydecay40 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx39, i32 0, i32 0
  %22 = load i32, i32* %i29, align 4
  %idxprom41 = sext i32 %22 to i64
  %arrayidx42 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %addr_out, i64 0, i64 %idxprom41
  %arraydecay43 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx42, i32 0, i32 0
  %23 = load i32, i32* %i29, align 4
  %idxprom44 = sext i32 %23 to i64
  %arrayidx45 = getelementptr inbounds [1 x [10000 x i32]], [1 x [10000 x i32]]* %data, i64 0, i64 %idxprom44
  %arraydecay46 = getelementptr inbounds [10000 x i32], [10000 x i32]* %arrayidx45, i32 0, i32 0
  %call47 = call i32 @_Z3bnnPiS_S_S_S_S_(i32* %arraydecay, i32* %arraydecay34, i32* %arraydecay37, i32* %arraydecay40, i32* %arraydecay43, i32* %arraydecay46)
  %24 = load i32, i32* %retval, align 4
  ret i32 %24
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
