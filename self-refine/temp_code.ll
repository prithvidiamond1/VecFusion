; ModuleID = 'temp_code.cpp'
source_filename = "temp_code.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: mustprogress nounwind ssp uwtable(sync)
define void @_Z8s112_optPfS_ii(ptr nocapture noundef %0, ptr nocapture noundef readonly %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #0 !dbg !9 {
  %5 = sext i32 %3 to i64, !dbg !12
  %6 = shl nsw i64 %5, 2, !dbg !13
  %7 = tail call ptr @malloc(i64 noundef %6) #5, !dbg !14
  %8 = icmp slt i32 %2, 1, !dbg !15
  %9 = icmp slt i32 %3, 2
  %10 = or i1 %8, %9, !dbg !16
  br i1 %10, label %30, label %11, !dbg !16

11:                                               ; preds = %4
  %12 = mul i32 %2, 3
  %13 = ptrtoint ptr %0 to i64, !dbg !12
  %14 = ptrtoint ptr %1 to i64, !dbg !12
  %15 = add nsw i32 %3, -1, !dbg !16
  %16 = tail call i32 @llvm.smax.i32(i32 %12, i32 1), !dbg !16
  %17 = zext i32 %15 to i64
  %18 = add i64 %13, 4, !dbg !16
  %19 = sub i64 %18, %14, !dbg !16
  %20 = icmp ult i32 %3, 5
  %21 = icmp ult i64 %19, 64
  %22 = or i1 %20, %21
  %23 = icmp ult i32 %3, 17
  %24 = and i64 %17, 4294967280
  %25 = icmp eq i64 %24, %17
  %26 = and i64 %17, 12
  %27 = icmp eq i64 %26, 0
  %28 = and i64 %17, 4294967292
  %29 = icmp eq i64 %28, %17
  br label %31, !dbg !16

30:                                               ; preds = %81, %4
  tail call void @free(ptr noundef %7), !dbg !17
  ret void, !dbg !18

31:                                               ; preds = %11, %81
  %32 = phi i32 [ %82, %81 ], [ 0, %11 ]
  tail call void @llvm.memcpy.p0.p0.i64(ptr align 4 %7, ptr align 4 %0, i64 %6, i1 false), !dbg !19
  br i1 %22, label %33, label %35, !dbg !20

33:                                               ; preds = %66, %80, %31
  %34 = phi i64 [ %24, %66 ], [ 0, %31 ], [ %28, %80 ]
  br label %84, !dbg !20

35:                                               ; preds = %31
  br i1 %23, label %67, label %36, !dbg !20

36:                                               ; preds = %35, %36
  %37 = phi i64 [ %63, %36 ], [ 0, %35 ], !dbg !21
  %38 = getelementptr inbounds nuw float, ptr %7, i64 %37, !dbg !22
  %39 = getelementptr inbounds nuw i8, ptr %38, i64 16, !dbg !22
  %40 = getelementptr inbounds nuw i8, ptr %38, i64 32, !dbg !22
  %41 = getelementptr inbounds nuw i8, ptr %38, i64 48, !dbg !22
  %42 = load <4 x float>, ptr %38, align 4, !dbg !22, !tbaa !23
  %43 = load <4 x float>, ptr %39, align 4, !dbg !22, !tbaa !23
  %44 = load <4 x float>, ptr %40, align 4, !dbg !22, !tbaa !23
  %45 = load <4 x float>, ptr %41, align 4, !dbg !22, !tbaa !23
  %46 = getelementptr inbounds nuw float, ptr %1, i64 %37, !dbg !27
  %47 = getelementptr inbounds nuw i8, ptr %46, i64 16, !dbg !27
  %48 = getelementptr inbounds nuw i8, ptr %46, i64 32, !dbg !27
  %49 = getelementptr inbounds nuw i8, ptr %46, i64 48, !dbg !27
  %50 = load <4 x float>, ptr %46, align 4, !dbg !27, !tbaa !23
  %51 = load <4 x float>, ptr %47, align 4, !dbg !27, !tbaa !23
  %52 = load <4 x float>, ptr %48, align 4, !dbg !27, !tbaa !23
  %53 = load <4 x float>, ptr %49, align 4, !dbg !27, !tbaa !23
  %54 = fadd fast <4 x float> %50, %42, !dbg !28
  %55 = fadd fast <4 x float> %51, %43, !dbg !28
  %56 = fadd fast <4 x float> %52, %44, !dbg !28
  %57 = fadd fast <4 x float> %53, %45, !dbg !28
  %58 = or disjoint i64 %37, 1, !dbg !21
  %59 = getelementptr inbounds nuw float, ptr %0, i64 %58, !dbg !29
  %60 = getelementptr inbounds nuw i8, ptr %59, i64 16, !dbg !30
  %61 = getelementptr inbounds nuw i8, ptr %59, i64 32, !dbg !30
  %62 = getelementptr inbounds nuw i8, ptr %59, i64 48, !dbg !30
  store <4 x float> %54, ptr %59, align 4, !dbg !30, !tbaa !23
  store <4 x float> %55, ptr %60, align 4, !dbg !30, !tbaa !23
  store <4 x float> %56, ptr %61, align 4, !dbg !30, !tbaa !23
  store <4 x float> %57, ptr %62, align 4, !dbg !30, !tbaa !23
  %63 = add nuw i64 %37, 16, !dbg !21
  %64 = icmp eq i64 %63, %24, !dbg !21
  br i1 %64, label %65, label %36, !dbg !21, !llvm.loop !31

65:                                               ; preds = %36
  br i1 %25, label %81, label %66, !dbg !20

66:                                               ; preds = %65
  br i1 %27, label %33, label %67, !dbg !20

67:                                               ; preds = %66, %35
  %68 = phi i64 [ %24, %66 ], [ 0, %35 ]
  br label %69, !dbg !20

69:                                               ; preds = %69, %67
  %70 = phi i64 [ %68, %67 ], [ %78, %69 ], !dbg !21
  %71 = getelementptr inbounds nuw float, ptr %7, i64 %70, !dbg !22
  %72 = load <4 x float>, ptr %71, align 4, !dbg !22, !tbaa !23
  %73 = getelementptr inbounds nuw float, ptr %1, i64 %70, !dbg !27
  %74 = load <4 x float>, ptr %73, align 4, !dbg !27, !tbaa !23
  %75 = fadd fast <4 x float> %74, %72, !dbg !28
  %76 = or disjoint i64 %70, 1, !dbg !21
  %77 = getelementptr inbounds nuw float, ptr %0, i64 %76, !dbg !29
  store <4 x float> %75, ptr %77, align 4, !dbg !30, !tbaa !23
  %78 = add nuw i64 %70, 4, !dbg !21
  %79 = icmp eq i64 %78, %28, !dbg !21
  br i1 %79, label %80, label %69, !dbg !21, !llvm.loop !36

80:                                               ; preds = %69
  br i1 %29, label %81, label %33, !dbg !20

81:                                               ; preds = %84, %80, %65
  %82 = add nuw nsw i32 %32, 1, !dbg !37
  %83 = icmp eq i32 %82, %16, !dbg !15
  br i1 %83, label %30, label %31, !dbg !16, !llvm.loop !38

84:                                               ; preds = %33, %84
  %85 = phi i64 [ %91, %84 ], [ %34, %33 ]
  %86 = getelementptr inbounds nuw float, ptr %7, i64 %85, !dbg !22
  %87 = load float, ptr %86, align 4, !dbg !22, !tbaa !23
  %88 = getelementptr inbounds nuw float, ptr %1, i64 %85, !dbg !27
  %89 = load float, ptr %88, align 4, !dbg !27, !tbaa !23
  %90 = fadd fast float %89, %87, !dbg !28
  %91 = add nuw nsw i64 %85, 1, !dbg !21
  %92 = getelementptr inbounds nuw float, ptr %0, i64 %91, !dbg !29
  store float %90, ptr %92, align 4, !dbg !30, !tbaa !23
  %93 = icmp eq i64 %91, %17, !dbg !40
  br i1 %93, label %81, label %84, !dbg !20, !llvm.loop !41
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
!12 = !DILocation(line: 8, column: 34, scope: !9)
!13 = !DILocation(line: 8, column: 41, scope: !9)
!14 = !DILocation(line: 8, column: 27, scope: !9)
!15 = !DILocation(line: 9, column: 25, scope: !9)
!16 = !DILocation(line: 9, column: 5, scope: !9)
!17 = !DILocation(line: 15, column: 5, scope: !9)
!18 = !DILocation(line: 16, column: 1, scope: !9)
!19 = !DILocation(line: 10, column: 9, scope: !9)
!20 = !DILocation(line: 11, column: 9, scope: !9)
!21 = !DILocation(line: 12, column: 17, scope: !9)
!22 = !DILocation(line: 12, column: 24, scope: !9)
!23 = !{!24, !24, i64 0}
!24 = !{!"float", !25, i64 0}
!25 = !{!"omnipotent char", !26, i64 0}
!26 = !{!"Simple C++ TBAA"}
!27 = !DILocation(line: 12, column: 33, scope: !9)
!28 = !DILocation(line: 12, column: 31, scope: !9)
!29 = !DILocation(line: 12, column: 13, scope: !9)
!30 = !DILocation(line: 12, column: 22, scope: !9)
!31 = distinct !{!31, !20, !32, !33, !34, !35}
!32 = !DILocation(line: 13, column: 9, scope: !9)
!33 = !{!"llvm.loop.mustprogress"}
!34 = !{!"llvm.loop.isvectorized", i32 1}
!35 = !{!"llvm.loop.unroll.runtime.disable"}
!36 = distinct !{!36, !20, !32, !33, !34, !35}
!37 = !DILocation(line: 9, column: 45, scope: !9)
!38 = distinct !{!38, !16, !39, !33}
!39 = !DILocation(line: 14, column: 5, scope: !9)
!40 = !DILocation(line: 11, column: 27, scope: !9)
!41 = distinct !{!41, !20, !32, !33, !34}
