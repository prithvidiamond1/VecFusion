; ModuleID = 'temp_code.cpp'
source_filename = "temp_code.cpp"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: mustprogress nofree norecurse nosync nounwind ssp memory(argmem: readwrite) uwtable(sync)
define void @_Z14set_points_optPfPiPKiiiiiiffb(ptr nocapture noundef writeonly initializes((0, 4)) %0, ptr nocapture noundef initializes((0, 4)) %1, ptr nocapture noundef readonly %2, i32 noundef %3, i32 noundef %4, i32 noundef %5, i32 noundef %6, i32 noundef %7, float noundef nofpclass(nan inf) %8, float noundef nofpclass(nan inf) %9, i1 noundef zeroext %10) local_unnamed_addr #0 !dbg !9 {
  %12 = ptrtoint ptr %2 to i64, !dbg !12
  %13 = ptrtoint ptr %1 to i64, !dbg !12
  %14 = fsub fast float %9, %8, !dbg !12
  %15 = sitofp i32 %4 to float, !dbg !13
  %16 = fsub fast float %14, %15, !dbg !14
  %17 = sitofp i32 %5 to float, !dbg !15
  %18 = fcmp fast ult float %14, %15, !dbg !16
  store i32 %6, ptr %1, align 4, !dbg !17, !tbaa !18
  store float %8, ptr %0, align 4, !dbg !22, !tbaa !23
  %19 = icmp sgt i32 %3, 0, !dbg !25
  br i1 %19, label %20, label %237, !dbg !26

20:                                               ; preds = %11
  %21 = zext nneg i32 %3 to i64, !dbg !25
  %22 = icmp ult i32 %3, 4, !dbg !26
  br i1 %22, label %23, label %25, !dbg !26

23:                                               ; preds = %52, %66, %25, %20
  %24 = phi i64 [ %32, %52 ], [ 0, %20 ], [ 0, %25 ], [ %57, %66 ]
  br label %215, !dbg !26

25:                                               ; preds = %20
  %26 = add i64 %13, 4, !dbg !26
  %27 = sub i64 %26, %12, !dbg !26
  %28 = icmp ult i64 %27, 64, !dbg !26
  br i1 %28, label %23, label %29, !dbg !26

29:                                               ; preds = %25
  %30 = icmp ult i32 %3, 16, !dbg !26
  br i1 %30, label %55, label %31, !dbg !26

31:                                               ; preds = %29
  %32 = and i64 %21, 2147483632, !dbg !26
  br label %33, !dbg !26

33:                                               ; preds = %33, %31
  %34 = phi i64 [ 0, %31 ], [ %48, %33 ], !dbg !27
  %35 = getelementptr inbounds nuw i32, ptr %2, i64 %34, !dbg !28
  %36 = getelementptr inbounds nuw i8, ptr %35, i64 16, !dbg !28
  %37 = getelementptr inbounds nuw i8, ptr %35, i64 32, !dbg !28
  %38 = getelementptr inbounds nuw i8, ptr %35, i64 48, !dbg !28
  %39 = load <4 x i32>, ptr %35, align 4, !dbg !28, !tbaa !18
  %40 = load <4 x i32>, ptr %36, align 4, !dbg !28, !tbaa !18
  %41 = load <4 x i32>, ptr %37, align 4, !dbg !28, !tbaa !18
  %42 = load <4 x i32>, ptr %38, align 4, !dbg !28, !tbaa !18
  %43 = or disjoint i64 %34, 1, !dbg !27
  %44 = getelementptr inbounds nuw i32, ptr %1, i64 %43, !dbg !29
  %45 = getelementptr inbounds nuw i8, ptr %44, i64 16, !dbg !30
  %46 = getelementptr inbounds nuw i8, ptr %44, i64 32, !dbg !30
  %47 = getelementptr inbounds nuw i8, ptr %44, i64 48, !dbg !30
  store <4 x i32> %39, ptr %44, align 4, !dbg !30, !tbaa !18
  store <4 x i32> %40, ptr %45, align 4, !dbg !30, !tbaa !18
  store <4 x i32> %41, ptr %46, align 4, !dbg !30, !tbaa !18
  store <4 x i32> %42, ptr %47, align 4, !dbg !30, !tbaa !18
  %48 = add nuw i64 %34, 16, !dbg !27
  %49 = icmp eq i64 %48, %32, !dbg !27
  br i1 %49, label %50, label %33, !dbg !27, !llvm.loop !31

