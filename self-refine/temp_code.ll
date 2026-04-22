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
  %16 = fcmp fast oge float %14, %15, !dbg !14
  br i1 %16, label %17, label %21, !dbg !15

17:                                               ; preds = %11
  %18 = fsub fast float %14, %15, !dbg !16
  %19 = sitofp i32 %5 to float, !dbg !17
  %20 = fdiv fast float %18, %19, !dbg !18
  br label %23, !dbg !19

21:                                               ; preds = %11
  %22 = fdiv fast float %14, %15, !dbg !20
  br label %23

23:                                               ; preds = %21, %17
  %24 = phi nsz float [ %20, %17 ], [ %22, %21 ], !dbg !21
  store i32 %6, ptr %1, align 4, !dbg !22, !tbaa !23
  store float %8, ptr %0, align 4, !dbg !27, !tbaa !28
  %25 = icmp sgt i32 %3, 0, !dbg !30
  br i1 %25, label %31, label %26, !dbg !31

26:                                               ; preds = %23
  %27 = sext i32 %3 to i64, !dbg !32
  %28 = shl nsw i64 %27, 2, !dbg !33
  %29 = alloca i8, i64 %28, align 8, !dbg !34
  %30 = alloca i8, i64 %28, align 8, !dbg !35
  br label %139, !dbg !36

31:                                               ; preds = %23
  %32 = zext nneg i32 %3 to i64, !dbg !30
  %33 = icmp ult i32 %3, 16, !dbg !31
  br i1 %33, label %34, label %36, !dbg !31

34:                                               ; preds = %59, %36, %31
  %35 = phi i64 [ 0, %36 ], [ 0, %31 ], [ %41, %59 ]
  br label %100, !dbg !31

36:                                               ; preds = %31
  %37 = add i64 %13, 4, !dbg !31
  %38 = sub i64 %37, %12, !dbg !31
  %39 = icmp ult i64 %38, 64, !dbg !31
  br i1 %39, label %34, label %40, !dbg !31

40:                                               ; preds = %36
  %41 = and i64 %32, 2147483632, !dbg !31
  br label %42, !dbg !31

42:                                               ; preds = %42, %40
  %43 = phi i64 [ 0, %40 ], [ %57, %42 ], !dbg !37
  %44 = getelementptr inbounds i32, ptr %2, i64 %43, !dbg !38
  %45 = getelementptr inbounds i8, ptr %44, i64 16, !dbg !38
  %46 = getelementptr inbounds i8, ptr %44, i64 32, !dbg !38
  %47 = getelementptr inbounds i8, ptr %44, i64 48, !dbg !38
  %48 = load <4 x i32>, ptr %44, align 4, !dbg !38, !tbaa !23
  %49 = load <4 x i32>, ptr %45, align 4, !dbg !38, !tbaa !23
  %50 = load <4 x i32>, ptr %46, align 4, !dbg !38, !tbaa !23
  %51 = load <4 x i32>, ptr %47, align 4, !dbg !38, !tbaa !23
  %52 = or disjoint i64 %43, 1, !dbg !37
  %53 = getelementptr inbounds i32, ptr %1, i64 %52, !dbg !39
  %54 = getelementptr inbounds i8, ptr %53, i64 16, !dbg !40
  %55 = getelementptr inbounds i8, ptr %53, i64 32, !dbg !40
  %56 = getelementptr inbounds i8, ptr %53, i64 48, !dbg !40
  store <4 x i32> %48, ptr %53, align 4, !dbg !40, !tbaa !23
  store <4 x i32> %49, ptr %54, align 4, !dbg !40, !tbaa !23
  store <4 x i32> %50, ptr %55, align 4, !dbg !40, !tbaa !23
  store <4 x i32> %51, ptr %56, align 4, !dbg !40, !tbaa !23
  %57 = add nuw i64 %43, 16, !dbg !37
  %58 = icmp eq i64 %57, %41, !dbg !37
  br i1 %58, label %59, label %42, !dbg !37, !llvm.loop !41

59:                                               ; preds = %42
  %60 = icmp eq i64 %41, %32, !dbg !31
  br i1 %60, label %61, label %34, !dbg !31

