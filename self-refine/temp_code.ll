; ModuleID = 'temp_code.cpp'
source_filename = "temp_code.cpp"
target datalayout = "e-m:o-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: mustprogress nofree norecurse nosync nounwind ssp memory(argmem: readwrite) uwtable(sync)
define void @_Z14set_points_optPfPiPKiiiiiiffb(ptr nocapture noundef writeonly %0, ptr nocapture noundef writeonly %1, ptr nocapture noundef readonly %2, i32 noundef %3, i32 noundef %4, i32 noundef %5, i32 noundef %6, i32 noundef %7, float noundef nofpclass(nan inf) %8, float noundef nofpclass(nan inf) %9, i1 noundef zeroext %10) local_unnamed_addr #0 !dbg !9 {
  %12 = ptrtoint ptr %2 to i64, !dbg !12
  %13 = ptrtoint ptr %1 to i64, !dbg !12
  %14 = fsub fast float %9, %8, !dbg !12
  %15 = sitofp i32 %4 to float, !dbg !13
  %16 = fcmp fast oge float %14, %15, !dbg !14
  br i1 %16, label %17, label %24, !dbg !15

17:                                               ; preds = %11
  %18 = fsub fast float %14, %15, !dbg !16
  %19 = sitofp i32 %5 to float, !dbg !17
  %20 = fdiv fast float %18, %19, !dbg !18
  store i32 %6, ptr %1, align 4, !dbg !19, !tbaa !20
  store float %8, ptr %0, align 4, !dbg !24, !tbaa !25
  %21 = zext i32 %3 to i64, !dbg !27
  %22 = alloca i32, i64 %21, align 4, !dbg !27
  %23 = icmp sgt i32 %3, 0, !dbg !28
  br i1 %23, label %29, label %153, !dbg !29

24:                                               ; preds = %11
  %25 = fdiv fast float %14, %15, !dbg !30
  store i32 %6, ptr %1, align 4, !dbg !19, !tbaa !20
  store float %8, ptr %0, align 4, !dbg !24, !tbaa !25
  %26 = zext i32 %3 to i64, !dbg !27
  %27 = alloca i32, i64 %26, align 4, !dbg !27
  %28 = icmp sgt i32 %3, 0, !dbg !28
  br i1 %28, label %29, label %153, !dbg !29

29:                                               ; preds = %24, %17
  %30 = phi ptr [ %27, %24 ], [ %22, %17 ]
  %31 = phi i64 [ %26, %24 ], [ %21, %17 ]
  %32 = phi float [ %25, %24 ], [ %20, %17 ]
  %33 = icmp ult i64 %31, 16, !dbg !29
  br i1 %33, label %34, label %37, !dbg !29

34:                                               ; preds = %74, %37, %29
  %35 = phi i32 [ %6, %37 ], [ %6, %29 ], [ %75, %74 ]
  %36 = phi i64 [ 0, %37 ], [ 0, %29 ], [ %42, %74 ]
  br label %116, !dbg !29

37:                                               ; preds = %29
  %38 = add i64 %13, 4, !dbg !29
  %39 = sub i64 %38, %12, !dbg !29
  %40 = icmp ult i64 %39, 64, !dbg !29
  br i1 %40, label %34, label %41, !dbg !29

41:                                               ; preds = %37
  %42 = and i64 %31, 4294967280, !dbg !29
  %43 = insertelement <4 x i32> poison, i32 %6, i64 3, !dbg !29
  br label %44, !dbg !29