50:                                               ; preds = %33
  %51 = icmp eq i64 %32, %21, !dbg !26
  br i1 %51, label %68, label %52, !dbg !26

52:                                               ; preds = %50
  %53 = and i64 %21, 12, !dbg !26
  %54 = icmp eq i64 %53, 0, !dbg !26
  br i1 %54, label %23, label %55, !dbg !26

55:                                               ; preds = %52, %29
  %56 = phi i64 [ %32, %52 ], [ 0, %29 ]
  %57 = and i64 %21, 2147483644, !dbg !26
  br label %58, !dbg !26

58:                                               ; preds = %58, %55
  %59 = phi i64 [ %56, %55 ], [ %64, %58 ], !dbg !27
  %60 = getelementptr inbounds nuw i32, ptr %2, i64 %59, !dbg !28
  %61 = load <4 x i32>, ptr %60, align 4, !dbg !28, !tbaa !18
  %62 = or disjoint i64 %59, 1, !dbg !27
  %63 = getelementptr inbounds nuw i32, ptr %1, i64 %62, !dbg !29
  store <4 x i32> %61, ptr %63, align 4, !dbg !30, !tbaa !18
  %64 = add nuw i64 %59, 4, !dbg !27
  %65 = icmp eq i64 %64, %57, !dbg !27
  br i1 %65, label %66, label %58, !dbg !27, !llvm.loop !36

66:                                               ; preds = %58
  %67 = icmp eq i64 %57, %21, !dbg !26
  br i1 %67, label %68, label %23, !dbg !26

68:                                               ; preds = %215, %66, %50
  %69 = alloca float, i64 %21, align 4, !dbg !37
  %70 = icmp ult i32 %3, 16, !dbg !38
  br i1 %18, label %135, label %71

71:                                               ; preds = %68
  br i1 %70, label %131, label %72, !dbg !38

72:                                               ; preds = %71
  %73 = and i64 %21, 2147483632, !dbg !38
  %74 = insertelement <4 x i32> poison, i32 %6, i64 3, !dbg !38
  %75 = insertelement <4 x i1> poison, i1 %10, i64 0, !dbg !38
  %76 = shufflevector <4 x i1> %75, <4 x i1> poison, <4 x i32> zeroinitializer, !dbg !38
  %77 = xor <4 x i1> %76, <i1 false, i1 true, i1 false, i1 true>, !dbg !38
  %78 = insertelement <4 x float> poison, float %16, i64 0, !dbg !38
  %79 = shufflevector <4 x float> %78, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !38
  %80 = insertelement <4 x float> poison, float %17, i64 0, !dbg !38
  %81 = shufflevector <4 x float> %80, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !38
  %82 = fdiv fast <4 x float> splat (float 1.000000e+00), %81
  %83 = fdiv fast <4 x float> splat (float 1.000000e+00), %81
  %84 = fdiv fast <4 x float> splat (float 1.000000e+00), %81
  %85 = fdiv fast <4 x float> splat (float 1.000000e+00), %81
  br label %86, !dbg !38

