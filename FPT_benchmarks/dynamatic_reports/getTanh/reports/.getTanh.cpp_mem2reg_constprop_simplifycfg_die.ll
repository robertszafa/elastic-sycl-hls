; ModuleID = '.getTanh.cpp_mem2reg_constprop_simplifycfg.ll'
source_filename = "src/getTanh.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@_ZZ4mainE5atanh = private unnamed_addr constant [1 x [12 x i32]] [[12 x i32] [i32 2249, i32 1046, i32 514, i32 256, i32 128, i32 100, i32 50, i32 16, i32 8, i32 4, i32 2, i32 1]], align 16
@_ZZ4mainE4cosh = private unnamed_addr constant [1 x [5 x i32]] [[5 x i32] [i32 4096, i32 6320, i32 15409, i32 41237, i32 111854]], align 16
@_ZZ4mainE4sinh = private unnamed_addr constant [1 x [5 x i32]] [[5 x i32] [i32 0, i32 4813, i32 14855, i32 41033, i32 111779]], align 16

; Function Attrs: noinline nounwind uwtable
define i32 @_Z7getTanhPiS_(i32* %A, i32* %addr) #0 {
entry:
  br label %for.body

for.body:                                         ; preds = %for.end, %entry
  %i.05 = phi i32 [ 0, %entry ], [ %inc41, %for.end ]
  %0 = zext i32 %i.05 to i64
  %arrayidx = getelementptr inbounds i32, i32* %addr, i64 %0
  %1 = load i32, i32* %arrayidx, align 4
  %idxprom1 = sext i32 %1 to i64
  %arrayidx2 = getelementptr inbounds i32, i32* %A, i64 %idxprom1
  %2 = load i32, i32* %arrayidx2, align 4
  %cmp3 = icmp sgt i32 %2, 8191
  %cmp10 = icmp sgt i32 %2, 4095
  %. = select i1 %cmp10, i32 5, i32 6
  %index_trigo.3 = select i1 %cmp3, i32 4, i32 %.
  %mul = shl nuw nsw i32 %index_trigo.3, 12
  %sub = sub nsw i32 %2, %mul
  br label %for.body18

for.body18:                                       ; preds = %for.body18, %for.body
  %k.04 = phi i32 [ 1, %for.body ], [ %inc, %for.body18 ]
  %beta.03 = phi i32 [ %sub, %for.body ], [ %add26, %for.body18 ]
  %x.02 = phi i32 [ 4945, %for.body ], [ %sub23, %for.body18 ]
  %y.01 = phi i32 [ 1, %for.body ], [ %sub25, %for.body18 ]
  %shr = ashr i32 %y.01, %k.04
  %sub19 = sub nsw i32 %x.02, %shr
  %shr20 = ashr i32 %x.02, %k.04
  %sub21 = sub nsw i32 %y.01, %shr20
  %add = add nsw i32 %beta.03, %index_trigo.3
  %shr22 = ashr i32 %sub21, %k.04
  %sub23 = sub nsw i32 %sub19, %shr22
  %shr24 = ashr i32 %sub19, %k.04
  %sub25 = sub nsw i32 %sub21, %shr24
  %add26 = add nsw i32 %add, %index_trigo.3
  %inc = add nuw nsw i32 %k.04, 1
  %cmp17 = icmp ult i32 %inc, 13
  br i1 %cmp17, label %for.body18, label %for.end

for.end:                                          ; preds = %for.body18
  %mul281 = add i32 %sub23, %sub25
  %add29 = mul i32 %index_trigo.3, %mul281
  %mul312 = add i32 %sub23, %sub25
  %add32 = mul i32 %index_trigo.3, %mul312
  %shr33 = ashr i32 %add32, 12
  %or = or i32 %add29, 1
  %or34 = or i32 %shr33, 1
  %mul35 = mul nsw i32 %or, %or34
  %3 = zext i32 %i.05 to i64
  %arrayidx37 = getelementptr inbounds i32, i32* %addr, i64 %3
  %4 = load i32, i32* %arrayidx37, align 4
  %idxprom38 = sext i32 %4 to i64
  %arrayidx39 = getelementptr inbounds i32, i32* %A, i64 %idxprom38
  store i32 %mul35, i32* %arrayidx39, align 4
  %inc41 = add nuw nsw i32 %i.05, 1
  %cmp = icmp ult i32 %inc41, 1000
  br i1 %cmp, label %for.body, label %for.end42

for.end42:                                        ; preds = %for.end
  ret i32 %mul35
}

; Function Attrs: noinline norecurse nounwind uwtable
define i32 @main() #1 {
entry:
  %a = alloca [1 x [1000 x i32]], align 16
  %b = alloca [1 x [1000 x i32]], align 16
  br label %for.body

for.body:                                         ; preds = %for.inc14, %entry
  br label %for.body3

for.body3:                                        ; preds = %for.body3, %for.body
  %j.01 = phi i32 [ 0, %for.body ], [ %inc, %for.body3 ]
  %0 = zext i32 %j.01 to i64
  %1 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 %0
  store i32 %j.01, i32* %1, align 4
  %2 = zext i32 %j.01 to i64
  %3 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 0, i64 %2
  store i32 %j.01, i32* %3, align 4
  %4 = zext i32 %j.01 to i64
  %5 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 %4
  store i32 0, i32* %5, align 4
  %inc = add nuw nsw i32 %j.01, 1
  %cmp2 = icmp ult i32 %inc, 1000
  br i1 %cmp2, label %for.body3, label %for.inc14

for.inc14:                                        ; preds = %for.body3
  br i1 false, label %for.body, label %for.end16

for.end16:                                        ; preds = %for.inc14
  %arraydecay = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %a, i64 0, i64 0, i64 0
  %arraydecay22 = getelementptr inbounds [1 x [1000 x i32]], [1 x [1000 x i32]]* %b, i64 0, i64 0, i64 0
  %call = call i32 @_Z7getTanhPiS_(i32* nonnull %arraydecay, i32* nonnull %arraydecay22)
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