44:                                               ; preds = %44, %41
  %45 = phi i64 [ 0, %41 ], [ %72, %44 ], !dbg !31
  %46 = phi <4 x i32> [ %43, %41 ], [ %54, %44 ]
  %47 = getelementptr inbounds i32, ptr %2, i64 %45, !dbg !32
  %48 = getelementptr inbounds i8, ptr %47, i64 16, !dbg !32
  %49 = getelementptr inbounds i8, ptr %47, i64 32, !dbg !32
  %50 = getelementptr inbounds i8, ptr %47, i64 48, !dbg !32
  %51 = load <4 x i32>, ptr %47, align 4, !dbg !32, !tbaa !20
  %52 = load <4 x i32>, ptr %48, align 4, !dbg !32, !tbaa !20
  %53 = load <4 x i32>, ptr %49, align 4, !dbg !32, !tbaa !20
  %54 = load <4 x i32>, ptr %50, align 4, !dbg !32, !tbaa !20
  %55 = shufflevector <4 x i32> %46, <4 x i32> %51, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %56 = shufflevector <4 x i32> %51, <4 x i32> %52, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %57 = shufflevector <4 x i32> %52, <4 x i32> %53, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %58 = shufflevector <4 x i32> %53, <4 x i32> %54, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %59 = or disjoint i64 %45, 1, !dbg !31
  %60 = getelementptr inbounds i32, ptr %1, i64 %59, !dbg !33
  %61 = getelementptr inbounds i8, ptr %60, i64 16, !dbg !34
  %62 = getelementptr inbounds i8, ptr %60, i64 32, !dbg !34
  %63 = getelementptr inbounds i8, ptr %60, i64 48, !dbg !34
  store <4 x i32> %51, ptr %60, align 4, !dbg !34, !tbaa !20
  store <4 x i32> %52, ptr %61, align 4, !dbg !34, !tbaa !20
  store <4 x i32> %53, ptr %62, align 4, !dbg !34, !tbaa !20
  store <4 x i32> %54, ptr %63, align 4, !dbg !34, !tbaa !20
  %64 = sub nsw <4 x i32> %51, %55, !dbg !35
  %65 = sub nsw <4 x i32> %52, %56, !dbg !35
  %66 = sub nsw <4 x i32> %53, %57, !dbg !35
  %67 = sub nsw <4 x i32> %54, %58, !dbg !35
  %68 = getelementptr inbounds i32, ptr %30, i64 %45, !dbg !36
  %69 = getelementptr inbounds i8, ptr %68, i64 16, !dbg !37
  %70 = getelementptr inbounds i8, ptr %68, i64 32, !dbg !37
  %71 = getelementptr inbounds i8, ptr %68, i64 48, !dbg !37
  store <4 x i32> %64, ptr %68, align 4, !dbg !37, !tbaa !20
  store <4 x i32> %65, ptr %69, align 4, !dbg !37, !tbaa !20
  store <4 x i32> %66, ptr %70, align 4, !dbg !37, !tbaa !20
  store <4 x i32> %67, ptr %71, align 4, !dbg !37, !tbaa !20
  %72 = add nuw i64 %45, 16, !dbg !31
  %73 = icmp eq i64 %72, %42, !dbg !31
  br i1 %73, label %74, label %44, !dbg !31, !llvm.loop !38

74:                                               ; preds = %44
  %75 = extractelement <4 x i32> %54, i64 3
  %76 = icmp eq i64 %31, %42, !dbg !29
  br i1 %76, label %77, label %34, !dbg !29

77:                                               ; preds = %116, %74
  %78 = alloca float, i64 %31, align 4, !dbg !43
  br i1 %16, label %79, label %139, !dbg !44

79:                                               ; preds = %77
  br i1 %33, label %80, label %82, !dbg !45

80:                                               ; preds = %114, %79
  %81 = phi i64 [ 0, %79 ], [ %83, %114 ]
  br label %126, !dbg !45

82:                                               ; preds = %79
  %83 = and i64 %31, 4294967280, !dbg !45
  %84 = insertelement <4 x i1> poison, i1 %10, i64 0, !dbg !45
  %85 = shufflevector <4 x i1> %84, <4 x i1> poison, <4 x i32> zeroinitializer, !dbg !45
  %86 = insertelement <4 x float> poison, float %32, i64 0, !dbg !45
  %87 = shufflevector <4 x float> %86, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !45
  %88 = xor <4 x i1> %85, <i1 false, i1 true, i1 false, i1 true>
  %89 = select <4 x i1> %88, <4 x float> %87, <4 x float> <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>
  br label %90, !dbg !45