86:                                               ; preds = %86, %72
  %87 = phi i64 [ 0, %72 ], [ %126, %86 ], !dbg !39
  %88 = phi <4 x i32> [ %74, %72 ], [ %97, %86 ]
  %89 = or disjoint i64 %87, 1, !dbg !39
  %90 = getelementptr inbounds nuw i32, ptr %1, i64 %89, !dbg !40
  %91 = getelementptr inbounds nuw i8, ptr %90, i64 16, !dbg !40
  %92 = getelementptr inbounds nuw i8, ptr %90, i64 32, !dbg !40
  %93 = getelementptr inbounds nuw i8, ptr %90, i64 48, !dbg !40
  %94 = load <4 x i32>, ptr %90, align 4, !dbg !40, !tbaa !18
  %95 = load <4 x i32>, ptr %91, align 4, !dbg !40, !tbaa !18
  %96 = load <4 x i32>, ptr %92, align 4, !dbg !40, !tbaa !18
  %97 = load <4 x i32>, ptr %93, align 4, !dbg !40, !tbaa !18
  %98 = shufflevector <4 x i32> %88, <4 x i32> %94, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %99 = shufflevector <4 x i32> %94, <4 x i32> %95, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %100 = shufflevector <4 x i32> %95, <4 x i32> %96, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %101 = shufflevector <4 x i32> %96, <4 x i32> %97, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %102 = sub nsw <4 x i32> %94, %98, !dbg !41
  %103 = sub nsw <4 x i32> %95, %99, !dbg !41
  %104 = sub nsw <4 x i32> %96, %100, !dbg !41
  %105 = sub nsw <4 x i32> %97, %101, !dbg !41
  %106 = sitofp <4 x i32> %102 to <4 x float>, !dbg !42
  %107 = sitofp <4 x i32> %103 to <4 x float>, !dbg !42
  %108 = sitofp <4 x i32> %104 to <4 x float>, !dbg !42
  %109 = sitofp <4 x i32> %105 to <4 x float>, !dbg !42
  %110 = fmul fast <4 x float> %79, %106
  %111 = fmul fast <4 x float> %79, %107
  %112 = fmul fast <4 x float> %79, %108
  %113 = fmul fast <4 x float> %79, %109
  %114 = fmul fast <4 x float> %110, %82
  %115 = fmul fast <4 x float> %111, %83
  %116 = fmul fast <4 x float> %112, %84
  %117 = fmul fast <4 x float> %113, %85
  %118 = select fast <4 x i1> %77, <4 x float> %114, <4 x float> %106, !dbg !42
  %119 = select fast <4 x i1> %77, <4 x float> %115, <4 x float> %107, !dbg !42
  %120 = select fast <4 x i1> %77, <4 x float> %116, <4 x float> %108, !dbg !42
  %121 = select fast <4 x i1> %77, <4 x float> %117, <4 x float> %109, !dbg !42
  %122 = getelementptr inbounds nuw float, ptr %69, i64 %87, !dbg !43
  %123 = getelementptr inbounds nuw i8, ptr %122, i64 16, !dbg !44
  %124 = getelementptr inbounds nuw i8, ptr %122, i64 32, !dbg !44
  %125 = getelementptr inbounds nuw i8, ptr %122, i64 48, !dbg !44
  store <4 x float> %118, ptr %122, align 4, !dbg !44, !tbaa !23
  store <4 x float> %119, ptr %123, align 4, !dbg !44, !tbaa !23
  store <4 x float> %120, ptr %124, align 4, !dbg !44, !tbaa !23
  store <4 x float> %121, ptr %125, align 4, !dbg !44, !tbaa !23
  %126 = add nuw i64 %87, 16, !dbg !39
  %127 = icmp eq i64 %126, %73, !dbg !39
  br i1 %127, label %128, label %86, !dbg !39, !llvm.loop !45

128:                                              ; preds = %86
  %129 = extractelement <4 x i32> %97, i64 3
  %130 = icmp eq i64 %73, %21, !dbg !38
  br i1 %130, label %214, label %131, !dbg !38

131:                                              ; preds = %71, %128
  %132 = phi i32 [ %6, %71 ], [ %129, %128 ]
  %133 = phi i64 [ 0, %71 ], [ %73, %128 ]
  %134 = fdiv fast float 1.000000e+00, %17
  br label %222, !dbg !38

135:                                              ; preds = %68
  br i1 %70, label %195, label %136, !dbg !38

136:                                              ; preds = %135
  %137 = and i64 %21, 2147483632, !dbg !38
  %138 = insertelement <4 x i32> poison, i32 %6, i64 3, !dbg !38
  %139 = insertelement <4 x i1> poison, i1 %10, i64 0, !dbg !38
  %140 = shufflevector <4 x i1> %139, <4 x i1> poison, <4 x i32> zeroinitializer, !dbg !38
  %141 = xor <4 x i1> %140, <i1 false, i1 true, i1 false, i1 true>, !dbg !38
  %142 = insertelement <4 x float> poison, float %14, i64 0, !dbg !38
  %143 = shufflevector <4 x float> %142, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !38
  %144 = insertelement <4 x float> poison, float %15, i64 0, !dbg !38
  %145 = shufflevector <4 x float> %144, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !38
  %146 = fdiv fast <4 x float> splat (float 1.000000e+00), %145
  %147 = fdiv fast <4 x float> splat (float 1.000000e+00), %145
  %148 = fdiv fast <4 x float> splat (float 1.000000e+00), %145
  %149 = fdiv fast <4 x float> splat (float 1.000000e+00), %145
  br label %150, !dbg !38

