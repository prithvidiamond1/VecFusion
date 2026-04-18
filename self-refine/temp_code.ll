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
  br i1 %25, label %33, label %26, !dbg !31

26:                                               ; preds = %23
  %27 = sext i32 %3 to i64, !dbg !32
  %28 = shl nsw i64 %27, 2, !dbg !32
  %29 = alloca i8, i64 %28, align 8, !dbg !32
  %30 = getelementptr inbounds nuw i8, ptr %1, i64 4, !dbg !33
  %31 = load i32, ptr %30, align 4, !dbg !33, !tbaa !23
  %32 = sub nsw i32 %31, %6, !dbg !34
  store i32 %32, ptr %29, align 8, !dbg !35, !tbaa !23
  br label %137, !dbg !36

33:                                               ; preds = %23
  %34 = zext nneg i32 %3 to i64, !dbg !30
  %35 = icmp ult i32 %3, 4, !dbg !31
  br i1 %35, label %36, label %38, !dbg !31

36:                                               ; preds = %65, %79, %38, %33
  %37 = phi i64 [ %45, %65 ], [ 0, %33 ], [ 0, %38 ], [ %70, %79 ]
  br label %130, !dbg !31

38:                                               ; preds = %33
  %39 = add i64 %13, 4, !dbg !31
  %40 = sub i64 %39, %12, !dbg !31
  %41 = icmp ult i64 %40, 64, !dbg !31
  br i1 %41, label %36, label %42, !dbg !31

42:                                               ; preds = %38
  %43 = icmp ult i32 %3, 16, !dbg !31
  br i1 %43, label %68, label %44, !dbg !31

44:                                               ; preds = %42
  %45 = and i64 %34, 2147483632, !dbg !31
  br label %46, !dbg !31

46:                                               ; preds = %46, %44
  %47 = phi i64 [ 0, %44 ], [ %61, %46 ], !dbg !37
  %48 = getelementptr inbounds nuw i32, ptr %2, i64 %47, !dbg !38
  %49 = getelementptr inbounds nuw i8, ptr %48, i64 16, !dbg !38
  %50 = getelementptr inbounds nuw i8, ptr %48, i64 32, !dbg !38
  %51 = getelementptr inbounds nuw i8, ptr %48, i64 48, !dbg !38
  %52 = load <4 x i32>, ptr %48, align 4, !dbg !38, !tbaa !23
  %53 = load <4 x i32>, ptr %49, align 4, !dbg !38, !tbaa !23
  %54 = load <4 x i32>, ptr %50, align 4, !dbg !38, !tbaa !23
  %55 = load <4 x i32>, ptr %51, align 4, !dbg !38, !tbaa !23
  %56 = or disjoint i64 %47, 1, !dbg !37
  %57 = getelementptr inbounds nuw i32, ptr %1, i64 %56, !dbg !39
  %58 = getelementptr inbounds nuw i8, ptr %57, i64 16, !dbg !40
  %59 = getelementptr inbounds nuw i8, ptr %57, i64 32, !dbg !40
  %60 = getelementptr inbounds nuw i8, ptr %57, i64 48, !dbg !40
  store <4 x i32> %52, ptr %57, align 4, !dbg !40, !tbaa !23
  store <4 x i32> %53, ptr %58, align 4, !dbg !40, !tbaa !23
  store <4 x i32> %54, ptr %59, align 4, !dbg !40, !tbaa !23
  store <4 x i32> %55, ptr %60, align 4, !dbg !40, !tbaa !23
  %61 = add nuw i64 %47, 16, !dbg !37
  %62 = icmp eq i64 %61, %45, !dbg !37
  br i1 %62, label %63, label %46, !dbg !37, !llvm.loop !41

63:                                               ; preds = %46
  %64 = icmp eq i64 %45, %34, !dbg !31
  br i1 %64, label %81, label %65, !dbg !31

65:                                               ; preds = %63
  %66 = and i64 %34, 12, !dbg !31
  %67 = icmp eq i64 %66, 0, !dbg !31
  br i1 %67, label %36, label %68, !dbg !31

68:                                               ; preds = %65, %42
  %69 = phi i64 [ %45, %65 ], [ 0, %42 ]
  %70 = and i64 %34, 2147483644, !dbg !31
  br label %71, !dbg !31

71:                                               ; preds = %71, %68
  %72 = phi i64 [ %69, %68 ], [ %77, %71 ], !dbg !37
  %73 = getelementptr inbounds nuw i32, ptr %2, i64 %72, !dbg !38
  %74 = load <4 x i32>, ptr %73, align 4, !dbg !38, !tbaa !23
  %75 = or disjoint i64 %72, 1, !dbg !37
  %76 = getelementptr inbounds nuw i32, ptr %1, i64 %75, !dbg !39
  store <4 x i32> %74, ptr %76, align 4, !dbg !40, !tbaa !23
  %77 = add nuw i64 %72, 4, !dbg !37
  %78 = icmp eq i64 %77, %70, !dbg !37
  br i1 %78, label %79, label %71, !dbg !37, !llvm.loop !46