90:                                               ; preds = %90, %82
  %91 = phi i64 [ 0, %82 ], [ %112, %90 ], !dbg !46
  %92 = getelementptr inbounds i32, ptr %30, i64 %91, !dbg !47
  %93 = getelementptr inbounds i8, ptr %92, i64 16, !dbg !47
  %94 = getelementptr inbounds i8, ptr %92, i64 32, !dbg !47
  %95 = getelementptr inbounds i8, ptr %92, i64 48, !dbg !47
  %96 = load <4 x i32>, ptr %92, align 4, !dbg !47, !tbaa !20
  %97 = load <4 x i32>, ptr %93, align 4, !dbg !47, !tbaa !20
  %98 = load <4 x i32>, ptr %94, align 4, !dbg !47, !tbaa !20
  %99 = load <4 x i32>, ptr %95, align 4, !dbg !47, !tbaa !20
  %100 = sitofp <4 x i32> %96 to <4 x float>, !dbg !47
  %101 = sitofp <4 x i32> %97 to <4 x float>, !dbg !47
  %102 = sitofp <4 x i32> %98 to <4 x float>, !dbg !47
  %103 = sitofp <4 x i32> %99 to <4 x float>, !dbg !47
  %104 = fmul fast <4 x float> %89, %100, !dbg !48
  %105 = fmul fast <4 x float> %89, %101, !dbg !48
  %106 = fmul fast <4 x float> %89, %102, !dbg !48
  %107 = fmul fast <4 x float> %89, %103, !dbg !48
  %108 = getelementptr inbounds float, ptr %78, i64 %91, !dbg !49
  %109 = getelementptr inbounds i8, ptr %108, i64 16, !dbg !50
  %110 = getelementptr inbounds i8, ptr %108, i64 32, !dbg !50
  %111 = getelementptr inbounds i8, ptr %108, i64 48, !dbg !50
  store <4 x float> %104, ptr %108, align 4, !dbg !50, !tbaa !25
  store <4 x float> %105, ptr %109, align 4, !dbg !50, !tbaa !25
  store <4 x float> %106, ptr %110, align 4, !dbg !50, !tbaa !25
  store <4 x float> %107, ptr %111, align 4, !dbg !50, !tbaa !25
  %112 = add nuw i64 %91, 16, !dbg !46
  %113 = icmp eq i64 %112, %83, !dbg !46
  br i1 %113, label %114, label %90, !dbg !46, !llvm.loop !51

114:                                              ; preds = %90
  %115 = icmp eq i64 %31, %83, !dbg !45
  br i1 %115, label %138, label %80, !dbg !45

116:                                              ; preds = %34, %116
  %117 = phi i32 [ %120, %116 ], [ %35, %34 ], !dbg !53
  %118 = phi i64 [ %121, %116 ], [ %36, %34 ]
  %119 = getelementptr inbounds i32, ptr %2, i64 %118, !dbg !32
  %120 = load i32, ptr %119, align 4, !dbg !32, !tbaa !20
  %121 = add nuw nsw i64 %118, 1, !dbg !31
  %122 = getelementptr inbounds i32, ptr %1, i64 %121, !dbg !33
  store i32 %120, ptr %122, align 4, !dbg !34, !tbaa !20
  %123 = sub nsw i32 %120, %117, !dbg !35
  %124 = getelementptr inbounds i32, ptr %30, i64 %118, !dbg !36
  store i32 %123, ptr %124, align 4, !dbg !37, !tbaa !20
  %125 = icmp eq i64 %121, %31, !dbg !28
  br i1 %125, label %77, label %116, !dbg !29, !llvm.loop !54