150:                                              ; preds = %150, %136
  %151 = phi i64 [ 0, %136 ], [ %190, %150 ], !dbg !39
  %152 = phi <4 x i32> [ %138, %136 ], [ %161, %150 ]
  %153 = or disjoint i64 %151, 1, !dbg !39
  %154 = getelementptr inbounds nuw i32, ptr %1, i64 %153, !dbg !40
  %155 = getelementptr inbounds nuw i8, ptr %154, i64 16, !dbg !40
  %156 = getelementptr inbounds nuw i8, ptr %154, i64 32, !dbg !40
  %157 = getelementptr inbounds nuw i8, ptr %154, i64 48, !dbg !40
  %158 = load <4 x i32>, ptr %154, align 4, !dbg !40, !tbaa !18
  %159 = load <4 x i32>, ptr %155, align 4, !dbg !40, !tbaa !18
  %160 = load <4 x i32>, ptr %156, align 4, !dbg !40, !tbaa !18
  %161 = load <4 x i32>, ptr %157, align 4, !dbg !40, !tbaa !18
  %162 = shufflevector <4 x i32> %152, <4 x i32> %158, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %163 = shufflevector <4 x i32> %158, <4 x i32> %159, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %164 = shufflevector <4 x i32> %159, <4 x i32> %160, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %165 = shufflevector <4 x i32> %160, <4 x i32> %161, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %166 = sub nsw <4 x i32> %158, %162, !dbg !41
  %167 = sub nsw <4 x i32> %159, %163, !dbg !41
  %168 = sub nsw <4 x i32> %160, %164, !dbg !41
  %169 = sub nsw <4 x i32> %161, %165, !dbg !41
  %170 = sitofp <4 x i32> %166 to <4 x float>, !dbg !47
  %171 = sitofp <4 x i32> %167 to <4 x float>, !dbg !47
  %172 = sitofp <4 x i32> %168 to <4 x float>, !dbg !47
  %173 = sitofp <4 x i32> %169 to <4 x float>, !dbg !47
  %174 = fmul fast <4 x float> %143, %170
  %175 = fmul fast <4 x float> %143, %171
  %176 = fmul fast <4 x float> %143, %172
  %177 = fmul fast <4 x float> %143, %173
  %178 = fmul fast <4 x float> %174, %146
  %179 = fmul fast <4 x float> %175, %147
  %180 = fmul fast <4 x float> %176, %148
  %181 = fmul fast <4 x float> %177, %149
  %182 = select fast <4 x i1> %141, <4 x float> zeroinitializer, <4 x float> %178, !dbg !47
  %183 = select fast <4 x i1> %141, <4 x float> zeroinitializer, <4 x float> %179, !dbg !47
  %184 = select fast <4 x i1> %141, <4 x float> zeroinitializer, <4 x float> %180, !dbg !47
  %185 = select fast <4 x i1> %141, <4 x float> zeroinitializer, <4 x float> %181, !dbg !47
  %186 = getelementptr inbounds nuw float, ptr %69, i64 %151, !dbg !48
  %187 = getelementptr inbounds nuw i8, ptr %186, i64 16, !dbg !49
  %188 = getelementptr inbounds nuw i8, ptr %186, i64 32, !dbg !49
  %189 = getelementptr inbounds nuw i8, ptr %186, i64 48, !dbg !49
  store <4 x float> %182, ptr %186, align 4, !dbg !49, !tbaa !23
  store <4 x float> %183, ptr %187, align 4, !dbg !49, !tbaa !23
  store <4 x float> %184, ptr %188, align 4, !dbg !49, !tbaa !23
  store <4 x float> %185, ptr %189, align 4, !dbg !49, !tbaa !23
  %190 = add nuw i64 %151, 16, !dbg !39
  %191 = icmp eq i64 %190, %137, !dbg !39
  br i1 %191, label %192, label %150, !dbg !39, !llvm.loop !50

192:                                              ; preds = %150
  %193 = extractelement <4 x i32> %161, i64 3
  %194 = icmp eq i64 %137, %21, !dbg !38
  br i1 %194, label %214, label %195, !dbg !38

195:                                              ; preds = %135, %192
  %196 = phi i32 [ %6, %135 ], [ %193, %192 ]
  %197 = phi i64 [ 0, %135 ], [ %137, %192 ]
  %198 = fdiv fast float 1.000000e+00, %15
  br label %199, !dbg !38

