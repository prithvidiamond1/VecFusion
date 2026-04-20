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
  br i1 %16, label %21, label %17, !dbg !15

17:                                               ; preds = %11
  %18 = fsub fast float %14, %15, !dbg !16
  %19 = sitofp i32 %5 to float, !dbg !17
  %20 = fdiv fast float %18, %19, !dbg !18
  br label %23, !dbg !19

21:                                               ; preds = %11
  %22 = fdiv fast float %14, %15, !dbg !20
  br label %23

23:                                               ; preds = %21, %17
  %24 = phi float [ %20, %17 ], [ 0.000000e+00, %21 ], !dbg !21
  %25 = phi float [ 1.000000e+00, %17 ], [ %22, %21 ], !dbg !21
  store i32 %6, ptr %1, align 4, !dbg !22, !tbaa !23
  store float %8, ptr %0, align 4, !dbg !27, !tbaa !28
  %26 = icmp sgt i32 %3, 0, !dbg !30
  br i1 %26, label %27, label %165, !dbg !31

27:                                               ; preds = %23
  %28 = zext nneg i32 %3 to i64, !dbg !30
  %29 = icmp ult i32 %3, 16, !dbg !31
  br i1 %29, label %30, label %32, !dbg !31

30:                                               ; preds = %55, %32, %27
  %31 = phi i64 [ 0, %32 ], [ 0, %27 ], [ %37, %55 ]
  br label %96, !dbg !31

32:                                               ; preds = %27
  %33 = add i64 %13, 4, !dbg !31
  %34 = sub i64 %33, %12, !dbg !31
  %35 = icmp ult i64 %34, 64, !dbg !31
  br i1 %35, label %30, label %36, !dbg !31

36:                                               ; preds = %32
  %37 = and i64 %28, 2147483632, !dbg !31
  br label %38, !dbg !31

38:                                               ; preds = %38, %36
  %39 = phi i64 [ 0, %36 ], [ %53, %38 ], !dbg !32
  %40 = getelementptr inbounds i32, ptr %2, i64 %39, !dbg !33
  %41 = getelementptr inbounds i8, ptr %40, i64 16, !dbg !33
  %42 = getelementptr inbounds i8, ptr %40, i64 32, !dbg !33
  %43 = getelementptr inbounds i8, ptr %40, i64 48, !dbg !33
  %44 = load <4 x i32>, ptr %40, align 4, !dbg !33, !tbaa !23
  %45 = load <4 x i32>, ptr %41, align 4, !dbg !33, !tbaa !23
  %46 = load <4 x i32>, ptr %42, align 4, !dbg !33, !tbaa !23
  %47 = load <4 x i32>, ptr %43, align 4, !dbg !33, !tbaa !23
  %48 = or disjoint i64 %39, 1, !dbg !32
  %49 = getelementptr inbounds i32, ptr %1, i64 %48, !dbg !34
  %50 = getelementptr inbounds i8, ptr %49, i64 16, !dbg !35
  %51 = getelementptr inbounds i8, ptr %49, i64 32, !dbg !35
  %52 = getelementptr inbounds i8, ptr %49, i64 48, !dbg !35
  store <4 x i32> %44, ptr %49, align 4, !dbg !35, !tbaa !23
  store <4 x i32> %45, ptr %50, align 4, !dbg !35, !tbaa !23
  store <4 x i32> %46, ptr %51, align 4, !dbg !35, !tbaa !23
  store <4 x i32> %47, ptr %52, align 4, !dbg !35, !tbaa !23
  %53 = add nuw i64 %39, 16, !dbg !32
  %54 = icmp eq i64 %53, %37, !dbg !32
  br i1 %54, label %55, label %38, !dbg !32, !llvm.loop !36

55:                                               ; preds = %38
  %56 = icmp eq i64 %37, %28, !dbg !31
  br i1 %56, label %57, label %30, !dbg !31

