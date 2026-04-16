; ModuleID = 'temp_code.cpp'
source_filename = "temp_code.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: mustprogress nofree norecurse nosync nounwind ssp memory(argmem: readwrite) uwtable(sync)
define void @_Z14set_points_optPfPiPKiiiiiiffb(ptr nocapture noundef writeonly %0, ptr nocapture noundef initializes((0, 4)) %1, ptr nocapture noundef readonly %2, i32 noundef %3, i32 noundef %4, i32 noundef %5, i32 noundef %6, i32 noundef %7, float noundef nofpclass(nan inf) %8, float noundef nofpclass(nan inf) %9, i1 noundef zeroext %10) local_unnamed_addr #0 !dbg !9 {
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
  %24 = phi float [ 1.000000e+00, %17 ], [ %22, %21 ], !dbg !21
  %25 = phi float [ %20, %17 ], [ 0.000000e+00, %21 ], !dbg !21
  store i32 %6, ptr %1, align 4, !dbg !22, !tbaa !23
  %26 = icmp sgt i32 %3, 0, !dbg !27
  br i1 %26, label %31, label %27, !dbg !28

27:                                               ; preds = %23
  %28 = add nsw i32 %3, 1, !dbg !29
  %29 = sext i32 %28 to i64, !dbg !30
  %30 = getelementptr inbounds i32, ptr %1, i64 %29, !dbg !30
  store i32 %7, ptr %30, align 4, !dbg !31, !tbaa !23
  store float %8, ptr %0, align 4, !dbg !32, !tbaa !33
  br label %225, !dbg !35

31:                                               ; preds = %23
  %32 = zext nneg i32 %3 to i64, !dbg !27
  %33 = icmp ult i32 %3, 4, !dbg !28
  br i1 %33, label %34, label %36, !dbg !28

34:                                               ; preds = %63, %77, %36, %31
  %35 = phi i64 [ %43, %63 ], [ 0, %31 ], [ 0, %36 ], [ %68, %77 ]
  br label %123, !dbg !28

36:                                               ; preds = %31
  %37 = add i64 %13, 4, !dbg !28
  %38 = sub i64 %37, %12, !dbg !28
  %39 = icmp ult i64 %38, 64, !dbg !28
  br i1 %39, label %34, label %40, !dbg !28

40:                                               ; preds = %36
  %41 = icmp ult i32 %3, 16, !dbg !28
  br i1 %41, label %66, label %42, !dbg !28

42:                                               ; preds = %40
  %43 = and i64 %32, 2147483632, !dbg !28
  br label %44, !dbg !28

44:                                               ; preds = %44, %42
  %45 = phi i64 [ 0, %42 ], [ %59, %44 ], !dbg !36
  %46 = getelementptr inbounds nuw i32, ptr %2, i64 %45, !dbg !37
  %47 = getelementptr inbounds nuw i8, ptr %46, i64 16, !dbg !37
  %48 = getelementptr inbounds nuw i8, ptr %46, i64 32, !dbg !37
  %49 = getelementptr inbounds nuw i8, ptr %46, i64 48, !dbg !37
  %50 = load <4 x i32>, ptr %46, align 4, !dbg !37, !tbaa !23
  %51 = load <4 x i32>, ptr %47, align 4, !dbg !37, !tbaa !23
  %52 = load <4 x i32>, ptr %48, align 4, !dbg !37, !tbaa !23
  %53 = load <4 x i32>, ptr %49, align 4, !dbg !37, !tbaa !23
  %54 = or disjoint i64 %45, 1, !dbg !36
  %55 = getelementptr inbounds nuw i32, ptr %1, i64 %54, !dbg !38
  %56 = getelementptr inbounds nuw i8, ptr %55, i64 16, !dbg !39
  %57 = getelementptr inbounds nuw i8, ptr %55, i64 32, !dbg !39
  %58 = getelementptr inbounds nuw i8, ptr %55, i64 48, !dbg !39
  store <4 x i32> %50, ptr %55, align 4, !dbg !39, !tbaa !23
  store <4 x i32> %51, ptr %56, align 4, !dbg !39, !tbaa !23
  store <4 x i32> %52, ptr %57, align 4, !dbg !39, !tbaa !23
  store <4 x i32> %53, ptr %58, align 4, !dbg !39, !tbaa !23
  %59 = add nuw i64 %45, 16, !dbg !36
  %60 = icmp eq i64 %59, %43, !dbg !36
  br i1 %60, label %61, label %44, !dbg !36, !llvm.loop !40