61:                                               ; preds = %100, %59
  %62 = shl nuw nsw i64 %32, 2, !dbg !33
  %63 = alloca i8, i64 %62, align 8, !dbg !34
  %64 = load i32, ptr %1, align 4, !dbg !46, !tbaa !23
  br i1 %33, label %65, label %68, !dbg !47

65:                                               ; preds = %97, %61
  %66 = phi i32 [ %64, %61 ], [ %98, %97 ]
  %67 = phi i64 [ 0, %61 ], [ %69, %97 ]
  br label %130, !dbg !47

68:                                               ; preds = %61
  %69 = and i64 %32, 2147483632, !dbg !47
  %70 = insertelement <4 x i32> poison, i32 %64, i64 3, !dbg !47
  br label %71, !dbg !47

71:                                               ; preds = %71, %68
  %72 = phi i64 [ 0, %68 ], [ %95, %71 ], !dbg !48
  %73 = phi <4 x i32> [ %70, %68 ], [ %82, %71 ]
  %74 = or disjoint i64 %72, 1, !dbg !48
  %75 = getelementptr inbounds i32, ptr %1, i64 %74, !dbg !49
  %76 = getelementptr inbounds i8, ptr %75, i64 16, !dbg !49
  %77 = getelementptr inbounds i8, ptr %75, i64 32, !dbg !49
  %78 = getelementptr inbounds i8, ptr %75, i64 48, !dbg !49
  %79 = load <4 x i32>, ptr %75, align 4, !dbg !49, !tbaa !23
  %80 = load <4 x i32>, ptr %76, align 4, !dbg !49, !tbaa !23
  %81 = load <4 x i32>, ptr %77, align 4, !dbg !49, !tbaa !23
  %82 = load <4 x i32>, ptr %78, align 4, !dbg !49, !tbaa !23
  %83 = shufflevector <4 x i32> %73, <4 x i32> %79, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %84 = shufflevector <4 x i32> %79, <4 x i32> %80, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %85 = shufflevector <4 x i32> %80, <4 x i32> %81, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %86 = shufflevector <4 x i32> %81, <4 x i32> %82, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %87 = sub nsw <4 x i32> %79, %83, !dbg !50
  %88 = sub nsw <4 x i32> %80, %84, !dbg !50
  %89 = sub nsw <4 x i32> %81, %85, !dbg !50
  %90 = sub nsw <4 x i32> %82, %86, !dbg !50
  %91 = getelementptr inbounds i32, ptr %63, i64 %72, !dbg !51
  %92 = getelementptr inbounds i8, ptr %91, i64 16, !dbg !52
  %93 = getelementptr inbounds i8, ptr %91, i64 32, !dbg !52
  %94 = getelementptr inbounds i8, ptr %91, i64 48, !dbg !52
  store <4 x i32> %87, ptr %91, align 8, !dbg !52, !tbaa !23
  store <4 x i32> %88, ptr %92, align 8, !dbg !52, !tbaa !23
  store <4 x i32> %89, ptr %93, align 8, !dbg !52, !tbaa !23
  store <4 x i32> %90, ptr %94, align 8, !dbg !52, !tbaa !23
  %95 = add nuw i64 %72, 16, !dbg !48
  %96 = icmp eq i64 %95, %69, !dbg !48
  br i1 %96, label %97, label %71, !dbg !48, !llvm.loop !53

97:                                               ; preds = %71
  %98 = extractelement <4 x i32> %82, i64 3
  %99 = icmp eq i64 %69, %32, !dbg !47
  br i1 %99, label %107, label %65, !dbg !47

100:                                              ; preds = %34, %100
  %101 = phi i64 [ %104, %100 ], [ %35, %34 ]
  %102 = getelementptr inbounds i32, ptr %2, i64 %101, !dbg !38
  %103 = load i32, ptr %102, align 4, !dbg !38, !tbaa !23
  %104 = add nuw nsw i64 %101, 1, !dbg !37
  %105 = getelementptr inbounds i32, ptr %1, i64 %104, !dbg !39
  store i32 %103, ptr %105, align 4, !dbg !40, !tbaa !23
  %106 = icmp eq i64 %104, %32, !dbg !30
  br i1 %106, label %61, label %100, !dbg !31, !llvm.loop !55