57:                                               ; preds = %96, %55
  %58 = shl nuw nsw i64 %28, 2, !dbg !41
  %59 = alloca i8, i64 %58, align 8, !dbg !41
  %60 = load i32, ptr %1, align 4, !dbg !42, !tbaa !23
  br i1 %29, label %61, label %64, !dbg !43

61:                                               ; preds = %93, %57
  %62 = phi i32 [ %60, %57 ], [ %94, %93 ]
  %63 = phi i64 [ 0, %57 ], [ %65, %93 ]
  br label %143, !dbg !43

64:                                               ; preds = %57
  %65 = and i64 %28, 2147483632, !dbg !43
  %66 = insertelement <4 x i32> poison, i32 %60, i64 3, !dbg !43
  br label %67, !dbg !43

67:                                               ; preds = %67, %64
  %68 = phi i64 [ 0, %64 ], [ %91, %67 ], !dbg !44
  %69 = phi <4 x i32> [ %66, %64 ], [ %78, %67 ]
  %70 = or disjoint i64 %68, 1, !dbg !44
  %71 = getelementptr inbounds i32, ptr %1, i64 %70, !dbg !45
  %72 = getelementptr inbounds i8, ptr %71, i64 16, !dbg !45
  %73 = getelementptr inbounds i8, ptr %71, i64 32, !dbg !45
  %74 = getelementptr inbounds i8, ptr %71, i64 48, !dbg !45
  %75 = load <4 x i32>, ptr %71, align 4, !dbg !45, !tbaa !23
  %76 = load <4 x i32>, ptr %72, align 4, !dbg !45, !tbaa !23
  %77 = load <4 x i32>, ptr %73, align 4, !dbg !45, !tbaa !23
  %78 = load <4 x i32>, ptr %74, align 4, !dbg !45, !tbaa !23
  %79 = shufflevector <4 x i32> %69, <4 x i32> %75, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %80 = shufflevector <4 x i32> %75, <4 x i32> %76, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %81 = shufflevector <4 x i32> %76, <4 x i32> %77, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %82 = shufflevector <4 x i32> %77, <4 x i32> %78, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %83 = sub nsw <4 x i32> %75, %79, !dbg !46
  %84 = sub nsw <4 x i32> %76, %80, !dbg !46
  %85 = sub nsw <4 x i32> %77, %81, !dbg !46
  %86 = sub nsw <4 x i32> %78, %82, !dbg !46
  %87 = getelementptr inbounds i32, ptr %59, i64 %68, !dbg !47
  %88 = getelementptr inbounds i8, ptr %87, i64 16, !dbg !48
  %89 = getelementptr inbounds i8, ptr %87, i64 32, !dbg !48
  %90 = getelementptr inbounds i8, ptr %87, i64 48, !dbg !48
  store <4 x i32> %83, ptr %87, align 8, !dbg !48, !tbaa !23
  store <4 x i32> %84, ptr %88, align 8, !dbg !48, !tbaa !23
  store <4 x i32> %85, ptr %89, align 8, !dbg !48, !tbaa !23
  store <4 x i32> %86, ptr %90, align 8, !dbg !48, !tbaa !23
  %91 = add nuw i64 %68, 16, !dbg !44
  %92 = icmp eq i64 %91, %65, !dbg !44
  br i1 %92, label %93, label %67, !dbg !44, !llvm.loop !49

93:                                               ; preds = %67
  %94 = extractelement <4 x i32> %78, i64 3
  %95 = icmp eq i64 %65, %28, !dbg !43
  br i1 %95, label %103, label %61, !dbg !43

96:                                               ; preds = %30, %96
  %97 = phi i64 [ %100, %96 ], [ %31, %30 ]
  %98 = getelementptr inbounds i32, ptr %2, i64 %97, !dbg !33
  %99 = load i32, ptr %98, align 4, !dbg !33, !tbaa !23
  %100 = add nuw nsw i64 %97, 1, !dbg !32
  %101 = getelementptr inbounds i32, ptr %1, i64 %100, !dbg !34
  store i32 %99, ptr %101, align 4, !dbg !35, !tbaa !23
  %102 = icmp eq i64 %100, %28, !dbg !30
  br i1 %102, label %57, label %96, !dbg !31, !llvm.loop !51