61:                                               ; preds = %44
  %62 = icmp eq i64 %43, %32, !dbg !28
  br i1 %62, label %79, label %63, !dbg !28

63:                                               ; preds = %61
  %64 = and i64 %32, 12, !dbg !28
  %65 = icmp eq i64 %64, 0, !dbg !28
  br i1 %65, label %34, label %66, !dbg !28

66:                                               ; preds = %63, %40
  %67 = phi i64 [ %43, %63 ], [ 0, %40 ]
  %68 = and i64 %32, 2147483644, !dbg !28
  br label %69, !dbg !28

69:                                               ; preds = %69, %66
  %70 = phi i64 [ %67, %66 ], [ %75, %69 ], !dbg !36
  %71 = getelementptr inbounds nuw i32, ptr %2, i64 %70, !dbg !37
  %72 = load <4 x i32>, ptr %71, align 4, !dbg !37, !tbaa !23
  %73 = or disjoint i64 %70, 1, !dbg !36
  %74 = getelementptr inbounds nuw i32, ptr %1, i64 %73, !dbg !38
  store <4 x i32> %72, ptr %74, align 4, !dbg !39, !tbaa !23
  %75 = add nuw i64 %70, 4, !dbg !36
  %76 = icmp eq i64 %75, %68, !dbg !36
  br i1 %76, label %77, label %69, !dbg !36, !llvm.loop !45

77:                                               ; preds = %69
  %78 = icmp eq i64 %68, %32, !dbg !28
  br i1 %78, label %79, label %34, !dbg !28

79:                                               ; preds = %123, %77, %61
  %80 = add nuw nsw i32 %3, 1, !dbg !29
  %81 = zext nneg i32 %80 to i64, !dbg !30
  %82 = getelementptr inbounds nuw i32, ptr %1, i64 %81, !dbg !30
  store i32 %7, ptr %82, align 4, !dbg !31, !tbaa !23
  %83 = shl nuw nsw i64 %81, 2, !dbg !46
  %84 = alloca i8, i64 %83, align 8, !dbg !47
  %85 = zext nneg i32 %3 to i64, !dbg !48
  %86 = load i32, ptr %1, align 4, !dbg !49, !tbaa !23
  %87 = icmp ult i32 %3, 16, !dbg !50
  br i1 %87, label %120, label %88, !dbg !50

88:                                               ; preds = %79
  %89 = and i64 %32, 2147483632, !dbg !50
  %90 = insertelement <4 x i32> poison, i32 %86, i64 3, !dbg !50
  br label %91, !dbg !50

91:                                               ; preds = %91, %88
  %92 = phi i64 [ 0, %88 ], [ %115, %91 ], !dbg !51
  %93 = phi <4 x i32> [ %90, %88 ], [ %102, %91 ]
  %94 = or disjoint i64 %92, 1, !dbg !51
  %95 = getelementptr inbounds nuw i32, ptr %1, i64 %94, !dbg !52
  %96 = getelementptr inbounds nuw i8, ptr %95, i64 16, !dbg !52
  %97 = getelementptr inbounds nuw i8, ptr %95, i64 32, !dbg !52
  %98 = getelementptr inbounds nuw i8, ptr %95, i64 48, !dbg !52
  %99 = load <4 x i32>, ptr %95, align 4, !dbg !52, !tbaa !23
  %100 = load <4 x i32>, ptr %96, align 4, !dbg !52, !tbaa !23
  %101 = load <4 x i32>, ptr %97, align 4, !dbg !52, !tbaa !23
  %102 = load <4 x i32>, ptr %98, align 4, !dbg !52, !tbaa !23
  %103 = shufflevector <4 x i32> %93, <4 x i32> %99, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %104 = shufflevector <4 x i32> %99, <4 x i32> %100, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %105 = shufflevector <4 x i32> %100, <4 x i32> %101, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %106 = shufflevector <4 x i32> %101, <4 x i32> %102, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %107 = sub nsw <4 x i32> %99, %103, !dbg !53
  %108 = sub nsw <4 x i32> %100, %104, !dbg !53
  %109 = sub nsw <4 x i32> %101, %105, !dbg !53
  %110 = sub nsw <4 x i32> %102, %106, !dbg !53
  %111 = getelementptr inbounds nuw i32, ptr %84, i64 %92, !dbg !54
  %112 = getelementptr inbounds nuw i8, ptr %111, i64 16, !dbg !55
  %113 = getelementptr inbounds nuw i8, ptr %111, i64 32, !dbg !55
  %114 = getelementptr inbounds nuw i8, ptr %111, i64 48, !dbg !55
  store <4 x i32> %107, ptr %111, align 8, !dbg !55, !tbaa !23
  store <4 x i32> %108, ptr %112, align 8, !dbg !55, !tbaa !23
  store <4 x i32> %109, ptr %113, align 8, !dbg !55, !tbaa !23
  store <4 x i32> %110, ptr %114, align 8, !dbg !55, !tbaa !23
  %115 = add nuw i64 %92, 16, !dbg !51
  %116 = icmp eq i64 %115, %89, !dbg !51
  br i1 %116, label %117, label %91, !dbg !51, !llvm.loop !56