107:                                              ; preds = %130, %97
  %108 = alloca i8, i64 %62, align 8, !dbg !35
  %109 = zext i1 %10 to i32, !dbg !56
  br i1 %33, label %110, label %112, !dbg !36

110:                                              ; preds = %128, %107
  %111 = phi i64 [ 0, %107 ], [ %113, %128 ]
  br label %260, !dbg !36

112:                                              ; preds = %107
  %113 = and i64 %32, 2147483632, !dbg !36
  %114 = insertelement <4 x i32> poison, i32 %109, i64 0, !dbg !36
  %115 = shufflevector <4 x i32> %114, <4 x i32> poison, <4 x i32> zeroinitializer, !dbg !36
  br label %116, !dbg !36

116:                                              ; preds = %116, %112
  %117 = phi i64 [ 0, %112 ], [ %125, %116 ], !dbg !57
  %118 = phi <4 x i32> [ <i32 0, i32 1, i32 2, i32 3>, %112 ], [ %126, %116 ], !dbg !58
  %119 = and <4 x i32> %118, <i32 1, i32 1, i32 1, i32 1>, !dbg !58
  %120 = xor <4 x i32> %119, %115, !dbg !59
  %121 = getelementptr inbounds i32, ptr %108, i64 %117, !dbg !60
  %122 = getelementptr inbounds i8, ptr %121, i64 16, !dbg !61
  %123 = getelementptr inbounds i8, ptr %121, i64 32, !dbg !61
  %124 = getelementptr inbounds i8, ptr %121, i64 48, !dbg !61
  store <4 x i32> %120, ptr %121, align 8, !dbg !61, !tbaa !23
  store <4 x i32> %120, ptr %122, align 8, !dbg !61, !tbaa !23
  store <4 x i32> %120, ptr %123, align 8, !dbg !61, !tbaa !23
  store <4 x i32> %120, ptr %124, align 8, !dbg !61, !tbaa !23
  %125 = add nuw i64 %117, 16, !dbg !57
  %126 = add <4 x i32> %118, <i32 16, i32 16, i32 16, i32 16>, !dbg !58
  %127 = icmp eq i64 %125, %113, !dbg !57
  br i1 %127, label %128, label %116, !dbg !57, !llvm.loop !62

128:                                              ; preds = %116
  %129 = icmp eq i64 %113, %32, !dbg !36
  br i1 %129, label %139, label %110, !dbg !36

130:                                              ; preds = %65, %130
  %131 = phi i32 [ %135, %130 ], [ %66, %65 ], !dbg !46
  %132 = phi i64 [ %133, %130 ], [ %67, %65 ]
  %133 = add nuw nsw i64 %132, 1, !dbg !48
  %134 = getelementptr inbounds i32, ptr %1, i64 %133, !dbg !49
  %135 = load i32, ptr %134, align 4, !dbg !49, !tbaa !23
  %136 = sub nsw i32 %135, %131, !dbg !50
  %137 = getelementptr inbounds i32, ptr %63, i64 %132, !dbg !51
  store i32 %136, ptr %137, align 4, !dbg !52, !tbaa !23
  %138 = icmp eq i64 %133, %32, !dbg !64
  br i1 %138, label %107, label %130, !dbg !47, !llvm.loop !65

139:                                              ; preds = %260, %128, %26
  %140 = phi ptr [ %30, %26 ], [ %108, %128 ], [ %108, %260 ]
  %141 = phi i64 [ %28, %26 ], [ %62, %128 ], [ %62, %260 ]
  %142 = phi ptr [ %29, %26 ], [ %63, %128 ], [ %63, %260 ]
  %143 = alloca i8, i64 %141, align 8, !dbg !66
  br i1 %16, label %200, label %144, !dbg !67

144:                                              ; preds = %139
  br i1 %25, label %145, label %300, !dbg !68

