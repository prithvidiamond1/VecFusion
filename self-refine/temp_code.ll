; ModuleID = 'temp_code.cpp'
source_filename = "temp_code.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: mustprogress nofree norecurse nosync nounwind ssp memory(argmem: readwrite) uwtable(sync)
define void @_Z14set_points_optPfPiPKiiiiiiffb(ptr nocapture noundef writeonly %0, ptr nocapture noundef %1, ptr nocapture noundef readonly %2, i32 noundef %3, i32 noundef %4, i32 noundef %5, i32 noundef %6, i32 noundef %7, float noundef nofpclass(nan inf) %8, float noundef nofpclass(nan inf) %9, i1 noundef zeroext %10) local_unnamed_addr #0 !dbg !9 {
  %12 = ptrtoint ptr %2 to i64, !dbg !12
  %13 = ptrtoint ptr %1 to i64, !dbg !12
  %14 = fsub fast float %9, %8, !dbg !12
  %15 = sitofp i32 %4 to float, !dbg !13
  %16 = fcmp fast ult float %14, %15, !dbg !14
  br i1 %16, label %21, label %17, !dbg !13

17:                                               ; preds = %11
  %18 = fsub fast float %14, %15, !dbg !15
  %19 = sitofp i32 %5 to float, !dbg !16
  %20 = fdiv fast float %18, %19, !dbg !17
  br label %23, !dbg !18

21:                                               ; preds = %11
  %22 = fdiv fast float %14, %15, !dbg !19
  br label %23

23:                                               ; preds = %21, %17
  %24 = phi float [ %20, %17 ], [ 0.000000e+00, %21 ], !dbg !20
  %25 = phi float [ 1.000000e+00, %17 ], [ %22, %21 ], !dbg !20
  store i32 %6, ptr %1, align 4, !dbg !21, !tbaa !22
  store float %8, ptr %0, align 4, !dbg !26, !tbaa !27
  %26 = icmp sgt i32 %3, 0, !dbg !29
  br i1 %26, label %31, label %27, !dbg !30

27:                                               ; preds = %23
  %28 = add nsw i32 %3, 1, !dbg !31
  %29 = sext i32 %28 to i64, !dbg !32
  %30 = getelementptr inbounds i32, ptr %1, i64 %29, !dbg !32
  store i32 %7, ptr %30, align 4, !dbg !33, !tbaa !22
  br label %140, !dbg !34

31:                                               ; preds = %23
  %32 = zext nneg i32 %3 to i64, !dbg !29
  %33 = icmp ult i32 %3, 16, !dbg !30
  br i1 %33, label %34, label %36, !dbg !30

34:                                               ; preds = %59, %36, %31
  %35 = phi i64 [ 0, %36 ], [ 0, %31 ], [ %41, %59 ]
  br label %118, !dbg !30

36:                                               ; preds = %31
  %37 = add i64 %13, 4, !dbg !30
  %38 = sub i64 %37, %12, !dbg !30
  %39 = icmp ult i64 %38, 64, !dbg !30
  br i1 %39, label %34, label %40, !dbg !30

40:                                               ; preds = %36
  %41 = and i64 %32, 2147483632, !dbg !30
  br label %42, !dbg !30

42:                                               ; preds = %42, %40
  %43 = phi i64 [ 0, %40 ], [ %57, %42 ], !dbg !35
  %44 = getelementptr inbounds i32, ptr %2, i64 %43, !dbg !36
  %45 = getelementptr inbounds i8, ptr %44, i64 16, !dbg !36
  %46 = getelementptr inbounds i8, ptr %44, i64 32, !dbg !36
  %47 = getelementptr inbounds i8, ptr %44, i64 48, !dbg !36
  %48 = load <4 x i32>, ptr %44, align 4, !dbg !36, !tbaa !22
  %49 = load <4 x i32>, ptr %45, align 4, !dbg !36, !tbaa !22
  %50 = load <4 x i32>, ptr %46, align 4, !dbg !36, !tbaa !22
  %51 = load <4 x i32>, ptr %47, align 4, !dbg !36, !tbaa !22
  %52 = or disjoint i64 %43, 1, !dbg !35
  %53 = getelementptr inbounds i32, ptr %1, i64 %52, !dbg !37
  %54 = getelementptr inbounds i8, ptr %53, i64 16, !dbg !38
  %55 = getelementptr inbounds i8, ptr %53, i64 32, !dbg !38
  %56 = getelementptr inbounds i8, ptr %53, i64 48, !dbg !38
  store <4 x i32> %48, ptr %53, align 4, !dbg !38, !tbaa !22
  store <4 x i32> %49, ptr %54, align 4, !dbg !38, !tbaa !22
  store <4 x i32> %50, ptr %55, align 4, !dbg !38, !tbaa !22
  store <4 x i32> %51, ptr %56, align 4, !dbg !38, !tbaa !22
  %57 = add nuw i64 %43, 16, !dbg !35
  %58 = icmp eq i64 %57, %41, !dbg !35
  br i1 %58, label %59, label %42, !dbg !35, !llvm.loop !39