117:                                              ; preds = %91
  %118 = extractelement <4 x i32> %102, i64 3
  %119 = icmp eq i64 %89, %32, !dbg !50
  br i1 %119, label %130, label %120, !dbg !50

120:                                              ; preds = %79, %117
  %121 = phi i32 [ %86, %79 ], [ %118, %117 ]
  %122 = phi i64 [ 0, %79 ], [ %89, %117 ]
  br label %202, !dbg !50

123:                                              ; preds = %34, %123
  %124 = phi i64 [ %127, %123 ], [ %35, %34 ]
  %125 = getelementptr inbounds nuw i32, ptr %2, i64 %124, !dbg !37
  %126 = load i32, ptr %125, align 4, !dbg !37, !tbaa !23
  %127 = add nuw nsw i64 %124, 1, !dbg !36
  %128 = getelementptr inbounds nuw i32, ptr %1, i64 %127, !dbg !38
  store i32 %126, ptr %128, align 4, !dbg !39, !tbaa !23
  %129 = icmp eq i64 %127, %32, !dbg !27
  br i1 %129, label %79, label %123, !dbg !28, !llvm.loop !58

130:                                              ; preds = %202, %117
  %131 = alloca i8, i64 %83, align 8, !dbg !59
  %132 = zext nneg i32 %3 to i64, !dbg !60
  %133 = icmp ult i32 %3, 4, !dbg !61
  br i1 %133, label %134, label %136, !dbg !61

134:                                              ; preds = %177, %200, %130
  %135 = phi i64 [ %139, %177 ], [ 0, %130 ], [ %182, %200 ]
  br label %213, !dbg !61

136:                                              ; preds = %130
  %137 = icmp ult i32 %3, 16, !dbg !61
  br i1 %137, label %180, label %138, !dbg !61

138:                                              ; preds = %136
  %139 = and i64 %32, 2147483632, !dbg !61
  %140 = insertelement <4 x i1> poison, i1 %10, i64 0, !dbg !61
  %141 = shufflevector <4 x i1> %140, <4 x i1> poison, <4 x i32> zeroinitializer, !dbg !61
  %142 = xor <4 x i1> %141, <i1 false, i1 true, i1 false, i1 true>, !dbg !61
  %143 = insertelement <4 x float> poison, float %25, i64 0, !dbg !61
  %144 = shufflevector <4 x float> %143, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !61
  %145 = insertelement <4 x float> poison, float %24, i64 0, !dbg !61
  %146 = shufflevector <4 x float> %145, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !61
  %147 = select fast <4 x i1> %142, <4 x float> %144, <4 x float> %146
  %148 = select fast <4 x i1> %142, <4 x float> %144, <4 x float> %146
  %149 = select fast <4 x i1> %142, <4 x float> %144, <4 x float> %146
  %150 = select fast <4 x i1> %142, <4 x float> %144, <4 x float> %146
  br label %151, !dbg !61