145:                                              ; preds = %144
  %146 = zext nneg i32 %3 to i64, !dbg !69
  %147 = icmp ult i32 %3, 16, !dbg !68
  br i1 %147, label %148, label %150, !dbg !68

148:                                              ; preds = %198, %145
  %149 = phi i64 [ 0, %145 ], [ %151, %198 ]
  br label %283, !dbg !68

150:                                              ; preds = %145
  %151 = and i64 %146, 2147483632, !dbg !68
  %152 = insertelement <4 x float> poison, float %24, i64 0, !dbg !68
  %153 = shufflevector <4 x float> %152, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !68
  br label %154, !dbg !68

154:                                              ; preds = %154, %150
  %155 = phi i64 [ 0, %150 ], [ %196, %154 ], !dbg !70
  %156 = getelementptr inbounds i32, ptr %142, i64 %155, !dbg !71
  %157 = getelementptr inbounds i8, ptr %156, i64 16, !dbg !71
  %158 = getelementptr inbounds i8, ptr %156, i64 32, !dbg !71
  %159 = getelementptr inbounds i8, ptr %156, i64 48, !dbg !71
  %160 = load <4 x i32>, ptr %156, align 8, !dbg !71, !tbaa !23
  %161 = load <4 x i32>, ptr %157, align 8, !dbg !71, !tbaa !23
  %162 = load <4 x i32>, ptr %158, align 8, !dbg !71, !tbaa !23
  %163 = load <4 x i32>, ptr %159, align 8, !dbg !71, !tbaa !23
  %164 = sitofp <4 x i32> %160 to <4 x float>, !dbg !71
  %165 = sitofp <4 x i32> %161 to <4 x float>, !dbg !71
  %166 = sitofp <4 x i32> %162 to <4 x float>, !dbg !71
  %167 = sitofp <4 x i32> %163 to <4 x float>, !dbg !71
  %168 = fmul fast <4 x float> %153, %164, !dbg !72
  %169 = fmul fast <4 x float> %153, %165, !dbg !72
  %170 = fmul fast <4 x float> %153, %166, !dbg !72
  %171 = fmul fast <4 x float> %153, %167, !dbg !72
  %172 = getelementptr inbounds i32, ptr %140, i64 %155, !dbg !73
  %173 = getelementptr inbounds i8, ptr %172, i64 16, !dbg !73
  %174 = getelementptr inbounds i8, ptr %172, i64 32, !dbg !73
  %175 = getelementptr inbounds i8, ptr %172, i64 48, !dbg !73
  %176 = load <4 x i32>, ptr %172, align 8, !dbg !73, !tbaa !23
  %177 = load <4 x i32>, ptr %173, align 8, !dbg !73, !tbaa !23
  %178 = load <4 x i32>, ptr %174, align 8, !dbg !73, !tbaa !23
  %179 = load <4 x i32>, ptr %175, align 8, !dbg !73, !tbaa !23
  %180 = sitofp <4 x i32> %176 to <4 x float>, !dbg !73
  %181 = sitofp <4 x i32> %177 to <4 x float>, !dbg !73
  %182 = sitofp <4 x i32> %178 to <4 x float>, !dbg !73
  %183 = sitofp <4 x i32> %179 to <4 x float>, !dbg !73
  %184 = fsub fast <4 x float> <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>, %180, !dbg !74
  %185 = fsub fast <4 x float> <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>, %181, !dbg !74
  %186 = fsub fast <4 x float> <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>, %182, !dbg !74
  %187 = fsub fast <4 x float> <float 1.000000e+00, float 1.000000e+00, float 1.000000e+00, float 1.000000e+00>, %183, !dbg !74
  %188 = fmul fast <4 x float> %168, %184, !dbg !75
  %189 = fmul fast <4 x float> %169, %185, !dbg !75
  %190 = fmul fast <4 x float> %170, %186, !dbg !75
  %191 = fmul fast <4 x float> %171, %187, !dbg !75
  %192 = getelementptr inbounds float, ptr %143, i64 %155, !dbg !76
  %193 = getelementptr inbounds i8, ptr %192, i64 16, !dbg !77
  %194 = getelementptr inbounds i8, ptr %192, i64 32, !dbg !77
  %195 = getelementptr inbounds i8, ptr %192, i64 48, !dbg !77
  store <4 x float> %188, ptr %192, align 8, !dbg !77, !tbaa !28
  store <4 x float> %189, ptr %193, align 8, !dbg !77, !tbaa !28
  store <4 x float> %190, ptr %194, align 8, !dbg !77, !tbaa !28
  store <4 x float> %191, ptr %195, align 8, !dbg !77, !tbaa !28
  %196 = add nuw i64 %155, 16, !dbg !70
  %197 = icmp eq i64 %196, %151, !dbg !70
  br i1 %197, label %198, label %154, !dbg !70, !llvm.loop !78