79:                                               ; preds = %71
  %80 = icmp eq i64 %70, %34, !dbg !31
  br i1 %80, label %81, label %36, !dbg !31

81:                                               ; preds = %130, %79, %63
  %82 = zext nneg i32 %3 to i64, !dbg !32
  %83 = shl nuw nsw i64 %82, 2, !dbg !32
  %84 = alloca i8, i64 %83, align 8, !dbg !32
  %85 = getelementptr inbounds nuw i8, ptr %1, i64 4, !dbg !33
  %86 = load i32, ptr %85, align 4, !dbg !33, !tbaa !23
  %87 = sub nsw i32 %86, %6, !dbg !34
  store i32 %87, ptr %84, align 8, !dbg !35, !tbaa !23
  %88 = icmp eq i32 %3, 1, !dbg !47
  br i1 %88, label %137, label %89, !dbg !36

89:                                               ; preds = %81
  %90 = zext nneg i32 %3 to i64, !dbg !47
  %91 = add nsw i64 %34, -1, !dbg !36
  %92 = icmp ult i32 %3, 17, !dbg !36
  br i1 %92, label %127, label %93, !dbg !36

93:                                               ; preds = %89
  %94 = and i64 %91, -16, !dbg !36
  %95 = or disjoint i64 %94, 1, !dbg !36
  %96 = insertelement <4 x i32> poison, i32 %86, i64 3, !dbg !36
  br label %97, !dbg !36

97:                                               ; preds = %97, %93
  %98 = phi i64 [ 0, %93 ], [ %122, %97 ]
  %99 = phi <4 x i32> [ %96, %93 ], [ %109, %97 ]
  %100 = or disjoint i64 %98, 1, !dbg !36
  %101 = or disjoint i64 %98, 2, !dbg !48
  %102 = getelementptr inbounds nuw i32, ptr %1, i64 %101, !dbg !49
  %103 = getelementptr inbounds nuw i8, ptr %102, i64 16, !dbg !49
  %104 = getelementptr inbounds nuw i8, ptr %102, i64 32, !dbg !49
  %105 = getelementptr inbounds nuw i8, ptr %102, i64 48, !dbg !49
  %106 = load <4 x i32>, ptr %102, align 4, !dbg !49, !tbaa !23
  %107 = load <4 x i32>, ptr %103, align 4, !dbg !49, !tbaa !23
  %108 = load <4 x i32>, ptr %104, align 4, !dbg !49, !tbaa !23
  %109 = load <4 x i32>, ptr %105, align 4, !dbg !49, !tbaa !23
  %110 = shufflevector <4 x i32> %99, <4 x i32> %106, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %111 = shufflevector <4 x i32> %106, <4 x i32> %107, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %112 = shufflevector <4 x i32> %107, <4 x i32> %108, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %113 = shufflevector <4 x i32> %108, <4 x i32> %109, <4 x i32> <i32 3, i32 4, i32 5, i32 6>
  %114 = sub nsw <4 x i32> %106, %110, !dbg !50
  %115 = sub nsw <4 x i32> %107, %111, !dbg !50
  %116 = sub nsw <4 x i32> %108, %112, !dbg !50
  %117 = sub nsw <4 x i32> %109, %113, !dbg !50
  %118 = getelementptr inbounds nuw i32, ptr %84, i64 %100, !dbg !51
  %119 = getelementptr inbounds nuw i8, ptr %118, i64 16, !dbg !52
  %120 = getelementptr inbounds nuw i8, ptr %118, i64 32, !dbg !52
  %121 = getelementptr inbounds nuw i8, ptr %118, i64 48, !dbg !52
  store <4 x i32> %114, ptr %118, align 4, !dbg !52, !tbaa !23
  store <4 x i32> %115, ptr %119, align 4, !dbg !52, !tbaa !23
  store <4 x i32> %116, ptr %120, align 4, !dbg !52, !tbaa !23
  store <4 x i32> %117, ptr %121, align 4, !dbg !52, !tbaa !23
  %122 = add nuw i64 %98, 16
  %123 = icmp eq i64 %122, %94
  br i1 %123, label %124, label %97, !llvm.loop !53

124:                                              ; preds = %97
  %125 = extractelement <4 x i32> %109, i64 3
  %126 = icmp eq i64 %91, %94, !dbg !36
  br i1 %126, label %137, label %127, !dbg !36

127:                                              ; preds = %89, %124
  %128 = phi i32 [ %86, %89 ], [ %125, %124 ]
  %129 = phi i64 [ 1, %89 ], [ %95, %124 ]
  br label %141, !dbg !36