126:                                              ; preds = %80, %126
  %127 = phi i64 [ %136, %126 ], [ %81, %80 ]
  %128 = trunc i64 %127 to i1, !dbg !55
  %129 = xor i1 %128, %10, !dbg !55
  %130 = getelementptr inbounds i32, ptr %30, i64 %127, !dbg !47
  %131 = load i32, ptr %130, align 4, !dbg !47, !tbaa !20
  %132 = sitofp i32 %131 to float, !dbg !47
  %133 = select fast i1 %129, float %32, float 1.000000e+00, !dbg !48
  %134 = fmul fast float %133, %132, !dbg !48
  %135 = getelementptr inbounds float, ptr %78, i64 %127, !dbg !49
  store float %134, ptr %135, align 4, !dbg !50, !tbaa !25
  %136 = add nuw nsw i64 %127, 1, !dbg !46
  %137 = icmp eq i64 %136, %31, !dbg !56
  br i1 %137, label %138, label %126, !dbg !45, !llvm.loop !57

138:                                              ; preds = %148, %126, %114
  br label %158, !dbg !58

139:                                              ; preds = %77, %148
  %140 = phi i64 [ %151, %148 ], [ 0, %77 ]
  %141 = trunc i64 %140 to i1, !dbg !59
  %142 = xor i1 %141, %10, !dbg !59
  br i1 %142, label %148, label %143, !dbg !60

143:                                              ; preds = %139
  %144 = getelementptr inbounds i32, ptr %30, i64 %140, !dbg !61
  %145 = load i32, ptr %144, align 4, !dbg !61, !tbaa !20
  %146 = sitofp i32 %145 to float, !dbg !61
  %147 = fmul fast float %32, %146, !dbg !62
  br label %148, !dbg !60

148:                                              ; preds = %139, %143
  %149 = phi fast float [ %147, %143 ], [ 0.000000e+00, %139 ], !dbg !60
  %150 = getelementptr inbounds float, ptr %78, i64 %140, !dbg !63
  store float %149, ptr %150, align 4, !dbg !64, !tbaa !25
  %151 = add nuw nsw i64 %140, 1, !dbg !65
  %152 = icmp eq i64 %151, %31, !dbg !66
  br i1 %152, label %138, label %139, !dbg !67, !llvm.loop !68

153:                                              ; preds = %158, %17, %24
  %154 = add nsw i32 %3, 1, !dbg !70
  %155 = sext i32 %154 to i64, !dbg !71
  %156 = getelementptr inbounds i32, ptr %1, i64 %155, !dbg !71
  store i32 %7, ptr %156, align 4, !dbg !72, !tbaa !20
  %157 = getelementptr inbounds float, ptr %0, i64 %155, !dbg !73
  store float %9, ptr %157, align 4, !dbg !74, !tbaa !25
  ret void, !dbg !75