198:                                              ; preds = %154
  %199 = icmp eq i64 %151, %146, !dbg !68
  br i1 %199, label %297, label %148, !dbg !68

200:                                              ; preds = %139
  br i1 %25, label %201, label %300, !dbg !80

201:                                              ; preds = %200
  %202 = zext nneg i32 %3 to i64, !dbg !81
  %203 = icmp ult i32 %3, 16, !dbg !80
  br i1 %203, label %204, label %206, !dbg !80

204:                                              ; preds = %258, %201
  %205 = phi i64 [ 0, %201 ], [ %207, %258 ]
  br label %268, !dbg !80

206:                                              ; preds = %201
  %207 = and i64 %202, 2147483632, !dbg !80
  %208 = insertelement <4 x float> poison, float %24, i64 0, !dbg !80
  %209 = shufflevector <4 x float> %208, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !80
  br label %210, !dbg !80

210:                                              ; preds = %210, %206
  %211 = phi i64 [ 0, %206 ], [ %256, %210 ], !dbg !82
  %212 = getelementptr inbounds i32, ptr %142, i64 %211, !dbg !83
  %213 = getelementptr inbounds i8, ptr %212, i64 16, !dbg !83
  %214 = getelementptr inbounds i8, ptr %212, i64 32, !dbg !83
  %215 = getelementptr inbounds i8, ptr %212, i64 48, !dbg !83
  %216 = load <4 x i32>, ptr %212, align 8, !dbg !83, !tbaa !23
  %217 = load <4 x i32>, ptr %213, align 8, !dbg !83, !tbaa !23
  %218 = load <4 x i32>, ptr %214, align 8, !dbg !83, !tbaa !23
  %219 = load <4 x i32>, ptr %215, align 8, !dbg !83, !tbaa !23
  %220 = sitofp <4 x i32> %216 to <4 x float>, !dbg !83
  %221 = sitofp <4 x i32> %217 to <4 x float>, !dbg !83
  %222 = sitofp <4 x i32> %218 to <4 x float>, !dbg !83
  %223 = sitofp <4 x i32> %219 to <4 x float>, !dbg !83
  %224 = getelementptr inbounds i32, ptr %140, i64 %211, !dbg !84
  %225 = getelementptr inbounds i8, ptr %224, i64 16, !dbg !84
  %226 = getelementptr inbounds i8, ptr %224, i64 32, !dbg !84
  %227 = getelementptr inbounds i8, ptr %224, i64 48, !dbg !84
  %228 = load <4 x i32>, ptr %224, align 8, !dbg !84, !tbaa !23
  %229 = load <4 x i32>, ptr %225, align 8, !dbg !84, !tbaa !23
  %230 = load <4 x i32>, ptr %226, align 8, !dbg !84, !tbaa !23
  %231 = load <4 x i32>, ptr %227, align 8, !dbg !84, !tbaa !23
  %232 = sitofp <4 x i32> %228 to <4 x float>, !dbg !84
  %233 = sitofp <4 x i32> %229 to <4 x float>, !dbg !84
  %234 = sitofp <4 x i32> %230 to <4 x float>, !dbg !84
  %235 = sitofp <4 x i32> %231 to <4 x float>, !dbg !84
  %236 = fmul fast <4 x float> %209, %220, !dbg !85
  %237 = fmul fast <4 x float> %209, %221, !dbg !85
  %238 = fmul fast <4 x float> %209, %222, !dbg !85
  %239 = fmul fast <4 x float> %209, %223, !dbg !85
  %240 = fsub fast <4 x float> %236, %220, !dbg !86
  %241 = fsub fast <4 x float> %237, %221, !dbg !86
  %242 = fsub fast <4 x float> %238, %222, !dbg !86
  %243 = fsub fast <4 x float> %239, %223, !dbg !86
  %244 = fmul fast <4 x float> %240, %232, !dbg !86
  %245 = fmul fast <4 x float> %241, %233, !dbg !86
  %246 = fmul fast <4 x float> %242, %234, !dbg !86
  %247 = fmul fast <4 x float> %243, %235, !dbg !86
  %248 = fadd fast <4 x float> %244, %220, !dbg !86
  %249 = fadd fast <4 x float> %245, %221, !dbg !86
  %250 = fadd fast <4 x float> %246, %222, !dbg !86
  %251 = fadd fast <4 x float> %247, %223, !dbg !86
  %252 = getelementptr inbounds float, ptr %143, i64 %211, !dbg !87
  %253 = getelementptr inbounds i8, ptr %252, i64 16, !dbg !88
  %254 = getelementptr inbounds i8, ptr %252, i64 32, !dbg !88
  %255 = getelementptr inbounds i8, ptr %252, i64 48, !dbg !88
  store <4 x float> %248, ptr %252, align 8, !dbg !88, !tbaa !28
  store <4 x float> %249, ptr %253, align 8, !dbg !88, !tbaa !28
  store <4 x float> %250, ptr %254, align 8, !dbg !88, !tbaa !28
  store <4 x float> %251, ptr %255, align 8, !dbg !88, !tbaa !28
  %256 = add nuw i64 %211, 16, !dbg !82
  %257 = icmp eq i64 %256, %207, !dbg !82
  br i1 %257, label %258, label %210, !dbg !82, !llvm.loop !89