130:                                              ; preds = %36, %130
  %131 = phi i64 [ %134, %130 ], [ %37, %36 ]
  %132 = getelementptr inbounds nuw i32, ptr %2, i64 %131, !dbg !38
  %133 = load i32, ptr %132, align 4, !dbg !38, !tbaa !23
  %134 = add nuw nsw i64 %131, 1, !dbg !37
  %135 = getelementptr inbounds nuw i32, ptr %1, i64 %134, !dbg !39
  store i32 %133, ptr %135, align 4, !dbg !40, !tbaa !23
  %136 = icmp eq i64 %134, %34, !dbg !30
  br i1 %136, label %81, label %130, !dbg !31, !llvm.loop !55

137:                                              ; preds = %141, %124, %26, %81
  %138 = phi ptr [ %29, %26 ], [ %84, %81 ], [ %84, %124 ], [ %84, %141 ]
  %139 = phi i64 [ %28, %26 ], [ %83, %81 ], [ %83, %124 ], [ %83, %141 ]
  %140 = alloca i8, i64 %139, align 8, !dbg !56
  br i1 %16, label %150, label %333, !dbg !57

141:                                              ; preds = %127, %141
  %142 = phi i32 [ %146, %141 ], [ %128, %127 ], !dbg !58
  %143 = phi i64 [ %144, %141 ], [ %129, %127 ]
  %144 = add nuw nsw i64 %143, 1, !dbg !48
  %145 = getelementptr inbounds nuw i32, ptr %1, i64 %144, !dbg !49
  %146 = load i32, ptr %145, align 4, !dbg !49, !tbaa !23
  %147 = sub nsw i32 %146, %142, !dbg !50
  %148 = getelementptr inbounds nuw i32, ptr %84, i64 %143, !dbg !51
  store i32 %147, ptr %148, align 4, !dbg !52, !tbaa !23
  %149 = icmp eq i64 %144, %90, !dbg !47
  br i1 %149, label %137, label %141, !dbg !36, !llvm.loop !59

150:                                              ; preds = %137
  br i1 %10, label %230, label %151, !dbg !60

151:                                              ; preds = %150
  br i1 %25, label %152, label %371, !dbg !61

152:                                              ; preds = %151
  %153 = zext nneg i32 %3 to i64, !dbg !62
  %154 = icmp ult i32 %3, 4, !dbg !61
  br i1 %154, label %155, label %157, !dbg !61

155:                                              ; preds = %203, %228, %152
  %156 = phi i64 [ %160, %203 ], [ 0, %152 ], [ %208, %228 ]
  br label %321, !dbg !61

157:                                              ; preds = %152
  %158 = icmp ult i32 %3, 16, !dbg !61
  br i1 %158, label %206, label %159, !dbg !61

159:                                              ; preds = %157
  %160 = and i64 %153, 2147483632, !dbg !61
  %161 = insertelement <4 x float> poison, float %24, i64 0, !dbg !61
  %162 = shufflevector <4 x float> %161, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !61
  br label %163, !dbg !61

163:                                              ; preds = %163, %159
  %164 = phi i64 [ 0, %159 ], [ %198, %163 ], !dbg !63
  %165 = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, %159 ], [ %199, %163 ]
  %166 = and <4 x i64> %165, splat (i64 1), !dbg !64
  %167 = and <4 x i64> %165, splat (i64 1), !dbg !64
  %168 = and <4 x i64> %165, splat (i64 1), !dbg !64
  %169 = and <4 x i64> %165, splat (i64 1), !dbg !64
  %170 = icmp eq <4 x i64> %166, zeroinitializer, !dbg !64
  %171 = icmp eq <4 x i64> %167, zeroinitializer, !dbg !64
  %172 = icmp eq <4 x i64> %168, zeroinitializer, !dbg !64
  %173 = icmp eq <4 x i64> %169, zeroinitializer, !dbg !64
  %174 = getelementptr inbounds nuw i32, ptr %138, i64 %164, !dbg !65
  %175 = getelementptr inbounds nuw i8, ptr %174, i64 16, !dbg !65
  %176 = getelementptr inbounds nuw i8, ptr %174, i64 32, !dbg !65
  %177 = getelementptr inbounds nuw i8, ptr %174, i64 48, !dbg !65
  %178 = load <4 x i32>, ptr %174, align 8, !dbg !65, !tbaa !23
  %179 = load <4 x i32>, ptr %175, align 8, !dbg !65, !tbaa !23
  %180 = load <4 x i32>, ptr %176, align 8, !dbg !65, !tbaa !23
  %181 = load <4 x i32>, ptr %177, align 8, !dbg !65, !tbaa !23
  %182 = sitofp <4 x i32> %178 to <4 x float>, !dbg !65
  %183 = sitofp <4 x i32> %179 to <4 x float>, !dbg !65
  %184 = sitofp <4 x i32> %180 to <4 x float>, !dbg !65
  %185 = sitofp <4 x i32> %181 to <4 x float>, !dbg !65
  %186 = select fast <4 x i1> %170, <4 x float> splat (float 1.000000e+00), <4 x float> %162, !dbg !66
  %187 = select fast <4 x i1> %171, <4 x float> splat (float 1.000000e+00), <4 x float> %162, !dbg !66
  %188 = select fast <4 x i1> %172, <4 x float> splat (float 1.000000e+00), <4 x float> %162, !dbg !66
  %189 = select fast <4 x i1> %173, <4 x float> splat (float 1.000000e+00), <4 x float> %162, !dbg !66
  %190 = fmul fast <4 x float> %186, %182, !dbg !66
  %191 = fmul fast <4 x float> %187, %183, !dbg !66
  %192 = fmul fast <4 x float> %188, %184, !dbg !66
  %193 = fmul fast <4 x float> %189, %185, !dbg !66
  %194 = getelementptr inbounds nuw float, ptr %140, i64 %164, !dbg !67
  %195 = getelementptr inbounds nuw i8, ptr %194, i64 16, !dbg !68
  %196 = getelementptr inbounds nuw i8, ptr %194, i64 32, !dbg !68
  %197 = getelementptr inbounds nuw i8, ptr %194, i64 48, !dbg !68
  store <4 x float> %190, ptr %194, align 8, !dbg !68, !tbaa !28
  store <4 x float> %191, ptr %195, align 8, !dbg !68, !tbaa !28
  store <4 x float> %192, ptr %196, align 8, !dbg !68, !tbaa !28
  store <4 x float> %193, ptr %197, align 8, !dbg !68, !tbaa !28
  %198 = add nuw i64 %164, 16, !dbg !63
  %199 = add <4 x i64> %165, splat (i64 16)
  %200 = icmp eq i64 %198, %160, !dbg !63
  br i1 %200, label %201, label %163, !dbg !63, !llvm.loop !69