59:                                               ; preds = %42
  %60 = icmp eq i64 %41, %32, !dbg !30
  br i1 %60, label %61, label %34, !dbg !30

61:                                               ; preds = %118, %59
  %62 = add nuw nsw i32 %3, 1, !dbg !31
  %63 = zext nneg i32 %62 to i64, !dbg !32
  %64 = getelementptr inbounds i32, ptr %1, i64 %63, !dbg !32
  store i32 %7, ptr %64, align 4, !dbg !33, !tbaa !22
  %65 = alloca float, i64 %63, align 4, !dbg !44
  %66 = load i32, ptr %1, align 4, !dbg !45, !tbaa !22
  br i1 %33, label %67, label %70, !dbg !34

67:                                               ; preds = %115, %61
  %68 = phi i32 [ %66, %61 ], [ %116, %115 ]
  %69 = phi i64 [ 0, %61 ], [ %71, %115 ]
  br label %125, !dbg !34

70:                                               ; preds = %61
  %71 = and i64 %32, 2147483632, !dbg !34
  %72 = insertelement <4 x i32> poison, i32 %66, i64 3, !dbg !34
  %73 = insertelement <4 x i1> poison, i1 %10, i64 0, !dbg !34
  %74 = shufflevector <4 x i1> %73, <4 x i1> poison, <4 x i32> zeroinitializer, !dbg !34
  %75 = xor <4 x i1> %74, <i1 false, i1 true, i1 false, i1 true>, !dbg !34
  %76 = insertelement <4 x float> poison, float %24, i64 0, !dbg !34
  %77 = shufflevector <4 x float> %76, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !34
  %78 = insertelement <4 x float> poison, float %25, i64 0, !dbg !34
  %79 = shufflevector <4 x float> %78, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !34
  %80 = select <4 x i1> %75, <4 x float> %77, <4 x float> %79
  br label %81, !dbg !34

81:                                               ; preds = %81, %70
  %82 = phi i64 [ 0, %70 ], [ %113, %81 ], !dbg !46
  %83 = phi <4 x i32> [ %72, %70 ], [ %92, %81 ]
  %84 = or disjoint i64 %82, 1, !dbg !46
  %85 = getelementptr inbounds i32, ptr %1, i64 %84, !dbg !47
  %86 = getelementptr inbounds i8, ptr %85, i64 16, !dbg !47
  %87 = getelementptr inbounds i8, ptr %85, i64 32, !dbg !47
  %88 = getelementptr inbounds i8, ptr %85, i64 48, !dbg !47
  %89 = load <4 x i32>, ptr %85, align 4, !dbg !47, !tbaa !22
  %90 = load <4 x i32>, ptr %86, align 4, !dbg !47, !tbaa !22
  %91 = load <4 x i32>, ptr %87, align 4, !dbg !47, !tbaa !22
  %92 = load <4 x i32>, ptr %88, align 4, !dbg !47, !tbaa !22
  %93 = shufflevector <4 x i32> %83, <4 x i32> %89, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %94 = shufflevector <4 x i32> %89, <4 x i32> %90, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %95 = shufflevector <4 x i32> %90, <4 x i32> %91, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %96 = shufflevector <4 x i32> %91, <4 x i32> %92, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %97 = sub nsw <4 x i32> %89, %93, !dbg !48
  %98 = sub nsw <4 x i32> %90, %94, !dbg !48
  %99 = sub nsw <4 x i32> %91, %95, !dbg !48
  %100 = sub nsw <4 x i32> %92, %96, !dbg !48
  %101 = sitofp <4 x i32> %97 to <4 x float>, !dbg !49
  %102 = sitofp <4 x i32> %98 to <4 x float>, !dbg !49
  %103 = sitofp <4 x i32> %99 to <4 x float>, !dbg !49
  %104 = sitofp <4 x i32> %100 to <4 x float>, !dbg !49
  %105 = fmul fast <4 x float> %80, %101, !dbg !50
  %106 = fmul fast <4 x float> %80, %102, !dbg !50
  %107 = fmul fast <4 x float> %80, %103, !dbg !50
  %108 = fmul fast <4 x float> %80, %104, !dbg !50
  %109 = getelementptr inbounds float, ptr %65, i64 %82, !dbg !51
  %110 = getelementptr inbounds i8, ptr %109, i64 16, !dbg !52
  %111 = getelementptr inbounds i8, ptr %109, i64 32, !dbg !52
  %112 = getelementptr inbounds i8, ptr %109, i64 48, !dbg !52
  store <4 x float> %105, ptr %109, align 4, !dbg !52, !tbaa !27
  store <4 x float> %106, ptr %110, align 4, !dbg !52, !tbaa !27
  store <4 x float> %107, ptr %111, align 4, !dbg !52, !tbaa !27
  store <4 x float> %108, ptr %112, align 4, !dbg !52, !tbaa !27
  %113 = add nuw i64 %82, 16, !dbg !46
  %114 = icmp eq i64 %113, %71, !dbg !46
  br i1 %114, label %115, label %81, !dbg !46, !llvm.loop !53