103:                                              ; preds = %143, %93
  %104 = alloca i8, i64 %58, align 8, !dbg !52
  br i1 %29, label %105, label %107, !dbg !53

105:                                              ; preds = %141, %103
  %106 = phi i64 [ 0, %103 ], [ %108, %141 ]
  br label %152, !dbg !53

107:                                              ; preds = %103
  %108 = and i64 %28, 2147483632, !dbg !53
  %109 = insertelement <4 x i1> poison, i1 %10, i64 0, !dbg !53
  %110 = shufflevector <4 x i1> %109, <4 x i1> poison, <4 x i32> zeroinitializer, !dbg !53
  %111 = insertelement <4 x float> poison, float %24, i64 0, !dbg !53
  %112 = shufflevector <4 x float> %111, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !53
  %113 = insertelement <4 x float> poison, float %25, i64 0, !dbg !53
  %114 = shufflevector <4 x float> %113, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !53
  %115 = xor <4 x i1> %110, <i1 false, i1 true, i1 false, i1 true>
  %116 = select <4 x i1> %115, <4 x float> %112, <4 x float> %114
  br label %117, !dbg !53

117:                                              ; preds = %117, %107
  %118 = phi i64 [ 0, %107 ], [ %139, %117 ], !dbg !54
  %119 = getelementptr inbounds i32, ptr %59, i64 %118, !dbg !55
  %120 = getelementptr inbounds i8, ptr %119, i64 16, !dbg !55
  %121 = getelementptr inbounds i8, ptr %119, i64 32, !dbg !55
  %122 = getelementptr inbounds i8, ptr %119, i64 48, !dbg !55
  %123 = load <4 x i32>, ptr %119, align 8, !dbg !55, !tbaa !23
  %124 = load <4 x i32>, ptr %120, align 8, !dbg !55, !tbaa !23
  %125 = load <4 x i32>, ptr %121, align 8, !dbg !55, !tbaa !23
  %126 = load <4 x i32>, ptr %122, align 8, !dbg !55, !tbaa !23
  %127 = sitofp <4 x i32> %123 to <4 x float>, !dbg !55
  %128 = sitofp <4 x i32> %124 to <4 x float>, !dbg !55
  %129 = sitofp <4 x i32> %125 to <4 x float>, !dbg !55
  %130 = sitofp <4 x i32> %126 to <4 x float>, !dbg !55
  %131 = fmul fast <4 x float> %116, %127, !dbg !56
  %132 = fmul fast <4 x float> %116, %128, !dbg !56
  %133 = fmul fast <4 x float> %116, %129, !dbg !56
  %134 = fmul fast <4 x float> %116, %130, !dbg !56
  %135 = getelementptr inbounds float, ptr %104, i64 %118, !dbg !57
  %136 = getelementptr inbounds i8, ptr %135, i64 16, !dbg !58
  %137 = getelementptr inbounds i8, ptr %135, i64 32, !dbg !58
  %138 = getelementptr inbounds i8, ptr %135, i64 48, !dbg !58
  store <4 x float> %131, ptr %135, align 8, !dbg !58, !tbaa !28
  store <4 x float> %132, ptr %136, align 8, !dbg !58, !tbaa !28
  store <4 x float> %133, ptr %137, align 8, !dbg !58, !tbaa !28
  store <4 x float> %134, ptr %138, align 8, !dbg !58, !tbaa !28
  %139 = add nuw i64 %118, 16, !dbg !54
  %140 = icmp eq i64 %139, %108, !dbg !54
  br i1 %140, label %141, label %117, !dbg !54, !llvm.loop !59

141:                                              ; preds = %117
  %142 = icmp eq i64 %108, %28, !dbg !53
  br i1 %142, label %164, label %105, !dbg !53

