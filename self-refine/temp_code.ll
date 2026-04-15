; ModuleID = 'temp_code.cpp'
source_filename = "temp_code.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: mustprogress nounwind ssp uwtable(sync)
define void @_Z8s112_optPfS_ii(ptr nocapture noundef %0, ptr nocapture noundef readonly %1, i32 noundef %2, i32 noundef %3) local_unnamed_addr #0 !dbg !9 {
  %5 = ptrtoint ptr %1 to i64, !dbg !12
  %6 = ptrtoint ptr %0 to i64, !dbg !12
  %7 = sext i32 %3 to i64, !dbg !12
  %8 = shl nsw i64 %7, 2, !dbg !13
  %9 = tail call ptr @malloc(i64 noundef %8) #5, !dbg !14
  %10 = icmp sgt i32 %2, 0, !dbg !15
  br i1 %10, label %11, label %93, !dbg !16

11:                                               ; preds = %4
  %12 = mul i32 %2, 3
  %13 = icmp sgt i32 %3, 1
  %14 = tail call i32 @llvm.smax.i32(i32 %12, i32 1), !dbg !16
  br i1 %13, label %15, label %93

15:                                               ; preds = %11
  %16 = add nsw i32 %3, -1
  %17 = zext i32 %16 to i64
  %18 = add i64 %6, 4, !dbg !16
  %19 = sub i64 %18, %5, !dbg !16
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
  br label %30, !dbg !16

30:                                               ; preds = %15, %90
  %31 = phi i32 [ %91, %90 ], [ 0, %15 ]
  tail call void @llvm.memcpy.p0.p0.i64(ptr align 4 %9, ptr align 4 %0, i64 %8, i1 false), !dbg !17
  br i1 %22, label %78, label %32, !dbg !18

32:                                               ; preds = %30
  br i1 %23, label %64, label %33, !dbg !18

33:                                               ; preds = %32, %33
  %34 = phi i64 [ %60, %33 ], [ 0, %32 ], !dbg !19
  %35 = getelementptr inbounds nuw float, ptr %9, i64 %34, !dbg !20
  %36 = getelementptr inbounds nuw i8, ptr %35, i64 16, !dbg !20
  %37 = getelementptr inbounds nuw i8, ptr %35, i64 32, !dbg !20
  %38 = getelementptr inbounds nuw i8, ptr %35, i64 48, !dbg !20
  %39 = load <4 x float>, ptr %35, align 4, !dbg !20, !tbaa !21
  %40 = load <4 x float>, ptr %36, align 4, !dbg !20, !tbaa !21
  %41 = load <4 x float>, ptr %37, align 4, !dbg !20, !tbaa !21
  %42 = load <4 x float>, ptr %38, align 4, !dbg !20, !tbaa !21
  %43 = getelementptr inbounds nuw float, ptr %1, i64 %34, !dbg !25
  %44 = getelementptr inbounds nuw i8, ptr %43, i64 16, !dbg !25
  %45 = getelementptr inbounds nuw i8, ptr %43, i64 32, !dbg !25
  %46 = getelementptr inbounds nuw i8, ptr %43, i64 48, !dbg !25
  %47 = load <4 x float>, ptr %43, align 4, !dbg !25, !tbaa !21
  %48 = load <4 x float>, ptr %44, align 4, !dbg !25, !tbaa !21
  %49 = load <4 x float>, ptr %45, align 4, !dbg !25, !tbaa !21
  %50 = load <4 x float>, ptr %46, align 4, !dbg !25, !tbaa !21
  %51 = fadd fast <4 x float> %47, %39, !dbg !26
  %52 = fadd fast <4 x float> %48, %40, !dbg !26
  %53 = fadd fast <4 x float> %49, %41, !dbg !26
  %54 = fadd fast <4 x float> %50, %42, !dbg !26
  %55 = or disjoint i64 %34, 1, !dbg !19
  %56 = getelementptr inbounds nuw float, ptr %0, i64 %55, !dbg !27
  %57 = getelementptr inbounds nuw i8, ptr %56, i64 16, !dbg !28
  %58 = getelementptr inbounds nuw i8, ptr %56, i64 32, !dbg !28
  %59 = getelementptr inbounds nuw i8, ptr %56, i64 48, !dbg !28
  store <4 x float> %51, ptr %56, align 4, !dbg !28, !tbaa !21
  store <4 x float> %52, ptr %57, align 4, !dbg !28, !tbaa !21
  store <4 x float> %53, ptr %58, align 4, !dbg !28, !tbaa !21
  store <4 x float> %54, ptr %59, align 4, !dbg !28, !tbaa !21
  %60 = add nuw i64 %34, 16, !dbg !19
  %61 = icmp eq i64 %60, %24, !dbg !19
  br i1 %61, label %62, label %33, !dbg !19, !llvm.loop !29

62:                                               ; preds = %33
  br i1 %25, label %90, label %63, !dbg !18

