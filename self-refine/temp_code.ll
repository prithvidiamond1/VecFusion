; ModuleID = 'temp_code.cpp'
source_filename = "temp_code.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: mustprogress nounwind ssp uwtable(sync)
define void @_Z8s112_optPfS_ii(ptr nocapture noundef %0, ptr nocapture noundef readonly %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #0 !dbg !9 {
  %5 = ptrtoint ptr %1 to i64
  %6 = ptrtoint ptr %0 to i64
  %7 = mul i32 %2, 3
  %8 = icmp sgt i32 %2, 0, !dbg !12
  br i1 %8, label %9, label %29, !dbg !13

9:                                                ; preds = %4
  %10 = sext i32 %3 to i64
  %11 = shl nsw i64 %10, 2
  %12 = icmp slt i32 %3, 2
  br i1 %12, label %29, label %13

13:                                               ; preds = %9
  %14 = add nsw i32 %3, -1, !dbg !13
  %15 = tail call i32 @llvm.smax.i32(i32 %7, i32 1), !dbg !13
  %16 = zext i32 %14 to i64
  %17 = add i64 %6, 4, !dbg !13
  %18 = sub i64 %17, %5, !dbg !13
  %19 = icmp ult i32 %3, 5
  %20 = icmp ult i64 %18, 64
  %21 = or i1 %19, %20
  %22 = icmp ult i32 %3, 17
  %23 = and i64 %16, 4294967280
  %24 = icmp eq i64 %23, %16
  %25 = and i64 %16, 12
  %26 = icmp eq i64 %25, 0
  %27 = and i64 %16, 4294967292
  %28 = icmp eq i64 %27, %16
  br label %30, !dbg !13

29:                                               ; preds = %81, %9, %4
  ret void, !dbg !14

30:                                               ; preds = %13, %81
  %31 = phi i32 [ %82, %81 ], [ 0, %13 ]
  %32 = tail call ptr @malloc(i64 noundef %11) #5, !dbg !15
  tail call void @llvm.memcpy.p0.p0.i64(ptr align 4 %32, ptr align 4 %0, i64 %11, i1 false), !dbg !16
  br i1 %21, label %33, label %35, !dbg !17

33:                                               ; preds = %66, %80, %30
  %34 = phi i64 [ %23, %66 ], [ 0, %30 ], [ %27, %80 ]
  br label %84, !dbg !17

35:                                               ; preds = %30
  br i1 %22, label %67, label %36, !dbg !17

36:                                               ; preds = %35, %36
  %37 = phi i64 [ %63, %36 ], [ 0, %35 ], !dbg !18
  %38 = getelementptr inbounds nuw float, ptr %32, i64 %37, !dbg !19
  %39 = getelementptr inbounds nuw i8, ptr %38, i64 16, !dbg !19
  %40 = getelementptr inbounds nuw i8, ptr %38, i64 32, !dbg !19
  %41 = getelementptr inbounds nuw i8, ptr %38, i64 48, !dbg !19
  %42 = load <4 x float>, ptr %38, align 4, !dbg !19, !tbaa !20
  %43 = load <4 x float>, ptr %39, align 4, !dbg !19, !tbaa !20
  %44 = load <4 x float>, ptr %40, align 4, !dbg !19, !tbaa !20
  %45 = load <4 x float>, ptr %41, align 4, !dbg !19, !tbaa !20
  %46 = getelementptr inbounds nuw float, ptr %1, i64 %37, !dbg !24
  %47 = getelementptr inbounds nuw i8, ptr %46, i64 16, !dbg !24
  %48 = getelementptr inbounds nuw i8, ptr %46, i64 32, !dbg !24
  %49 = getelementptr inbounds nuw i8, ptr %46, i64 48, !dbg !24
  %50 = load <4 x float>, ptr %46, align 4, !dbg !24, !tbaa !20
  %51 = load <4 x float>, ptr %47, align 4, !dbg !24, !tbaa !20
  %52 = load <4 x float>, ptr %48, align 4, !dbg !24, !tbaa !20
  %53 = load <4 x float>, ptr %49, align 4, !dbg !24, !tbaa !20
  %54 = fadd fast <4 x float> %50, %42, !dbg !25
  %55 = fadd fast <4 x float> %51, %43, !dbg !25
  %56 = fadd fast <4 x float> %52, %44, !dbg !25
  %57 = fadd fast <4 x float> %53, %45, !dbg !25
  %58 = or disjoint i64 %37, 1, !dbg !18
  %59 = getelementptr inbounds nuw float, ptr %0, i64 %58, !dbg !26
  %60 = getelementptr inbounds nuw i8, ptr %59, i64 16, !dbg !27
  %61 = getelementptr inbounds nuw i8, ptr %59, i64 32, !dbg !27
  %62 = getelementptr inbounds nuw i8, ptr %59, i64 48, !dbg !27
  store <4 x float> %54, ptr %59, align 4, !dbg !27, !tbaa !20
  store <4 x float> %55, ptr %60, align 4, !dbg !27, !tbaa !20
  store <4 x float> %56, ptr %61, align 4, !dbg !27, !tbaa !20
  store <4 x float> %57, ptr %62, align 4, !dbg !27, !tbaa !20
  %63 = add nuw i64 %37, 16, !dbg !18
  %64 = icmp eq i64 %63, %23, !dbg !18
  br i1 %64, label %65, label %36, !dbg !18, !llvm.loop !28

65:                                               ; preds = %36
  br i1 %24, label %81, label %66, !dbg !17

66:                                               ; preds = %65
  br i1 %26, label %33, label %67, !dbg !17

67:                                               ; preds = %66, %35
  %68 = phi i64 [ %23, %66 ], [ 0, %35 ]
  br label %69, !dbg !17

69:                                               ; preds = %69, %67
  %70 = phi i64 [ %68, %67 ], [ %78, %69 ], !dbg !18
  %71 = getelementptr inbounds nuw float, ptr %32, i64 %70, !dbg !19
  %72 = load <4 x float>, ptr %71, align 4, !dbg !19, !tbaa !20
  %73 = getelementptr inbounds nuw float, ptr %1, i64 %70, !dbg !24
  %74 = load <4 x float>, ptr %73, align 4, !dbg !24, !tbaa !20
  %75 = fadd fast <4 x float> %74, %72, !dbg !25
  %76 = or disjoint i64 %70, 1, !dbg !18
  %77 = getelementptr inbounds nuw float, ptr %0, i64 %76, !dbg !26
  store <4 x float> %75, ptr %77, align 4, !dbg !27, !tbaa !20
  %78 = add nuw i64 %70, 4, !dbg !18
  %79 = icmp eq i64 %78, %27, !dbg !18
  br i1 %79, label %80, label %69, !dbg !18, !llvm.loop !33

80:                                               ; preds = %69
  br i1 %28, label %81, label %33, !dbg !17

81:                                               ; preds = %84, %80, %65
  tail call void @free(ptr noundef nonnull %32), !dbg !34
  %82 = add nuw nsw i32 %31, 1, !dbg !35
  %83 = icmp eq i32 %82, %15, !dbg !12
  br i1 %83, label %29, label %30, !dbg !13, !llvm.loop !36

84:                                               ; preds = %33, %84
  %85 = phi i64 [ %91, %84 ], [ %34, %33 ]
  %86 = getelementptr inbounds nuw float, ptr %32, i64 %85, !dbg !19
  %87 = load float, ptr %86, align 4, !dbg !19, !tbaa !20
  %88 = getelementptr inbounds nuw float, ptr %1, i64 %85, !dbg !24
  %89 = load float, ptr %88, align 4, !dbg !24, !tbaa !20
  %90 = fadd fast float %89, %87, !dbg !25
  %91 = add nuw nsw i64 %85, 1, !dbg !18
  %92 = getelementptr inbounds nuw float, ptr %0, i64 %91, !dbg !26
  store float %90, ptr %92, align 4, !dbg !27, !tbaa !20
  %93 = icmp eq i64 %91, %16, !dbg !38
  br i1 %93, label %81, label %84, !dbg !17, !llvm.loop !39
}