143:                                              ; preds = %61, %143
  %144 = phi i32 [ %148, %143 ], [ %62, %61 ], !dbg !42
  %145 = phi i64 [ %146, %143 ], [ %63, %61 ]
  %146 = add nuw nsw i64 %145, 1, !dbg !44
  %147 = getelementptr inbounds i32, ptr %1, i64 %146, !dbg !45
  %148 = load i32, ptr %147, align 4, !dbg !45, !tbaa !23
  %149 = sub nsw i32 %148, %144, !dbg !46
  %150 = getelementptr inbounds i32, ptr %59, i64 %145, !dbg !47
  store i32 %149, ptr %150, align 4, !dbg !48, !tbaa !23
  %151 = icmp eq i64 %146, %28, !dbg !61
  br i1 %151, label %103, label %143, !dbg !43, !llvm.loop !62

152:                                              ; preds = %105, %152
  %153 = phi i64 [ %162, %152 ], [ %106, %105 ]
  %154 = trunc i64 %153 to i1, !dbg !63
  %155 = xor i1 %154, %10, !dbg !63
  %156 = select fast i1 %155, float %24, float %25, !dbg !64
  %157 = getelementptr inbounds i32, ptr %59, i64 %153, !dbg !55
  %158 = load i32, ptr %157, align 4, !dbg !55, !tbaa !23
  %159 = sitofp i32 %158 to float, !dbg !55
  %160 = fmul fast float %156, %159, !dbg !56
  %161 = getelementptr inbounds float, ptr %104, i64 %153, !dbg !57
  store float %160, ptr %161, align 4, !dbg !58, !tbaa !28
  %162 = add nuw nsw i64 %153, 1, !dbg !54
  %163 = icmp eq i64 %162, %28, !dbg !65
  br i1 %163, label %164, label %152, !dbg !53, !llvm.loop !66

164:                                              ; preds = %152, %141
  br label %170, !dbg !67

165:                                              ; preds = %170, %23
  %166 = add nsw i32 %3, 1, !dbg !68
  %167 = sext i32 %166 to i64, !dbg !69
  %168 = getelementptr inbounds i32, ptr %1, i64 %167, !dbg !69
  store i32 %7, ptr %168, align 4, !dbg !70, !tbaa !23
  %169 = getelementptr inbounds float, ptr %0, i64 %167, !dbg !71
  store float %9, ptr %169, align 4, !dbg !72, !tbaa !28
  ret void, !dbg !73