201:                                              ; preds = %163
  %202 = icmp eq i64 %160, %153, !dbg !61
  br i1 %202, label %368, label %203, !dbg !61

203:                                              ; preds = %201
  %204 = and i64 %153, 12, !dbg !61
  %205 = icmp eq i64 %204, 0, !dbg !61
  br i1 %205, label %155, label %206, !dbg !61

206:                                              ; preds = %203, %157
  %207 = phi i64 [ %160, %203 ], [ 0, %157 ]
  %208 = and i64 %153, 2147483644, !dbg !61
  %209 = insertelement <4 x i64> poison, i64 %207, i64 0, !dbg !61
  %210 = shufflevector <4 x i64> %209, <4 x i64> poison, <4 x i32> zeroinitializer, !dbg !61
  %211 = or disjoint <4 x i64> %210, <i64 0, i64 1, i64 2, i64 3>, !dbg !61
  %212 = insertelement <4 x float> poison, float %24, i64 0, !dbg !61
  %213 = shufflevector <4 x float> %212, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !61
  br label %214, !dbg !61

214:                                              ; preds = %214, %206
  %215 = phi i64 [ %207, %206 ], [ %225, %214 ], !dbg !63
  %216 = phi <4 x i64> [ %211, %206 ], [ %226, %214 ]
  %217 = and <4 x i64> %216, splat (i64 1), !dbg !64
  %218 = icmp eq <4 x i64> %217, zeroinitializer, !dbg !64
  %219 = getelementptr inbounds nuw i32, ptr %138, i64 %215, !dbg !65
  %220 = load <4 x i32>, ptr %219, align 8, !dbg !65, !tbaa !23
  %221 = sitofp <4 x i32> %220 to <4 x float>, !dbg !65
  %222 = select fast <4 x i1> %218, <4 x float> splat (float 1.000000e+00), <4 x float> %213, !dbg !66
  %223 = fmul fast <4 x float> %222, %221, !dbg !66
  %224 = getelementptr inbounds nuw float, ptr %140, i64 %215, !dbg !67
  store <4 x float> %223, ptr %224, align 8, !dbg !68, !tbaa !28
  %225 = add nuw i64 %215, 4, !dbg !63
  %226 = add <4 x i64> %216, splat (i64 4)
  %227 = icmp eq i64 %225, %208, !dbg !63
  br i1 %227, label %228, label %214, !dbg !63, !llvm.loop !71

228:                                              ; preds = %214
  %229 = icmp eq i64 %208, %153, !dbg !61
  br i1 %229, label %368, label %155, !dbg !61

230:                                              ; preds = %150
  br i1 %25, label %231, label %371, !dbg !72

231:                                              ; preds = %230
  %232 = zext nneg i32 %3 to i64, !dbg !73
  %233 = icmp ult i32 %3, 4, !dbg !72
  br i1 %233, label %307, label %234, !dbg !72

234:                                              ; preds = %231
  %235 = icmp ult i32 %3, 16, !dbg !72
  br i1 %235, label %283, label %236, !dbg !72

236:                                              ; preds = %234
  %237 = and i64 %232, 2147483632, !dbg !72
  %238 = insertelement <4 x float> poison, float %24, i64 0, !dbg !72
  %239 = shufflevector <4 x float> %238, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !72
  br label %240, !dbg !72