; Function Attrs: mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite)
declare noalias noundef ptr @malloc(i64 noundef) local_unnamed_addr #1

; Function Attrs: mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite)
declare void @llvm.memcpy.p0.p0.i64(ptr noalias nocapture writeonly, ptr noalias nocapture readonly, i64, i1 immarg) #2

; Function Attrs: mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite)
declare void @free(ptr allocptr nocapture noundef) local_unnamed_addr #3

; Function Attrs: nocallback nofree nosync nounwind speculatable willreturn memory(none)
declare i32 @llvm.smax.i32(i32, i32) #4

attributes #0 = { mustprogress nounwind ssp uwtable(sync) "approx-func-fp-math"="true" "frame-pointer"="non-leaf" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" "unsafe-fp-math"="true" }
attributes #1 = { mustprogress nofree nounwind willreturn allockind("alloc,uninitialized") allocsize(0) memory(inaccessiblemem: readwrite) "alloc-family"="malloc" "approx-func-fp-math"="true" "frame-pointer"="non-leaf" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" "unsafe-fp-math"="true" }
attributes #2 = { mustprogress nocallback nofree nounwind willreturn memory(argmem: readwrite) }
attributes #3 = { mustprogress nounwind willreturn allockind("free") memory(argmem: readwrite, inaccessiblemem: readwrite) "alloc-family"="malloc" "approx-func-fp-math"="true" "frame-pointer"="non-leaf" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" "unsafe-fp-math"="true" }
attributes #4 = { nocallback nofree nosync nounwind speculatable willreturn memory(none) }
attributes #5 = { allocsize(0) }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.dbg.cu = !{!6}
!llvm.ident = !{!8}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 26, i32 2]}
!1 = !{i32 2, !"Debug Info Version", i32 3}
!2 = !{i32 1, !"wchar_size", i32 4}
!3 = !{i32 8, !"PIC Level", i32 2}
!4 = !{i32 7, !"uwtable", i32 1}
!5 = !{i32 7, !"frame-pointer", i32 1}
!6 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !7, producer: "clang version 20.1.8 (https://github.com/llvm/llvm-project.git 87f0227cb60147a26a1eeb4fb06e3b505e9c7261)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: Apple, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX26.2.sdk", sdk: "MacOSX26.2.sdk")
!7 = !DIFile(filename: "temp_code.cpp", directory: "/Users/torence/VecTrans/self-refine")
!8 = !{!"clang version 20.1.8 (https://github.com/llvm/llvm-project.git 87f0227cb60147a26a1eeb4fb06e3b505e9c7261)"}
!9 = distinct !DISubprogram(name: "s112_opt", scope: !7, file: !7, line: 7, type: !10, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !6)
!10 = !DISubroutineType(types: !11)
!11 = !{}
!12 = !DILocation(line: 8, column: 25, scope: !9)
!13 = !DILocation(line: 8, column: 5, scope: !9)
!14 = !DILocation(line: 16, column: 1, scope: !9)
!15 = !DILocation(line: 9, column: 32, scope: !9)
!16 = !DILocation(line: 10, column: 9, scope: !9)
!17 = !DILocation(line: 11, column: 9, scope: !9)
!18 = !DILocation(line: 12, column: 17, scope: !9)
!19 = !DILocation(line: 12, column: 24, scope: !9)
!20 = !{!21, !21, i64 0}
!21 = !{!"float", !22, i64 0}
!22 = !{!"omnipotent char", !23, i64 0}
!23 = !{!"Simple C++ TBAA"}
!24 = !DILocation(line: 12, column: 34, scope: !9)
!25 = !DILocation(line: 12, column: 32, scope: !9)
!26 = !DILocation(line: 12, column: 13, scope: !9)
!27 = !DILocation(line: 12, column: 22, scope: !9)
!28 = distinct !{!28, !17, !29, !30, !31, !32}
!29 = !DILocation(line: 13, column: 9, scope: !9)
!30 = !{!"llvm.loop.mustprogress"}
!31 = !{!"llvm.loop.isvectorized", i32 1}
!32 = !{!"llvm.loop.unroll.runtime.disable"}
!33 = distinct !{!33, !17, !29, !30, !31, !32}
!34 = !DILocation(line: 14, column: 9, scope: !9)
!35 = !DILocation(line: 8, column: 45, scope: !9)
!36 = distinct !{!36, !13, !37, !30}
!37 = !DILocation(line: 15, column: 5, scope: !9)
!38 = !DILocation(line: 11, column: 27, scope: !9)
!39 = distinct !{!39, !17, !29, !30, !31}