258:                                              ; preds = %210
  %259 = icmp eq i64 %207, %202, !dbg !80
  br i1 %259, label %297, label %204, !dbg !80

260:                                              ; preds = %110, %260
  %261 = phi i64 [ %266, %260 ], [ %111, %110 ]
  %262 = trunc nuw nsw i64 %261 to i32, !dbg !58
  %263 = and i32 %262, 1, !dbg !58
  %264 = xor i32 %263, %109, !dbg !59
  %265 = getelementptr inbounds i32, ptr %108, i64 %261, !dbg !60
  store i32 %264, ptr %265, align 4, !dbg !61, !tbaa !23
  %266 = add nuw nsw i64 %261, 1, !dbg !57
  %267 = icmp eq i64 %266, %32, !dbg !91
  br i1 %267, label %139, label %260, !dbg !36, !llvm.loop !92

268:                                              ; preds = %204, %268
  %269 = phi i64 [ %281, %268 ], [ %205, %204 ]
  %270 = getelementptr inbounds i32, ptr %142, i64 %269, !dbg !83
  %271 = load i32, ptr %270, align 4, !dbg !83, !tbaa !23
  %272 = sitofp i32 %271 to float, !dbg !83
  %273 = getelementptr inbounds i32, ptr %140, i64 %269, !dbg !84
  %274 = load i32, ptr %273, align 4, !dbg !84, !tbaa !23
  %275 = sitofp i32 %274 to float, !dbg !84
  %276 = fmul fast float %24, %272, !dbg !85
  %277 = fsub fast float %276, %272, !dbg !86
  %278 = fmul fast float %277, %275, !dbg !86
  %279 = fadd fast float %278, %272, !dbg !86
  %280 = getelementptr inbounds float, ptr %143, i64 %269, !dbg !87
  store float %279, ptr %280, align 4, !dbg !88, !tbaa !28
  %281 = add nuw nsw i64 %269, 1, !dbg !82
  %282 = icmp eq i64 %281, %202, !dbg !81
  br i1 %282, label %297, label %268, !dbg !80, !llvm.loop !93