240:                                              ; preds = %240, %236
  %241 = phi i64 [ 0, %236 ], [ %275, %240 ], !dbg !74
  %242 = phi <4 x i64> [ <i64 0, i64 1, i64 2, i64 3>, %236 ], [ %276, %240 ]
  %243 = and <4 x i64> %242, splat (i64 1), !dbg !75
  %244 = and <4 x i64> %242, splat (i64 1), !dbg !75
  %245 = and <4 x i64> %242, splat (i64 1), !dbg !75
  %246 = and <4 x i64> %242, splat (i64 1), !dbg !75
  %247 = icmp eq <4 x i64> %243, zeroinitializer, !dbg !75
  %248 = icmp eq <4 x i64> %244, zeroinitializer, !dbg !75
  %249 = icmp eq <4 x i64> %245, zeroinitializer, !dbg !75
  %250 = icmp eq <4 x i64> %246, zeroinitializer, !dbg !75
  %251 = getelementptr inbounds nuw i32, ptr %138, i64 %241, !dbg !76
  %252 = getelementptr inbounds nuw i8, ptr %251, i64 16, !dbg !76
  %253 = getelementptr inbounds nuw i8, ptr %251, i64 32, !dbg !76
  %254 = getelementptr inbounds nuw i8, ptr %251, i64 48, !dbg !76
  %255 = load <4 x i32>, ptr %251, align 8, !dbg !76, !tbaa !23
  %256 = load <4 x i32>, ptr %252, align 8, !dbg !76, !tbaa !23
  %257 = load <4 x i32>, ptr %253, align 8, !dbg !76, !tbaa !23
  %258 = load <4 x i32>, ptr %254, align 8, !dbg !76, !tbaa !23
  %259 = sitofp <4 x i32> %255 to <4 x float>, !dbg !76
  %260 = sitofp <4 x i32> %256 to <4 x float>, !dbg !76
  %261 = sitofp <4 x i32> %257 to <4 x float>, !dbg !76
  %262 = sitofp <4 x i32> %258 to <4 x float>, !dbg !76
  %263 = select fast <4 x i1> %247, <4 x float> %239, <4 x float> splat (float 1.000000e+00), !dbg !77
  %264 = select fast <4 x i1> %248, <4 x float> %239, <4 x float> splat (float 1.000000e+00), !dbg !77
  %265 = select fast <4 x i1> %249, <4 x float> %239, <4 x float> splat (float 1.000000e+00), !dbg !77
  %266 = select fast <4 x i1> %250, <4 x float> %239, <4 x float> splat (float 1.000000e+00), !dbg !77
  %267 = fmul fast <4 x float> %263, %259, !dbg !77
  %268 = fmul fast <4 x float> %264, %260, !dbg !77
  %269 = fmul fast <4 x float> %265, %261, !dbg !77
  %270 = fmul fast <4 x float> %266, %262, !dbg !77
  %271 = getelementptr inbounds nuw float, ptr %140, i64 %241, !dbg !78
  %272 = getelementptr inbounds nuw i8, ptr %271, i64 16, !dbg !79
  %273 = getelementptr inbounds nuw i8, ptr %271, i64 32, !dbg !79
  %274 = getelementptr inbounds nuw i8, ptr %271, i64 48, !dbg !79
  store <4 x float> %267, ptr %271, align 8, !dbg !79, !tbaa !28
  store <4 x float> %268, ptr %272, align 8, !dbg !79, !tbaa !28
  store <4 x float> %269, ptr %273, align 8, !dbg !79, !tbaa !28
  store <4 x float> %270, ptr %274, align 8, !dbg !79, !tbaa !28
  %275 = add nuw i64 %241, 16, !dbg !74
  %276 = add <4 x i64> %242, splat (i64 16)
  %277 = icmp eq i64 %275, %237, !dbg !74
  br i1 %277, label %278, label %240, !dbg !74, !llvm.loop !80

278:                                              ; preds = %240
  %279 = icmp eq i64 %237, %232, !dbg !72
  br i1 %279, label %368, label %280, !dbg !72

280:                                              ; preds = %278
  %281 = and i64 %232, 12, !dbg !72
  %282 = icmp eq i64 %281, 0, !dbg !72
  br i1 %282, label %307, label %283, !dbg !72

283:                                              ; preds = %280, %234
  %284 = phi i64 [ %237, %280 ], [ 0, %234 ]
  %285 = and i64 %232, 2147483644, !dbg !72
  %286 = insertelement <4 x i64> poison, i64 %284, i64 0, !dbg !72
  %287 = shufflevector <4 x i64> %286, <4 x i64> poison, <4 x i32> zeroinitializer, !dbg !72
  %288 = or disjoint <4 x i64> %287, <i64 0, i64 1, i64 2, i64 3>, !dbg !72
  %289 = insertelement <4 x float> poison, float %24, i64 0, !dbg !72
  %290 = shufflevector <4 x float> %289, <4 x float> poison, <4 x i32> zeroinitializer, !dbg !72
  br label %291, !dbg !72