63:                                               ; preds = %62
  br i1 %27, label %78, label %64, !dbg !18

64:                                               ; preds = %63, %32
  %65 = phi i64 [ %24, %63 ], [ 0, %32 ]
  br label %66, !dbg !18

66:                                               ; preds = %66, %64
  %67 = phi i64 [ %65, %64 ], [ %75, %66 ], !dbg !19
  %68 = getelementptr inbounds nuw float, ptr %9, i64 %67, !dbg !20
  %69 = load <4 x float>, ptr %68, align 4, !dbg !20, !tbaa !21
  %70 = getelementptr inbounds nuw float, ptr %1, i64 %67, !dbg !25
  %71 = load <4 x float>, ptr %70, align 4, !dbg !25, !tbaa !21
  %72 = fadd fast <4 x float> %71, %69, !dbg !26
  %73 = or disjoint i64 %67, 1, !dbg !19
  %74 = getelementptr inbounds nuw float, ptr %0, i64 %73, !dbg !27
  store <4 x float> %72, ptr %74, align 4, !dbg !28, !tbaa !21
  %75 = add nuw i64 %67, 4, !dbg !19
  %76 = icmp eq i64 %75, %28, !dbg !19
  br i1 %76, label %77, label %66, !dbg !19, !llvm.loop !34

77:                                               ; preds = %66
  br i1 %29, label %90, label %78, !dbg !18

78:                                               ; preds = %63, %77, %30
  %79 = phi i64 [ %24, %63 ], [ 0, %30 ], [ %28, %77 ]
  br label %80, !dbg !18

80:                                               ; preds = %78, %80
  %81 = phi i64 [ %87, %80 ], [ %79, %78 ]
  %82 = getelementptr inbounds nuw float, ptr %9, i64 %81, !dbg !20
  %83 = load float, ptr %82, align 4, !dbg !20, !tbaa !21
  %84 = getelementptr inbounds nuw float, ptr %1, i64 %81, !dbg !25
  %85 = load float, ptr %84, align 4, !dbg !25, !tbaa !21
  %86 = fadd fast float %85, %83, !dbg !26
  %87 = add nuw nsw i64 %81, 1, !dbg !19
  %88 = getelementptr inbounds nuw float, ptr %0, i64 %87, !dbg !27
  store float %86, ptr %88, align 4, !dbg !28, !tbaa !21
  %89 = icmp eq i64 %87, %17, !dbg !35
  br i1 %89, label %90, label %80, !dbg !18, !llvm.loop !36

90:                                               ; preds = %80, %77, %62
  %91 = add nuw nsw i32 %31, 1, !dbg !37
  %92 = icmp eq i32 %91, %14, !dbg !15
  br i1 %92, label %93, label %30, !dbg !16, !llvm.loop !38

93:                                               ; preds = %90, %11, %4
  tail call void @free(ptr noundef %9), !dbg !40
  ret void, !dbg !41
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
!12 = !DILocation(line: 8, column: 35, scope: !9)
!13 = !DILocation(line: 8, column: 42, scope: !9)
!14 = !DILocation(line: 8, column: 28, scope: !9)
!15 = !DILocation(line: 9, column: 25, scope: !9)
!16 = !DILocation(line: 9, column: 5, scope: !9)
!17 = !DILocation(line: 10, column: 9, scope: !9)
!18 = !DILocation(line: 11, column: 9, scope: !9)
!19 = !DILocation(line: 12, column: 17, scope: !9)
!20 = !DILocation(line: 12, column: 24, scope: !9)
!21 = !{!22, !22, i64 0}
!22 = !{!"float", !23, i64 0}
!23 = !{!"omnipotent char", !24, i64 0}
!24 = !{!"Simple C++ TBAA"}
!25 = !DILocation(line: 12, column: 34, scope: !9)
!26 = !DILocation(line: 12, column: 32, scope: !9)
!27 = !DILocation(line: 12, column: 13, scope: !9)
!28 = !DILocation(line: 12, column: 22, scope: !9)
!29 = distinct !{!29, !18, !30, !31, !32, !33}
!30 = !DILocation(line: 13, column: 9, scope: !9)
!31 = !{!"llvm.loop.mustprogress"}
!32 = !{!"llvm.loop.isvectorized", i32 1}
!33 = !{!"llvm.loop.unroll.runtime.disable"}
!34 = distinct !{!34, !18, !30, !31, !32, !33}
!35 = !DILocation(line: 11, column: 27, scope: !9)
!36 = distinct !{!36, !18, !30, !31, !32}
!37 = !DILocation(line: 9, column: 45, scope: !9)
!38 = distinct !{!38, !16, !39, !31}
!39 = !DILocation(line: 14, column: 5, scope: !9)
!40 = !DILocation(line: 15, column: 5, scope: !9)
!41 = !DILocation(line: 16, column: 1, scope: !9)