151:                                              ; preds = %151, %138
  %152 = phi i64 [ 0, %138 ], [ %173, %151 ], !dbg !62
  %153 = getelementptr inbounds nuw i32, ptr %84, i64 %152, !dbg !63
  %154 = getelementptr inbounds nuw i8, ptr %153, i64 16, !dbg !63
  %155 = getelementptr inbounds nuw i8, ptr %153, i64 32, !dbg !63
  %156 = getelementptr inbounds nuw i8, ptr %153, i64 48, !dbg !63
  %157 = load <4 x i32>, ptr %153, align 8, !dbg !63, !tbaa !23
  %158 = load <4 x i32>, ptr %154, align 8, !dbg !63, !tbaa !23
  %159 = load <4 x i32>, ptr %155, align 8, !dbg !63, !tbaa !23
  %160 = load <4 x i32>, ptr %156, align 8, !dbg !63, !tbaa !23
  %161 = sitofp <4 x i32> %157 to <4 x float>, !dbg !63
  %162 = sitofp <4 x i32> %158 to <4 x float>, !dbg !63
  %163 = sitofp <4 x i32> %159 to <4 x float>, !dbg !63
  %164 = sitofp <4 x i32> %160 to <4 x float>, !dbg !63
  %165 = fmul fast <4 x float> %147, %161, !dbg !64
  %166 = fmul fast <4 x float> %148, %162, !dbg !64
  %167 = fmul fast <4 x float> %149, %163, !dbg !64
  %168 = fmul fast <4 x float> %150, %164, !dbg !64
  %169 = getelementptr inbounds nuw float, ptr %131, i64 %152, !dbg !65
  %170 = getelementptr inbounds nuw i8, ptr %169, i64 16, !dbg !66
  %171 = getelementptr inbounds nuw i8, ptr %169, i64 32, !dbg !66
  %172 = getelementptr inbounds nuw i8, ptr %169, i64 48, !dbg !66
  store <4 x float> %165, ptr %169, align 8, !dbg !66, !tbaa !33
  store <4 x float> %166, ptr %170, align 8, !dbg !66, !tbaa !33
  store <4 x float> %167, ptr %171, align 8, !dbg !66, !tbaa !33
  store <4 x float> %168, ptr %172, align 8, !dbg !66, !tbaa !33
  %173 = add nuw i64 %152, 16, !dbg !62
  %174 = icmp eq i64 %173, %139, !dbg !62
  br i1 %174, label %175, label %151, !dbg !62, !llvm.loop !67

175:                                              ; preds = %151
  %176 = icmp eq i64 %139, %32, !dbg !61
  br i1 %176, label %211, label %177, !dbg !61

177:                                              ; preds = %175
  %178 = and i64 %32, 12, !dbg !61
  %179 = icmp eq i64 %178, 0, !dbg !61
  br i1 %179, label %134, label %180, !dbg !61

180:                                              ; preds = %177, %136
  %181 = phi i64 [ %139, %177 ], [ 0, %136 ]
  %182 = and i64 %32, 2147483644, !dbg !61
  %183 = insertelement <4 x i1> poison, i1 %10, i64 0, !dbg !61
  %184 = shufflevector <4 x i1> %183, <4 x i1> poison, <4 x i32> zeroinitializer, !dbg !61
  %185 = xor <4 x i1> %184, <i1 false, i1 true, i1 false, i1 true>, !dbg !61
  %186 = insertelement <4 x float> poison, float %25, i64 0, !dbg !61
  %187 = shufflevector <4 x float> %186, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !61
  %188 = insertelement <4 x float> poison, float %24, i64 0, !dbg !61
  %189 = shufflevector <4 x float> %188, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !61
  %190 = select fast <4 x i1> %185, <4 x float> %187, <4 x float> %189
  br label %191, !dbg !61

191:                                              ; preds = %191, %180
  %192 = phi i64 [ %181, %180 ], [ %198, %191 ], !dbg !62
  %193 = getelementptr inbounds nuw i32, ptr %84, i64 %192, !dbg !63
  %194 = load <4 x i32>, ptr %193, align 8, !dbg !63, !tbaa !23
  %195 = sitofp <4 x i32> %194 to <4 x float>, !dbg !63
  %196 = fmul fast <4 x float> %190, %195, !dbg !64
  %197 = getelementptr inbounds nuw float, ptr %131, i64 %192, !dbg !65
  store <4 x float> %196, ptr %197, align 8, !dbg !66, !tbaa !33
  %198 = add nuw i64 %192, 4, !dbg !62
  %199 = icmp eq i64 %198, %182, !dbg !62
  br i1 %199, label %200, label %191, !dbg !62, !llvm.loop !69

200:                                              ; preds = %191
  %201 = icmp eq i64 %182, %32, !dbg !61
  br i1 %201, label %211, label %134, !dbg !61