291:                                              ; preds = %291, %283
  %292 = phi i64 [ %284, %283 ], [ %302, %291 ], !dbg !74
  %293 = phi <4 x i64> [ %288, %283 ], [ %303, %291 ]
  %294 = and <4 x i64> %293, splat (i64 1), !dbg !75
  %295 = icmp eq <4 x i64> %294, zeroinitializer, !dbg !75
  %296 = getelementptr inbounds nuw i32, ptr %138, i64 %292, !dbg !76
  %297 = load <4 x i32>, ptr %296, align 8, !dbg !76, !tbaa !23
  %298 = sitofp <4 x i32> %297 to <4 x float>, !dbg !76
  %299 = select fast <4 x i1> %295, <4 x float> %290, <4 x float> splat (float 1.000000e+00), !dbg !77
  %300 = fmul fast <4 x float> %299, %298, !dbg !77
  %301 = getelementptr inbounds nuw float, ptr %140, i64 %292, !dbg !78
  store <4 x float> %300, ptr %301, align 8, !dbg !79, !tbaa !28
  %302 = add nuw i64 %292, 4, !dbg !74
  %303 = add <4 x i64> %293, splat (i64 4)
  %304 = icmp eq i64 %302, %285, !dbg !74
  br i1 %304, label %305, label %291, !dbg !74, !llvm.loop !82

305:                                              ; preds = %291
  %306 = icmp eq i64 %285, %232, !dbg !72
  br i1 %306, label %368, label %307, !dbg !72

307:                                              ; preds = %280, %305, %231
  %308 = phi i64 [ %237, %280 ], [ 0, %231 ], [ %285, %305 ]
  br label %309, !dbg !72

309:                                              ; preds = %307, %309
  %310 = phi i64 [ %319, %309 ], [ %308, %307 ]
  %311 = and i64 %310, 1, !dbg !75
  %312 = icmp eq i64 %311, 0, !dbg !75
  %313 = getelementptr inbounds nuw i32, ptr %138, i64 %310, !dbg !76
  %314 = load i32, ptr %313, align 4, !dbg !76, !tbaa !23
  %315 = sitofp i32 %314 to float, !dbg !76
  %316 = select fast i1 %312, float %24, float 1.000000e+00, !dbg !77
  %317 = fmul fast float %316, %315, !dbg !77
  %318 = getelementptr inbounds nuw float, ptr %140, i64 %310, !dbg !78
  store float %317, ptr %318, align 4, !dbg !79, !tbaa !28
  %319 = add nuw nsw i64 %310, 1, !dbg !74
  %320 = icmp eq i64 %319, %232, !dbg !73
  br i1 %320, label %368, label %309, !dbg !72, !llvm.loop !83

321:                                              ; preds = %155, %321
  %322 = phi i64 [ %331, %321 ], [ %156, %155 ]
  %323 = and i64 %322, 1, !dbg !64
  %324 = icmp eq i64 %323, 0, !dbg !64
  %325 = getelementptr inbounds nuw i32, ptr %138, i64 %322, !dbg !65
  %326 = load i32, ptr %325, align 4, !dbg !65, !tbaa !23
  %327 = sitofp i32 %326 to float, !dbg !65
  %328 = select fast i1 %324, float 1.000000e+00, float %24, !dbg !66
  %329 = fmul fast float %328, %327, !dbg !66
  %330 = getelementptr inbounds nuw float, ptr %140, i64 %322, !dbg !67
  store float %329, ptr %330, align 4, !dbg !68, !tbaa !28
  %331 = add nuw nsw i64 %322, 1, !dbg !63
  %332 = icmp eq i64 %331, %153, !dbg !62
  br i1 %332, label %368, label %321, !dbg !61, !llvm.loop !84

333:                                              ; preds = %137
  br i1 %10, label %337, label %334, !dbg !85

334:                                              ; preds = %333
  br i1 %25, label %335, label %371, !dbg !86

335:                                              ; preds = %334
  %336 = zext nneg i32 %3 to i64, !dbg !87
  br label %354, !dbg !86

337:                                              ; preds = %333
  br i1 %25, label %338, label %371, !dbg !88

338:                                              ; preds = %337
  %339 = zext nneg i32 %3 to i64, !dbg !89
  br label %340, !dbg !88

340:                                              ; preds = %338, %349
  %341 = phi i64 [ 0, %338 ], [ %352, %349 ]
  %342 = and i64 %341, 1, !dbg !90
  %343 = icmp eq i64 %342, 0, !dbg !90
  br i1 %343, label %349, label %344, !dbg !91

344:                                              ; preds = %340
  %345 = getelementptr inbounds nuw i32, ptr %138, i64 %341, !dbg !92
  %346 = load i32, ptr %345, align 4, !dbg !92, !tbaa !23
  %347 = sitofp i32 %346 to float, !dbg !92
  %348 = fmul fast float %24, %347, !dbg !93
  br label %349, !dbg !91