115:                                              ; preds = %81
  %116 = extractelement <4 x i32> %92, i64 3
  %117 = icmp eq i64 %71, %32, !dbg !34
  br i1 %117, label %139, label %67, !dbg !34

118:                                              ; preds = %34, %118
  %119 = phi i64 [ %122, %118 ], [ %35, %34 ]
  %120 = getelementptr inbounds i32, ptr %2, i64 %119, !dbg !36
  %121 = load i32, ptr %120, align 4, !dbg !36, !tbaa !22
  %122 = add nuw nsw i64 %119, 1, !dbg !35
  %123 = getelementptr inbounds i32, ptr %1, i64 %122, !dbg !37
  store i32 %121, ptr %123, align 4, !dbg !38, !tbaa !22
  %124 = icmp eq i64 %122, %32, !dbg !29
  br i1 %124, label %61, label %118, !dbg !30, !llvm.loop !55

125:                                              ; preds = %67, %125
  %126 = phi i32 [ %131, %125 ], [ %68, %67 ], !dbg !45
  %127 = phi i64 [ %129, %125 ], [ %69, %67 ]
  %128 = phi i1 [ %137, %125 ], [ %10, %67 ]
  %129 = add nuw nsw i64 %127, 1, !dbg !46
  %130 = getelementptr inbounds i32, ptr %1, i64 %129, !dbg !47
  %131 = load i32, ptr %130, align 4, !dbg !47, !tbaa !22
  %132 = sub nsw i32 %131, %126, !dbg !48
  %133 = sitofp i32 %132 to float, !dbg !49
  %134 = select fast i1 %128, float %24, float %25, !dbg !50
  %135 = fmul fast float %134, %133, !dbg !50
  %136 = getelementptr inbounds float, ptr %65, i64 %127, !dbg !51
  store float %135, ptr %136, align 4, !dbg !52, !tbaa !27
  %137 = xor i1 %128, true, !dbg !56
  %138 = icmp eq i64 %129, %32, !dbg !57
  br i1 %138, label %139, label %125, !dbg !34, !llvm.loop !58

139:                                              ; preds = %125, %115
  br label %143, !dbg !59

140:                                              ; preds = %143, %27
  %141 = phi i64 [ %29, %27 ], [ %63, %143 ]
  %142 = getelementptr inbounds float, ptr %0, i64 %141, !dbg !60
  store float %9, ptr %142, align 4, !dbg !61, !tbaa !27
  ret void, !dbg !62

143:                                              ; preds = %139, %143
  %144 = phi i64 [ %149, %143 ], [ 0, %139 ]
  %145 = phi float [ %148, %143 ], [ %8, %139 ]
  %146 = getelementptr inbounds float, ptr %65, i64 %144, !dbg !63
  %147 = load float, ptr %146, align 4, !dbg !63, !tbaa !27
  %148 = fadd fast float %147, %145, !dbg !64
  %149 = add nuw nsw i64 %144, 1, !dbg !65
  %150 = getelementptr inbounds float, ptr %0, i64 %149, !dbg !66
  store float %148, ptr %150, align 4, !dbg !67, !tbaa !27
  %151 = icmp eq i64 %149, %32, !dbg !68
  br i1 %151, label %140, label %143, !dbg !59, !llvm.loop !69
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind ssp memory(argmem: readwrite) uwtable(sync) "approx-func-fp-math"="true" "frame-pointer"="non-leaf" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "probe-stack"="__chkstk_darwin" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+bti,+ccdp,+ccidx,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8.5a,+v8a,+zcm,+zcz" "unsafe-fp-math"="true" }