202:                                              ; preds = %120, %202
  %203 = phi i32 [ %207, %202 ], [ %121, %120 ], !dbg !49
  %204 = phi i64 [ %205, %202 ], [ %122, %120 ]
  %205 = add nuw nsw i64 %204, 1, !dbg !51
  %206 = getelementptr inbounds nuw i32, ptr %1, i64 %205, !dbg !52
  %207 = load i32, ptr %206, align 4, !dbg !52, !tbaa !23
  %208 = sub nsw i32 %207, %203, !dbg !53
  %209 = getelementptr inbounds nuw i32, ptr %84, i64 %204, !dbg !54
  store i32 %208, ptr %209, align 4, !dbg !55, !tbaa !23
  %210 = icmp eq i64 %205, %85, !dbg !48
  br i1 %210, label %130, label %202, !dbg !50, !llvm.loop !70

211:                                              ; preds = %213, %200, %175
  store float %8, ptr %0, align 4, !dbg !32, !tbaa !33
  %212 = zext nneg i32 %3 to i64, !dbg !71
  br label %228, !dbg !35

213:                                              ; preds = %134, %213
  %214 = phi i64 [ %223, %213 ], [ %135, %134 ]
  %215 = phi i1 [ %222, %213 ], [ %10, %134 ]
  %216 = select fast i1 %215, float %25, float %24, !dbg !72
  %217 = getelementptr inbounds nuw i32, ptr %84, i64 %214, !dbg !63
  %218 = load i32, ptr %217, align 4, !dbg !63, !tbaa !23
  %219 = sitofp i32 %218 to float, !dbg !63
  %220 = fmul fast float %216, %219, !dbg !64
  %221 = getelementptr inbounds nuw float, ptr %131, i64 %214, !dbg !65
  store float %220, ptr %221, align 4, !dbg !66, !tbaa !33
  %222 = xor i1 %215, true, !dbg !73
  %223 = add nuw nsw i64 %214, 1, !dbg !62
  %224 = icmp eq i64 %223, %132, !dbg !60
  br i1 %224, label %211, label %213, !dbg !61, !llvm.loop !74

225:                                              ; preds = %228, %27
  %226 = phi i64 [ %29, %27 ], [ %81, %228 ]
  %227 = getelementptr inbounds float, ptr %0, i64 %226, !dbg !75
  store float %9, ptr %227, align 4, !dbg !76, !tbaa !33
  ret void, !dbg !77

228:                                              ; preds = %211, %228
  %229 = phi float [ %8, %211 ], [ %233, %228 ], !dbg !78
  %230 = phi i64 [ 0, %211 ], [ %234, %228 ]
  %231 = getelementptr inbounds nuw float, ptr %131, i64 %230, !dbg !79
  %232 = load float, ptr %231, align 4, !dbg !79, !tbaa !33
  %233 = fadd fast float %232, %229, !dbg !80
  %234 = add nuw nsw i64 %230, 1, !dbg !81
  %235 = getelementptr inbounds nuw float, ptr %0, i64 %234, !dbg !82
  store float %233, ptr %235, align 4, !dbg !83, !tbaa !33
  %236 = icmp eq i64 %234, %212, !dbg !71
  br i1 %236, label %225, label %228, !dbg !35, !llvm.loop !84
}