349:                                              ; preds = %340, %344
  %350 = phi fast float [ %348, %344 ], [ 0.000000e+00, %340 ], !dbg !91
  %351 = getelementptr inbounds nuw float, ptr %140, i64 %341, !dbg !94
  store float %350, ptr %351, align 4, !dbg !95, !tbaa !28
  %352 = add nuw nsw i64 %341, 1, !dbg !96
  %353 = icmp eq i64 %352, %339, !dbg !89
  br i1 %353, label %368, label %340, !dbg !88, !llvm.loop !97

354:                                              ; preds = %335, %363
  %355 = phi i64 [ 0, %335 ], [ %366, %363 ]
  %356 = and i64 %355, 1, !dbg !99
  %357 = icmp eq i64 %356, 0, !dbg !99
  br i1 %357, label %358, label %363, !dbg !100

358:                                              ; preds = %354
  %359 = getelementptr inbounds nuw i32, ptr %138, i64 %355, !dbg !101
  %360 = load i32, ptr %359, align 4, !dbg !101, !tbaa !23
  %361 = sitofp i32 %360 to float, !dbg !101
  %362 = fmul fast float %24, %361, !dbg !102
  br label %363, !dbg !100

363:                                              ; preds = %354, %358
  %364 = phi fast float [ %362, %358 ], [ 0.000000e+00, %354 ], !dbg !100
  %365 = getelementptr inbounds nuw float, ptr %140, i64 %355, !dbg !103
  store float %364, ptr %365, align 4, !dbg !104, !tbaa !28
  %366 = add nuw nsw i64 %355, 1, !dbg !105
  %367 = icmp eq i64 %366, %336, !dbg !87
  br i1 %367, label %368, label %354, !dbg !86, !llvm.loop !106

368:                                              ; preds = %363, %349, %321, %309, %201, %228, %278, %305
  br i1 %25, label %369, label %371, !dbg !108

369:                                              ; preds = %368
  %370 = zext nneg i32 %3 to i64, !dbg !109
  br label %376, !dbg !108

371:                                              ; preds = %376, %230, %151, %337, %334, %368
  %372 = add nsw i32 %3, 1, !dbg !110
  %373 = sext i32 %372 to i64, !dbg !111
  %374 = getelementptr inbounds i32, ptr %1, i64 %373, !dbg !111
  store i32 %7, ptr %374, align 4, !dbg !112, !tbaa !23
  %375 = getelementptr inbounds float, ptr %0, i64 %373, !dbg !113
  store float %9, ptr %375, align 4, !dbg !114, !tbaa !28
  ret void, !dbg !115