283:                                              ; preds = %148, %283
  %284 = phi i64 [ %295, %283 ], [ %149, %148 ]
  %285 = getelementptr inbounds i32, ptr %142, i64 %284, !dbg !71
  %286 = load i32, ptr %285, align 4, !dbg !71, !tbaa !23
  %287 = sitofp i32 %286 to float, !dbg !71
  %288 = fmul fast float %24, %287, !dbg !72
  %289 = getelementptr inbounds i32, ptr %140, i64 %284, !dbg !73
  %290 = load i32, ptr %289, align 4, !dbg !73, !tbaa !23
  %291 = sitofp i32 %290 to float, !dbg !73
  %292 = fsub fast float 1.000000e+00, %291, !dbg !74
  %293 = fmul fast float %288, %292, !dbg !75
  %294 = getelementptr inbounds float, ptr %143, i64 %284, !dbg !76
  store float %293, ptr %294, align 4, !dbg !77, !tbaa !28
  %295 = add nuw nsw i64 %284, 1, !dbg !70
  %296 = icmp eq i64 %295, %146, !dbg !69
  br i1 %296, label %297, label %283, !dbg !68, !llvm.loop !94

297:                                              ; preds = %283, %268, %198, %258
  br i1 %25, label %298, label %300, !dbg !95

298:                                              ; preds = %297
  %299 = zext nneg i32 %3 to i64, !dbg !96
  br label %305, !dbg !95

300:                                              ; preds = %305, %200, %144, %297
  %301 = add nsw i32 %3, 1, !dbg !97
  %302 = sext i32 %301 to i64, !dbg !98
  %303 = getelementptr inbounds i32, ptr %1, i64 %302, !dbg !98
  store i32 %7, ptr %303, align 4, !dbg !99, !tbaa !23
  %304 = getelementptr inbounds float, ptr %0, i64 %302, !dbg !100
  store float %9, ptr %304, align 4, !dbg !101, !tbaa !28
  ret void, !dbg !102