attributes #0 = { mustprogress nofree norecurse nosync nounwind ssp memory(argmem: readwrite) uwtable(sync) "approx-func-fp-math"="true" "frame-pointer"="non-leaf" "no-infs-fp-math"="true" "no-nans-fp-math"="true" "no-signed-zeros-fp-math"="true" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" "unsafe-fp-math"="true" }

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
!9 = distinct !DISubprogram(name: "set_points_opt", scope: !7, file: !7, line: 7, type: !10, scopeLine: 7, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !6)
!10 = !DISubroutineType(types: !11)
!11 = !{}
!12 = !DILocation(line: 8, column: 27, scope: !9)
!13 = !DILocation(line: 10, column: 23, scope: !9)
!14 = !DILocation(line: 10, column: 32, scope: !9)
!15 = !DILocation(line: 11, column: 9, scope: !9)
!16 = !DILocation(line: 12, column: 25, scope: !9)
!17 = !DILocation(line: 12, column: 58, scope: !9)
!18 = !DILocation(line: 12, column: 47, scope: !9)
!19 = !DILocation(line: 13, column: 5, scope: !9)
!20 = !DILocation(line: 14, column: 24, scope: !9)
!21 = !DILocation(line: 0, scope: !9)
!22 = !DILocation(line: 22, column: 12, scope: !9)
!23 = !{!24, !24, i64 0}
!24 = !{!"int", !25, i64 0}
!25 = !{!"omnipotent char", !26, i64 0}
!26 = !{!"Simple C++ TBAA"}
!27 = !DILocation(line: 23, column: 23, scope: !9)
!28 = !DILocation(line: 23, column: 5, scope: !9)
!29 = !DILocation(line: 26, column: 18, scope: !9)
!30 = !DILocation(line: 26, column: 5, scope: !9)
!31 = !DILocation(line: 26, column: 23, scope: !9)
!32 = !DILocation(line: 44, column: 12, scope: !9)
!33 = !{!34, !34, i64 0}
!34 = !{!"float", !25, i64 0}
!35 = !DILocation(line: 45, column: 5, scope: !9)
!36 = !DILocation(line: 24, column: 15, scope: !9)
!37 = !DILocation(line: 24, column: 22, scope: !9)
!38 = !DILocation(line: 24, column: 9, scope: !9)
!39 = !DILocation(line: 24, column: 20, scope: !9)
!40 = distinct !{!40, !28, !41, !42, !43, !44}
!41 = !DILocation(line: 25, column: 5, scope: !9)
!42 = !{!"llvm.loop.mustprogress"}
!43 = !{!"llvm.loop.isvectorized", i32 1}
!44 = !{!"llvm.loop.unroll.runtime.disable"}
!45 = distinct !{!45, !28, !41, !42, !43, !44}
!46 = !DILocation(line: 29, column: 59, scope: !9)
!47 = !DILocation(line: 29, column: 27, scope: !9)
!48 = !DILocation(line: 30, column: 23, scope: !9)
!49 = !DILocation(line: 31, column: 36, scope: !9)
!50 = !DILocation(line: 30, column: 5, scope: !9)
!51 = !DILocation(line: 31, column: 29, scope: !9)
!52 = !DILocation(line: 31, column: 23, scope: !9)
!53 = !DILocation(line: 31, column: 34, scope: !9)
!54 = !DILocation(line: 31, column: 9, scope: !9)
!55 = !DILocation(line: 31, column: 21, scope: !9)
!56 = distinct !{!56, !50, !57, !42, !43, !44}
!57 = !DILocation(line: 32, column: 5, scope: !9)
!58 = distinct !{!58, !28, !41, !42, !43}
!59 = !DILocation(line: 35, column: 31, scope: !9)
!60 = !DILocation(line: 37, column: 23, scope: !9)
!61 = !DILocation(line: 37, column: 5, scope: !9)
!62 = !DILocation(line: 37, column: 36, scope: !9)
!63 = !DILocation(line: 39, column: 34, scope: !9)
!64 = !DILocation(line: 39, column: 25, scope: !9)
!65 = !DILocation(line: 39, column: 9, scope: !9)
!66 = !DILocation(line: 39, column: 21, scope: !9)
!67 = distinct !{!67, !61, !68, !42, !43, !44}
!68 = !DILocation(line: 41, column: 5, scope: !9)
!69 = distinct !{!69, !61, !68, !42, !43, !44}
!70 = distinct !{!70, !50, !57, !42, !44, !43}
!71 = !DILocation(line: 45, column: 23, scope: !9)
!72 = !DILocation(line: 38, column: 19, scope: !9)
!73 = !DILocation(line: 40, column: 23, scope: !9)
!74 = distinct !{!74, !61, !68, !42, !44, !43}
!75 = !DILocation(line: 48, column: 5, scope: !9)
!76 = !DILocation(line: 48, column: 23, scope: !9)
!77 = !DILocation(line: 49, column: 1, scope: !9)
!78 = !DILocation(line: 46, column: 22, scope: !9)
!79 = !DILocation(line: 46, column: 31, scope: !9)
!80 = !DILocation(line: 46, column: 29, scope: !9)
!81 = !DILocation(line: 46, column: 15, scope: !9)
!82 = !DILocation(line: 46, column: 9, scope: !9)
!83 = !DILocation(line: 46, column: 20, scope: !9)
!84 = distinct !{!84, !35, !85, !42}
!85 = !DILocation(line: 47, column: 5, scope: !9)