170:                                              ; preds = %164, %170
  %171 = phi float [ %175, %170 ], [ %8, %164 ], !dbg !74
  %172 = phi i64 [ %176, %170 ], [ 0, %164 ]
  %173 = getelementptr inbounds float, ptr %104, i64 %172, !dbg !75
  %174 = load float, ptr %173, align 4, !dbg !75, !tbaa !28
  %175 = fadd fast float %174, %171, !dbg !76
  %176 = add nuw nsw i64 %172, 1, !dbg !77
  %177 = getelementptr inbounds float, ptr %0, i64 %176, !dbg !78
  store float %175, ptr %177, align 4, !dbg !79, !tbaa !28
  %178 = icmp eq i64 %176, %28, !dbg !80
  br i1 %178, label %165, label %170, !dbg !67, !llvm.loop !81
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
!13 = !DILocation(line: 12, column: 24, scope: !9)
!14 = !DILocation(line: 12, column: 33, scope: !9)
!15 = !DILocation(line: 14, column: 9, scope: !9)
!16 = !DILocation(line: 15, column: 25, scope: !9)
!17 = !DILocation(line: 15, column: 58, scope: !9)
!18 = !DILocation(line: 15, column: 47, scope: !9)
!19 = !DILocation(line: 16, column: 5, scope: !9)
!20 = !DILocation(line: 17, column: 24, scope: !9)
!21 = !DILocation(line: 0, scope: !9)
!22 = !DILocation(line: 20, column: 12, scope: !9)
!23 = !{!24, !24, i64 0}
!24 = !{!"int", !25, i64 0}
!25 = !{!"omnipotent char", !26, i64 0}
!26 = !{!"Simple C++ TBAA"}
!27 = !DILocation(line: 21, column: 12, scope: !9)
!28 = !{!29, !29, i64 0}
!29 = !{!"float", !25, i64 0}
!30 = !DILocation(line: 24, column: 23, scope: !9)
!31 = !DILocation(line: 24, column: 5, scope: !9)
!32 = !DILocation(line: 25, column: 15, scope: !9)
!33 = !DILocation(line: 25, column: 22, scope: !9)
!34 = !DILocation(line: 25, column: 9, scope: !9)
!35 = !DILocation(line: 25, column: 20, scope: !9)
!36 = distinct !{!36, !31, !37, !38, !39, !40}
!37 = !DILocation(line: 26, column: 5, scope: !9)
!38 = !{!"llvm.loop.mustprogress"}
!39 = !{!"llvm.loop.isvectorized", i32 1}
!40 = !{!"llvm.loop.unroll.runtime.disable"}
!41 = !DILocation(line: 29, column: 27, scope: !9)
!42 = !DILocation(line: 31, column: 36, scope: !9)
!43 = !DILocation(line: 30, column: 5, scope: !9)
!44 = !DILocation(line: 31, column: 29, scope: !9)
!45 = !DILocation(line: 31, column: 23, scope: !9)
!46 = !DILocation(line: 31, column: 34, scope: !9)
!47 = !DILocation(line: 31, column: 9, scope: !9)
!48 = !DILocation(line: 31, column: 21, scope: !9)
!49 = distinct !{!49, !43, !50, !38, !39, !40}
!50 = !DILocation(line: 32, column: 5, scope: !9)
!51 = distinct !{!51, !31, !37, !38, !39}
!52 = !DILocation(line: 40, column: 31, scope: !9)
!53 = !DILocation(line: 43, column: 5, scope: !9)
!54 = !DILocation(line: 43, column: 36, scope: !9)
!55 = !DILocation(line: 46, column: 41, scope: !9)
!56 = !DILocation(line: 46, column: 39, scope: !9)
!57 = !DILocation(line: 46, column: 9, scope: !9)
!58 = !DILocation(line: 46, column: 21, scope: !9)
!59 = distinct !{!59, !53, !60, !38, !39, !40}
!60 = !DILocation(line: 47, column: 5, scope: !9)
!61 = !DILocation(line: 30, column: 23, scope: !9)
!62 = distinct !{!62, !43, !50, !38, !40, !39}
!63 = !DILocation(line: 44, column: 32, scope: !9)
!64 = !DILocation(line: 45, column: 33, scope: !9)
!65 = !DILocation(line: 43, column: 23, scope: !9)
!66 = distinct !{!66, !53, !60, !38, !40, !39}
!67 = !DILocation(line: 50, column: 5, scope: !9)
!68 = !DILocation(line: 54, column: 18, scope: !9)
!69 = !DILocation(line: 54, column: 5, scope: !9)
!70 = !DILocation(line: 54, column: 23, scope: !9)
!71 = !DILocation(line: 55, column: 5, scope: !9)
!72 = !DILocation(line: 55, column: 23, scope: !9)
!73 = !DILocation(line: 56, column: 1, scope: !9)
!74 = !DILocation(line: 51, column: 22, scope: !9)
!75 = !DILocation(line: 51, column: 31, scope: !9)
!76 = !DILocation(line: 51, column: 29, scope: !9)
!77 = !DILocation(line: 51, column: 15, scope: !9)
!78 = !DILocation(line: 51, column: 9, scope: !9)
!79 = !DILocation(line: 51, column: 20, scope: !9)
!80 = !DILocation(line: 50, column: 23, scope: !9)
!81 = distinct !{!81, !67, !82, !38}
!82 = !DILocation(line: 52, column: 5, scope: !9)