305:                                              ; preds = %298, %305
  %306 = phi float [ %8, %298 ], [ %310, %305 ], !dbg !103
  %307 = phi i64 [ 0, %298 ], [ %311, %305 ]
  %308 = getelementptr inbounds float, ptr %143, i64 %307, !dbg !104
  %309 = load float, ptr %308, align 4, !dbg !104, !tbaa !28
  %310 = fadd fast float %309, %306, !dbg !105
  %311 = add nuw nsw i64 %307, 1, !dbg !106
  %312 = getelementptr inbounds float, ptr %0, i64 %311, !dbg !107
  store float %310, ptr %312, align 4, !dbg !108, !tbaa !28
  %313 = icmp eq i64 %311, %299, !dbg !96
  br i1 %313, label %300, label %305, !dbg !95, !llvm.loop !109
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
!32 = !DILocation(line: 29, column: 44, scope: !9)
!33 = !DILocation(line: 29, column: 53, scope: !9)
!34 = !DILocation(line: 29, column: 27, scope: !9)
!35 = !DILocation(line: 35, column: 32, scope: !9)
!36 = !DILocation(line: 37, column: 5, scope: !9)
!37 = !DILocation(line: 25, column: 15, scope: !9)
!38 = !DILocation(line: 25, column: 22, scope: !9)
!39 = !DILocation(line: 25, column: 9, scope: !9)
!40 = !DILocation(line: 25, column: 20, scope: !9)
!41 = distinct !{!41, !31, !42, !43, !44, !45}
!42 = !DILocation(line: 26, column: 5, scope: !9)
!43 = !{!"llvm.loop.mustprogress"}
!44 = !{!"llvm.loop.isvectorized", i32 1}
!45 = !{!"llvm.loop.unroll.runtime.disable"}
!46 = !DILocation(line: 31, column: 36, scope: !9)
!47 = !DILocation(line: 30, column: 5, scope: !9)
!48 = !DILocation(line: 31, column: 29, scope: !9)
!49 = !DILocation(line: 31, column: 23, scope: !9)
!50 = !DILocation(line: 31, column: 34, scope: !9)
!51 = !DILocation(line: 31, column: 9, scope: !9)
!52 = !DILocation(line: 31, column: 21, scope: !9)
!53 = distinct !{!53, !47, !54, !43, !44, !45}
!54 = !DILocation(line: 32, column: 5, scope: !9)
!55 = distinct !{!55, !31, !42, !43, !44}
!56 = !DILocation(line: 36, column: 22, scope: !9)
!57 = !DILocation(line: 37, column: 36, scope: !9)
!58 = !DILocation(line: 38, column: 44, scope: !9)
!59 = !DILocation(line: 38, column: 39, scope: !9)
!60 = !DILocation(line: 38, column: 9, scope: !9)
!61 = !DILocation(line: 38, column: 26, scope: !9)
!62 = distinct !{!62, !36, !63, !43, !44, !45}
!63 = !DILocation(line: 39, column: 5, scope: !9)
!64 = !DILocation(line: 30, column: 23, scope: !9)
!65 = distinct !{!65, !47, !54, !43, !45, !44}
!66 = !DILocation(line: 42, column: 36, scope: !9)
!67 = !DILocation(line: 43, column: 9, scope: !9)
!68 = !DILocation(line: 52, column: 9, scope: !9)
!69 = !DILocation(line: 52, column: 27, scope: !9)
!70 = !DILocation(line: 52, column: 40, scope: !9)
!71 = !DILocation(line: 53, column: 47, scope: !9)
!72 = !DILocation(line: 53, column: 38, scope: !9)
!73 = !DILocation(line: 53, column: 69, scope: !9)
!74 = !DILocation(line: 53, column: 67, scope: !9)
!75 = !DILocation(line: 53, column: 59, scope: !9)
!76 = !DILocation(line: 53, column: 13, scope: !9)
!77 = !DILocation(line: 53, column: 30, scope: !9)
!78 = distinct !{!78, !68, !79, !43, !44, !45}
!79 = !DILocation(line: 54, column: 9, scope: !9)
!80 = !DILocation(line: 45, column: 9, scope: !9)
!81 = !DILocation(line: 45, column: 27, scope: !9)
!82 = !DILocation(line: 45, column: 40, scope: !9)
!83 = !DILocation(line: 46, column: 39, scope: !9)
!84 = !DILocation(line: 46, column: 61, scope: !9)
!85 = !DILocation(line: 47, column: 41, scope: !9)
!86 = !DILocation(line: 48, column: 43, scope: !9)
!87 = !DILocation(line: 48, column: 13, scope: !9)
!88 = !DILocation(line: 48, column: 30, scope: !9)
!89 = distinct !{!89, !80, !90, !43, !44, !45}
!90 = !DILocation(line: 49, column: 9, scope: !9)
!91 = !DILocation(line: 37, column: 23, scope: !9)
!92 = distinct !{!92, !36, !63, !43, !45, !44}
!93 = distinct !{!93, !80, !90, !43, !45, !44}
!94 = distinct !{!94, !68, !79, !43, !45, !44}
!95 = !DILocation(line: 59, column: 5, scope: !9)
!96 = !DILocation(line: 59, column: 23, scope: !9)
!97 = !DILocation(line: 63, column: 18, scope: !9)
!98 = !DILocation(line: 63, column: 5, scope: !9)
!99 = !DILocation(line: 63, column: 23, scope: !9)
!100 = !DILocation(line: 64, column: 5, scope: !9)
!101 = !DILocation(line: 64, column: 23, scope: !9)
!102 = !DILocation(line: 65, column: 1, scope: !9)
!103 = !DILocation(line: 60, column: 22, scope: !9)
!104 = !DILocation(line: 60, column: 31, scope: !9)
!105 = !DILocation(line: 60, column: 29, scope: !9)
!106 = !DILocation(line: 60, column: 15, scope: !9)
!107 = !DILocation(line: 60, column: 9, scope: !9)
!108 = !DILocation(line: 60, column: 20, scope: !9)
!109 = distinct !{!109, !95, !110, !43}
!110 = !DILocation(line: 61, column: 5, scope: !9)