199:                                              ; preds = %195, %199
  %200 = phi i32 [ %205, %199 ], [ %196, %195 ], !dbg !51
  %201 = phi i64 [ %203, %199 ], [ %197, %195 ]
  %202 = phi i1 [ %212, %199 ], [ %10, %195 ]
  %203 = add nuw nsw i64 %201, 1, !dbg !39
  %204 = getelementptr inbounds nuw i32, ptr %1, i64 %203, !dbg !40
  %205 = load i32, ptr %204, align 4, !dbg !40, !tbaa !18
  %206 = sub nsw i32 %205, %200, !dbg !41
  %207 = sitofp i32 %206 to float, !dbg !47
  %208 = fmul fast float %14, %207
  %209 = fmul fast float %208, %198
  %210 = select fast i1 %202, float 0.000000e+00, float %209, !dbg !47
  %211 = getelementptr inbounds nuw float, ptr %69, i64 %201, !dbg !48
  store float %210, ptr %211, align 4, !dbg !49, !tbaa !23
  %212 = xor i1 %202, true, !dbg !52
  %213 = icmp eq i64 %203, %21, !dbg !53
  br i1 %213, label %214, label %199, !dbg !38, !llvm.loop !54

214:                                              ; preds = %222, %199, %128, %192
  br label %242, !dbg !55

215:                                              ; preds = %23, %215
  %216 = phi i64 [ %219, %215 ], [ %24, %23 ]
  %217 = getelementptr inbounds nuw i32, ptr %2, i64 %216, !dbg !28
  %218 = load i32, ptr %217, align 4, !dbg !28, !tbaa !18
  %219 = add nuw nsw i64 %216, 1, !dbg !27
  %220 = getelementptr inbounds nuw i32, ptr %1, i64 %219, !dbg !29
  store i32 %218, ptr %220, align 4, !dbg !30, !tbaa !18
  %221 = icmp eq i64 %219, %21, !dbg !25
  br i1 %221, label %68, label %215, !dbg !26, !llvm.loop !56

222:                                              ; preds = %131, %222
  %223 = phi i32 [ %228, %222 ], [ %132, %131 ], !dbg !51
  %224 = phi i64 [ %226, %222 ], [ %133, %131 ]
  %225 = phi i1 [ %235, %222 ], [ %10, %131 ]
  %226 = add nuw nsw i64 %224, 1, !dbg !39
  %227 = getelementptr inbounds nuw i32, ptr %1, i64 %226, !dbg !40
  %228 = load i32, ptr %227, align 4, !dbg !40, !tbaa !18
  %229 = sub nsw i32 %228, %223, !dbg !41
  %230 = sitofp i32 %229 to float, !dbg !42
  %231 = fmul fast float %16, %230
  %232 = fmul fast float %231, %134
  %233 = select fast i1 %225, float %232, float %230, !dbg !42
  %234 = getelementptr inbounds nuw float, ptr %69, i64 %224, !dbg !43
  store float %233, ptr %234, align 4, !dbg !44, !tbaa !23
  %235 = xor i1 %225, true, !dbg !52
  %236 = icmp eq i64 %226, %21, !dbg !53
  br i1 %236, label %214, label %222, !dbg !38, !llvm.loop !57

237:                                              ; preds = %242, %11
  %238 = add nsw i32 %3, 1, !dbg !58
  %239 = sext i32 %238 to i64, !dbg !59
  %240 = getelementptr inbounds i32, ptr %1, i64 %239, !dbg !59
  store i32 %7, ptr %240, align 4, !dbg !60, !tbaa !18
  %241 = getelementptr inbounds float, ptr %0, i64 %239, !dbg !61
  store float %9, ptr %241, align 4, !dbg !62, !tbaa !23
  ret void, !dbg !63