376:                                              ; preds = %369, %376
  %377 = phi float [ %8, %369 ], [ %381, %376 ], !dbg !116
  %378 = phi i64 [ 0, %369 ], [ %382, %376 ]
  %379 = getelementptr inbounds nuw float, ptr %140, i64 %378, !dbg !117
  %380 = load float, ptr %379, align 4, !dbg !117, !tbaa !28
  %381 = fadd fast float %380, %377, !dbg !118
  %382 = add nuw nsw i64 %378, 1, !dbg !119
  %383 = getelementptr inbounds nuw float, ptr %0, i64 %382, !dbg !120
  store float %381, ptr %383, align 4, !dbg !121, !tbaa !28
  %384 = icmp eq i64 %382, %370, !dbg !109
  br i1 %384, label %371, label %376, !dbg !108, !llvm.loop !122
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
!13 = !DILocation(line: 12, column: 23, scope: !9)
!14 = !DILocation(line: 12, column: 32, scope: !9)
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
!32 = !DILocation(line: 29, column: 28, scope: !9)
!33 = !DILocation(line: 30, column: 20, scope: !9)
!34 = !DILocation(line: 30, column: 27, scope: !9)
!35 = !DILocation(line: 30, column: 18, scope: !9)
!36 = !DILocation(line: 31, column: 5, scope: !9)
!37 = !DILocation(line: 25, column: 15, scope: !9)
!38 = !DILocation(line: 25, column: 22, scope: !9)
!39 = !DILocation(line: 25, column: 9, scope: !9)
!40 = !DILocation(line: 25, column: 20, scope: !9)
!41 = distinct !{!41, !31, !42, !43, !44, !45}
!42 = !DILocation(line: 26, column: 5, scope: !9)
!43 = !{!"llvm.loop.mustprogress"}
!44 = !{!"llvm.loop.isvectorized", i32 1}
!45 = !{!"llvm.loop.unroll.runtime.disable"}
!46 = distinct !{!46, !31, !42, !43, !44, !45}
!47 = !DILocation(line: 31, column: 23, scope: !9)
!48 = !DILocation(line: 32, column: 30, scope: !9)
!49 = !DILocation(line: 32, column: 24, scope: !9)
!50 = !DILocation(line: 32, column: 35, scope: !9)
!51 = !DILocation(line: 32, column: 9, scope: !9)
!52 = !DILocation(line: 32, column: 22, scope: !9)
!53 = distinct !{!53, !36, !54, !43, !44, !45}
!54 = !DILocation(line: 33, column: 5, scope: !9)
!55 = distinct !{!55, !31, !42, !43, !44}
!56 = !DILocation(line: 36, column: 32, scope: !9)
!57 = !DILocation(line: 38, column: 9, scope: !9)
!58 = !DILocation(line: 32, column: 37, scope: !9)
!59 = distinct !{!59, !36, !54, !43, !45, !44}
!60 = !DILocation(line: 40, column: 13, scope: !9)
!61 = !DILocation(line: 47, column: 13, scope: !9)
!62 = !DILocation(line: 47, column: 31, scope: !9)
!63 = !DILocation(line: 47, column: 44, scope: !9)
!64 = !DILocation(line: 48, column: 39, scope: !9)
!65 = !DILocation(line: 48, scope: !9)
!66 = !DILocation(line: 48, column: 32, scope: !9)
!67 = !DILocation(line: 48, column: 17, scope: !9)
!68 = !DILocation(line: 48, column: 30, scope: !9)
!69 = distinct !{!69, !61, !70, !43, !44, !45}
!70 = !DILocation(line: 49, column: 13, scope: !9)
!71 = distinct !{!71, !61, !70, !43, !44, !45}
!72 = !DILocation(line: 42, column: 13, scope: !9)
!73 = !DILocation(line: 42, column: 31, scope: !9)
!74 = !DILocation(line: 42, column: 44, scope: !9)
!75 = !DILocation(line: 43, column: 39, scope: !9)
!76 = !DILocation(line: 43, scope: !9)
!77 = !DILocation(line: 43, column: 32, scope: !9)
!78 = !DILocation(line: 43, column: 17, scope: !9)
!79 = !DILocation(line: 43, column: 30, scope: !9)
!80 = distinct !{!80, !72, !81, !43, !44, !45}
!81 = !DILocation(line: 44, column: 13, scope: !9)
!82 = distinct !{!82, !72, !81, !43, !44, !45}
!83 = distinct !{!83, !72, !81, !43, !45, !44}
!84 = distinct !{!84, !61, !70, !43, !45, !44}
!85 = !DILocation(line: 53, column: 13, scope: !9)
!86 = !DILocation(line: 60, column: 13, scope: !9)
!87 = !DILocation(line: 60, column: 31, scope: !9)
!88 = !DILocation(line: 55, column: 13, scope: !9)
!89 = !DILocation(line: 55, column: 31, scope: !9)
!90 = !DILocation(line: 56, column: 39, scope: !9)
!91 = !DILocation(line: 56, column: 32, scope: !9)
!92 = !DILocation(line: 56, column: 63, scope: !9)
!93 = !DILocation(line: 56, column: 61, scope: !9)
!94 = !DILocation(line: 56, column: 17, scope: !9)
!95 = !DILocation(line: 56, column: 30, scope: !9)
!96 = !DILocation(line: 55, column: 44, scope: !9)
!97 = distinct !{!97, !88, !98, !43}
!98 = !DILocation(line: 57, column: 13, scope: !9)
!99 = !DILocation(line: 61, column: 39, scope: !9)
!100 = !DILocation(line: 61, column: 32, scope: !9)
!101 = !DILocation(line: 61, column: 56, scope: !9)
!102 = !DILocation(line: 61, column: 54, scope: !9)
!103 = !DILocation(line: 61, column: 17, scope: !9)
!104 = !DILocation(line: 61, column: 30, scope: !9)
!105 = !DILocation(line: 60, column: 44, scope: !9)
!106 = distinct !{!106, !86, !107, !43}
!107 = !DILocation(line: 62, column: 13, scope: !9)
!108 = !DILocation(line: 67, column: 5, scope: !9)
!109 = !DILocation(line: 67, column: 23, scope: !9)
!110 = !DILocation(line: 71, column: 18, scope: !9)
!111 = !DILocation(line: 71, column: 5, scope: !9)
!112 = !DILocation(line: 71, column: 23, scope: !9)
!113 = !DILocation(line: 72, column: 5, scope: !9)
!114 = !DILocation(line: 72, column: 23, scope: !9)
!115 = !DILocation(line: 73, column: 1, scope: !9)
!116 = !DILocation(line: 68, column: 22, scope: !9)
!117 = !DILocation(line: 68, column: 31, scope: !9)
!118 = !DILocation(line: 68, column: 29, scope: !9)
!119 = !DILocation(line: 68, column: 15, scope: !9)
!120 = !DILocation(line: 68, column: 9, scope: !9)
!121 = !DILocation(line: 68, column: 20, scope: !9)
!122 = distinct !{!122, !108, !123, !43}
!123 = !DILocation(line: 69, column: 5, scope: !9)