158:                                              ; preds = %138, %158
  %159 = phi i64 [ %164, %158 ], [ 0, %138 ]
  %160 = phi float [ %163, %158 ], [ %8, %138 ]
  %161 = getelementptr inbounds float, ptr %78, i64 %159, !dbg !76
  %162 = load float, ptr %161, align 4, !dbg !76, !tbaa !25
  %163 = fadd fast float %162, %160, !dbg !77
  %164 = add nuw nsw i64 %159, 1, !dbg !78
  %165 = getelementptr inbounds float, ptr %0, i64 %164, !dbg !79
  store float %163, ptr %165, align 4, !dbg !80, !tbaa !25
  %166 = icmp eq i64 %164, %31, !dbg !81
  br i1 %166, label %153, label %158, !dbg !58, !llvm.loop !82
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
!19 = !DILocation(line: 20, column: 12, scope: !9)
!20 = !{!21, !21, i64 0}
!21 = !{!"int", !22, i64 0}
!22 = !{!"omnipotent char", !23, i64 0}
!23 = !{!"Simple C++ TBAA"}
!24 = !DILocation(line: 21, column: 12, scope: !9)
!25 = !{!26, !26, i64 0}
!26 = !{!"float", !22, i64 0}
!27 = !DILocation(line: 24, column: 5, scope: !9)
!28 = !DILocation(line: 25, column: 23, scope: !9)
!29 = !DILocation(line: 25, column: 5, scope: !9)
!30 = !DILocation(line: 17, column: 24, scope: !9)
!31 = !DILocation(line: 26, column: 15, scope: !9)
!32 = !DILocation(line: 26, column: 22, scope: !9)
!33 = !DILocation(line: 26, column: 9, scope: !9)
!34 = !DILocation(line: 26, column: 20, scope: !9)
!35 = !DILocation(line: 27, column: 34, scope: !9)
!36 = !DILocation(line: 27, column: 9, scope: !9)
!37 = !DILocation(line: 27, column: 21, scope: !9)
!38 = distinct !{!38, !29, !39, !40, !41, !42}
!39 = !DILocation(line: 28, column: 5, scope: !9)
!40 = !{!"llvm.loop.mustprogress"}
!41 = !{!"llvm.loop.isvectorized", i32 1}
!42 = !{!"llvm.loop.unroll.runtime.disable"}
!43 = !DILocation(line: 31, column: 5, scope: !9)
!44 = !DILocation(line: 32, column: 9, scope: !9)
!45 = !DILocation(line: 34, column: 9, scope: !9)
!46 = !DILocation(line: 34, column: 40, scope: !9)
!47 = !DILocation(line: 36, scope: !9)
!48 = !DILocation(line: 36, column: 27, scope: !9)
!49 = !DILocation(line: 36, column: 13, scope: !9)
!50 = !DILocation(line: 36, column: 25, scope: !9)
!51 = distinct !{!51, !45, !52, !40, !41, !42}
!52 = !DILocation(line: 37, column: 9, scope: !9)
!53 = !DILocation(line: 27, column: 36, scope: !9)
!54 = distinct !{!54, !29, !39, !40, !41}
!55 = !DILocation(line: 35, column: 33, scope: !9)
!56 = !DILocation(line: 34, column: 27, scope: !9)
!57 = distinct !{!57, !45, !52, !40, !42, !41}
!58 = !DILocation(line: 48, column: 5, scope: !9)
!59 = !DILocation(line: 41, column: 33, scope: !9)
!60 = !DILocation(line: 42, column: 27, scope: !9)
!61 = !DILocation(line: 42, column: 58, scope: !9)
!62 = !DILocation(line: 42, column: 56, scope: !9)
!63 = !DILocation(line: 42, column: 13, scope: !9)
!64 = !DILocation(line: 42, column: 25, scope: !9)
!65 = !DILocation(line: 40, column: 40, scope: !9)
!66 = !DILocation(line: 40, column: 27, scope: !9)
!67 = !DILocation(line: 40, column: 9, scope: !9)
!68 = distinct !{!68, !67, !69, !40}
!69 = !DILocation(line: 43, column: 9, scope: !9)
!70 = !DILocation(line: 53, column: 18, scope: !9)
!71 = !DILocation(line: 53, column: 5, scope: !9)
!72 = !DILocation(line: 53, column: 23, scope: !9)
!73 = !DILocation(line: 54, column: 5, scope: !9)
!74 = !DILocation(line: 54, column: 23, scope: !9)
!75 = !DILocation(line: 55, column: 1, scope: !9)
!76 = !DILocation(line: 49, column: 24, scope: !9)
!77 = !DILocation(line: 49, column: 21, scope: !9)
!78 = !DILocation(line: 50, column: 15, scope: !9)
!79 = !DILocation(line: 50, column: 9, scope: !9)
!80 = !DILocation(line: 50, column: 20, scope: !9)
!81 = !DILocation(line: 48, column: 23, scope: !9)
!82 = distinct !{!82, !58, !83, !40}
!83 = !DILocation(line: 51, column: 5, scope: !9)