!llvm.module.flags = !{!0, !1, !2, !3, !4, !5}
!llvm.dbg.cu = !{!6}
!llvm.ident = !{!8}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 26, i32 2]}
!1 = !{i32 2, !"Debug Info Version", i32 3}
!2 = !{i32 1, !"wchar_size", i32 4}
!3 = !{i32 8, !"PIC Level", i32 2}
!4 = !{i32 7, !"uwtable", i32 1}
!5 = !{i32 7, !"frame-pointer", i32 1}
!6 = distinct !DICompileUnit(language: DW_LANG_C_plus_plus_14, file: !7, producer: "Apple clang version 17.0.0 (clang-1700.0.13.5)", isOptimized: true, runtimeVersion: 0, emissionKind: NoDebug, splitDebugInlining: false, nameTableKind: Apple, sysroot: "/Library/Developer/CommandLineTools/SDKs/MacOSX26.2.sdk", sdk: "MacOSX26.2.sdk")
!7 = !DIFile(filename: "temp_code.cpp", directory: "/Users/torence/VecTrans/self-refine")
!8 = !{!"Apple clang version 17.0.0 (clang-1700.0.13.5)"}
!9 = distinct !DISubprogram(name: "set_points_opt", scope: !7, file: !7, line: 7, type: !10, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !6)
!10 = !DISubroutineType(types: !11)
!11 = !{}
!12 = !DILocation(line: 10, column: 27, scope: !9)
!13 = !DILocation(line: 12, column: 9, scope: !9)
!14 = !DILocation(line: 12, column: 18, scope: !9)
!15 = !DILocation(line: 13, column: 25, scope: !9)
!16 = !DILocation(line: 13, column: 58, scope: !9)
!17 = !DILocation(line: 13, column: 47, scope: !9)
!18 = !DILocation(line: 14, column: 5, scope: !9)
!19 = !DILocation(line: 15, column: 24, scope: !9)
!20 = !DILocation(line: 0, scope: !9)
!21 = !DILocation(line: 18, column: 12, scope: !9)
!22 = !{!23, !23, i64 0}
!23 = !{!"int", !24, i64 0}
!24 = !{!"omnipotent char", !25, i64 0}
!25 = !{!"Simple C++ TBAA"}
!26 = !DILocation(line: 19, column: 12, scope: !9)
!27 = !{!28, !28, i64 0}
!28 = !{!"float", !24, i64 0}
!29 = !DILocation(line: 22, column: 23, scope: !9)
!30 = !DILocation(line: 22, column: 5, scope: !9)
!31 = !DILocation(line: 25, column: 18, scope: !9)
!32 = !DILocation(line: 25, column: 5, scope: !9)
!33 = !DILocation(line: 25, column: 23, scope: !9)
!34 = !DILocation(line: 39, column: 5, scope: !9)
!35 = !DILocation(line: 23, column: 15, scope: !9)
!36 = !DILocation(line: 23, column: 22, scope: !9)
!37 = !DILocation(line: 23, column: 9, scope: !9)
!38 = !DILocation(line: 23, column: 20, scope: !9)
!39 = distinct !{!39, !30, !40, !41, !42, !43}
!40 = !DILocation(line: 24, column: 5, scope: !9)
!41 = !{!"llvm.loop.mustprogress"}
!42 = !{!"llvm.loop.isvectorized", i32 1}
!43 = !{!"llvm.loop.unroll.runtime.disable"}
!44 = !DILocation(line: 38, column: 5, scope: !9)
!45 = !DILocation(line: 40, column: 37, scope: !9)
!46 = !DILocation(line: 40, column: 30, scope: !9)
!47 = !DILocation(line: 40, column: 24, scope: !9)
!48 = !DILocation(line: 40, column: 35, scope: !9)
!49 = !DILocation(line: 42, column: 55, scope: !9)
!50 = !DILocation(line: 42, column: 64, scope: !9)
!51 = !DILocation(line: 42, column: 9, scope: !9)
!52 = !DILocation(line: 42, column: 21, scope: !9)
!53 = distinct !{!53, !34, !54, !41, !42, !43}
!54 = !DILocation(line: 45, column: 5, scope: !9)
!55 = distinct !{!55, !30, !40, !41, !42}
!56 = !DILocation(line: 44, column: 22, scope: !9)
!57 = !DILocation(line: 39, column: 23, scope: !9)
!58 = distinct !{!58, !34, !54, !41, !43, !42}
!59 = !DILocation(line: 49, column: 5, scope: !9)
!60 = !DILocation(line: 54, column: 5, scope: !9)
!61 = !DILocation(line: 54, column: 23, scope: !9)
!62 = !DILocation(line: 55, column: 1, scope: !9)
!63 = !DILocation(line: 50, column: 23, scope: !9)
!64 = !DILocation(line: 50, column: 20, scope: !9)
!65 = !DILocation(line: 51, column: 15, scope: !9)
!66 = !DILocation(line: 51, column: 9, scope: !9)
!67 = !DILocation(line: 51, column: 20, scope: !9)
!68 = !DILocation(line: 49, column: 23, scope: !9)
!69 = distinct !{!69, !59, !70, !41}
!70 = !DILocation(line: 52, column: 5, scope: !9)