242:                                              ; preds = %214, %242
  %243 = phi float [ %247, %242 ], [ %8, %214 ], !dbg !64
  %244 = phi i64 [ %248, %242 ], [ 0, %214 ]
  %245 = getelementptr inbounds nuw float, ptr %69, i64 %244, !dbg !65
  %246 = load float, ptr %245, align 4, !dbg !65, !tbaa !23
  %247 = fadd fast float %246, %243, !dbg !66
  %248 = add nuw nsw i64 %244, 1, !dbg !67
  %249 = getelementptr inbounds nuw float, ptr %0, i64 %248, !dbg !68
  store float %247, ptr %249, align 4, !dbg !69, !tbaa !23
  %250 = icmp eq i64 %248, %21, !dbg !70
  br i1 %250, label %237, label %242, !dbg !55, !llvm.loop !71
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
!9 = distinct !DISubprogram(name: "set_points_opt", scope: !7, file: !7, line: 7, type: !10, scopeLine: 9, flags: DIFlagPrototyped, spFlags: DISPFlagDefinition | DISPFlagOptimized, unit: !6)
!10 = !DISubroutineType(types: !11)
!11 = !{}
!12 = !DILocation(line: 10, column: 27, scope: !9)
!13 = !DILocation(line: 12, column: 45, scope: !9)
!14 = !DILocation(line: 12, column: 34, scope: !9)
!15 = !DILocation(line: 12, column: 67, scope: !9)
!16 = !DILocation(line: 15, column: 31, scope: !9)
!17 = !DILocation(line: 18, column: 12, scope: !9)
!18 = !{!19, !19, i64 0}
!19 = !{!"int", !20, i64 0}
!20 = !{!"omnipotent char", !21, i64 0}
!21 = !{!"Simple C++ TBAA"}
!22 = !DILocation(line: 19, column: 12, scope: !9)
!23 = !{!24, !24, i64 0}
!24 = !{!"float", !20, i64 0}
!25 = !DILocation(line: 22, column: 23, scope: !9)
!26 = !DILocation(line: 22, column: 5, scope: !9)
!27 = !DILocation(line: 23, column: 15, scope: !9)
!28 = !DILocation(line: 23, column: 22, scope: !9)
!29 = !DILocation(line: 23, column: 9, scope: !9)
!30 = !DILocation(line: 23, column: 20, scope: !9)
!31 = distinct !{!31, !26, !32, !33, !34, !35}
!32 = !DILocation(line: 24, column: 5, scope: !9)
!33 = !{!"llvm.loop.mustprogress"}
!34 = !{!"llvm.loop.isvectorized", i32 1}
!35 = !{!"llvm.loop.unroll.runtime.disable"}
!36 = distinct !{!36, !26, !32, !33, !34, !35}
!37 = !DILocation(line: 27, column: 5, scope: !9)
!38 = !DILocation(line: 30, column: 5, scope: !9)
!39 = !DILocation(line: 31, column: 30, scope: !9)
!40 = !DILocation(line: 31, column: 24, scope: !9)
!41 = !DILocation(line: 31, column: 35, scope: !9)
!42 = !DILocation(line: 34, column: 27, scope: !9)
!43 = !DILocation(line: 34, column: 13, scope: !9)
!44 = !DILocation(line: 34, column: 25, scope: !9)
!45 = distinct !{!45, !38, !46, !33, !34, !35}
!46 = !DILocation(line: 40, column: 5, scope: !9)
!47 = !DILocation(line: 36, column: 27, scope: !9)
!48 = !DILocation(line: 36, column: 13, scope: !9)
!49 = !DILocation(line: 36, column: 25, scope: !9)
!50 = distinct !{!50, !38, !46, !33, !34, !35}
!51 = !DILocation(line: 31, column: 37, scope: !9)
!52 = !DILocation(line: 39, column: 28, scope: !9)
!53 = !DILocation(line: 30, column: 23, scope: !9)
!54 = distinct !{!54, !38, !46, !33, !35, !34}
!55 = !DILocation(line: 43, column: 5, scope: !9)
!56 = distinct !{!56, !26, !32, !33, !34}
!57 = distinct !{!57, !38, !46, !33, !35, !34}
!58 = !DILocation(line: 47, column: 18, scope: !9)
!59 = !DILocation(line: 47, column: 5, scope: !9)
!60 = !DILocation(line: 47, column: 23, scope: !9)
!61 = !DILocation(line: 48, column: 5, scope: !9)
!62 = !DILocation(line: 48, column: 23, scope: !9)
!63 = !DILocation(line: 49, column: 1, scope: !9)
!64 = !DILocation(line: 44, column: 22, scope: !9)
!65 = !DILocation(line: 44, column: 31, scope: !9)
!66 = !DILocation(line: 44, column: 29, scope: !9)
!67 = !DILocation(line: 44, column: 15, scope: !9)
!68 = !DILocation(line: 44, column: 9, scope: !9)
!69 = !DILocation(line: 44, column: 20, scope: !9)
!70 = !DILocation(line: 43, column: 23, scope: !9)
!71 = distinct !{!71, !55, !72, !33}
!72 = !DILocation(line: 45, column: 5, scope: !9)
