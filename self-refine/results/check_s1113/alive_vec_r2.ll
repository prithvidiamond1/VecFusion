; ModuleID = '/Users/torence/VecTrans/self-refine/results/check_s1113/alive_vec_r2.c'
source_filename = "/Users/torence/VecTrans/self-refine/results/check_s1113/alive_vec_r2.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define void @s1113(i32 noundef %0, i32 noundef %1, ptr noundef %2, ptr noundef %3) #0 {
  %5 = alloca i32, align 4
  %6 = alloca i32, align 4
  %7 = alloca ptr, align 8
  %8 = alloca ptr, align 8
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  %11 = alloca float, align 4
  %12 = alloca i32, align 4
  store i32 %0, ptr %5, align 4
  store i32 %1, ptr %6, align 4
  store ptr %2, ptr %7, align 8
  store ptr %3, ptr %8, align 8
  %13 = load i32, ptr %6, align 4
  %14 = sdiv i32 %13, 2
  store i32 %14, ptr %9, align 4
  store i32 0, ptr %10, align 4
  br label %15

15:                                               ; preds = %184, %4
  %16 = load i32, ptr %10, align 4
  %17 = load i32, ptr %5, align 4
  %18 = mul nsw i32 2, %17
  %19 = icmp slt i32 %16, %18
  br i1 %19, label %20, label %187

20:                                               ; preds = %15
  %21 = load ptr, ptr %7, align 8
  %22 = load i32, ptr %9, align 4
  %23 = sext i32 %22 to i64
  %24 = getelementptr inbounds float, ptr %21, i64 %23
  %25 = load float, ptr %24, align 4
  store float %25, ptr %11, align 4
  store i32 0, ptr %12, align 4
  br label %26

26:                                               ; preds = %136, %20
  %27 = load i32, ptr %12, align 4
  %28 = load i32, ptr %9, align 4
  %29 = sub nsw i32 %28, 8
  %30 = icmp sle i32 %27, %29
  br i1 %30, label %31, label %139

31:                                               ; preds = %26
  %32 = load float, ptr %11, align 4
  %33 = load ptr, ptr %8, align 8
  %34 = load i32, ptr %12, align 4
  %35 = add nsw i32 %34, 0
  %36 = sext i32 %35 to i64
  %37 = getelementptr inbounds float, ptr %33, i64 %36
  %38 = load float, ptr %37, align 4
  %39 = fadd float %32, %38
  %40 = load ptr, ptr %7, align 8
  %41 = load i32, ptr %12, align 4
  %42 = add nsw i32 %41, 0
  %43 = sext i32 %42 to i64
  %44 = getelementptr inbounds float, ptr %40, i64 %43
  store float %39, ptr %44, align 4
  %45 = load float, ptr %11, align 4
  %46 = load ptr, ptr %8, align 8
  %47 = load i32, ptr %12, align 4
  %48 = add nsw i32 %47, 1
  %49 = sext i32 %48 to i64
  %50 = getelementptr inbounds float, ptr %46, i64 %49
  %51 = load float, ptr %50, align 4
  %52 = fadd float %45, %51
  %53 = load ptr, ptr %7, align 8
  %54 = load i32, ptr %12, align 4
  %55 = add nsw i32 %54, 1
  %56 = sext i32 %55 to i64
  %57 = getelementptr inbounds float, ptr %53, i64 %56
  store float %52, ptr %57, align 4
  %58 = load float, ptr %11, align 4
  %59 = load ptr, ptr %8, align 8
  %60 = load i32, ptr %12, align 4
  %61 = add nsw i32 %60, 2
  %62 = sext i32 %61 to i64
  %63 = getelementptr inbounds float, ptr %59, i64 %62
  %64 = load float, ptr %63, align 4
  %65 = fadd float %58, %64
  %66 = load ptr, ptr %7, align 8
  %67 = load i32, ptr %12, align 4
  %68 = add nsw i32 %67, 2
  %69 = sext i32 %68 to i64
  %70 = getelementptr inbounds float, ptr %66, i64 %69
  store float %65, ptr %70, align 4
  %71 = load float, ptr %11, align 4
  %72 = load ptr, ptr %8, align 8
  %73 = load i32, ptr %12, align 4
  %74 = add nsw i32 %73, 3
  %75 = sext i32 %74 to i64
  %76 = getelementptr inbounds float, ptr %72, i64 %75
  %77 = load float, ptr %76, align 4
  %78 = fadd float %71, %77
  %79 = load ptr, ptr %7, align 8
  %80 = load i32, ptr %12, align 4
  %81 = add nsw i32 %80, 3
  %82 = sext i32 %81 to i64
  %83 = getelementptr inbounds float, ptr %79, i64 %82
  store float %78, ptr %83, align 4
  %84 = load float, ptr %11, align 4
  %85 = load ptr, ptr %8, align 8
  %86 = load i32, ptr %12, align 4
  %87 = add nsw i32 %86, 4
  %88 = sext i32 %87 to i64
  %89 = getelementptr inbounds float, ptr %85, i64 %88
  %90 = load float, ptr %89, align 4
  %91 = fadd float %84, %90
  %92 = load ptr, ptr %7, align 8
  %93 = load i32, ptr %12, align 4
  %94 = add nsw i32 %93, 4
  %95 = sext i32 %94 to i64
  %96 = getelementptr inbounds float, ptr %92, i64 %95
  store float %91, ptr %96, align 4
  %97 = load float, ptr %11, align 4
  %98 = load ptr, ptr %8, align 8
  %99 = load i32, ptr %12, align 4
  %100 = add nsw i32 %99, 5
  %101 = sext i32 %100 to i64
  %102 = getelementptr inbounds float, ptr %98, i64 %101
  %103 = load float, ptr %102, align 4
  %104 = fadd float %97, %103
  %105 = load ptr, ptr %7, align 8
  %106 = load i32, ptr %12, align 4
  %107 = add nsw i32 %106, 5
  %108 = sext i32 %107 to i64
  %109 = getelementptr inbounds float, ptr %105, i64 %108
  store float %104, ptr %109, align 4
  %110 = load float, ptr %11, align 4
  %111 = load ptr, ptr %8, align 8
  %112 = load i32, ptr %12, align 4
  %113 = add nsw i32 %112, 6
  %114 = sext i32 %113 to i64
  %115 = getelementptr inbounds float, ptr %111, i64 %114
  %116 = load float, ptr %115, align 4
  %117 = fadd float %110, %116
  %118 = load ptr, ptr %7, align 8
  %119 = load i32, ptr %12, align 4
  %120 = add nsw i32 %119, 6
  %121 = sext i32 %120 to i64
  %122 = getelementptr inbounds float, ptr %118, i64 %121
  store float %117, ptr %122, align 4
  %123 = load float, ptr %11, align 4
  %124 = load ptr, ptr %8, align 8
  %125 = load i32, ptr %12, align 4
  %126 = add nsw i32 %125, 7
  %127 = sext i32 %126 to i64
  %128 = getelementptr inbounds float, ptr %124, i64 %127
  %129 = load float, ptr %128, align 4
  %130 = fadd float %123, %129
  %131 = load ptr, ptr %7, align 8
  %132 = load i32, ptr %12, align 4
  %133 = add nsw i32 %132, 7
  %134 = sext i32 %133 to i64
  %135 = getelementptr inbounds float, ptr %131, i64 %134
  store float %130, ptr %135, align 4
  br label %136

136:                                              ; preds = %31
  %137 = load i32, ptr %12, align 4
  %138 = add nsw i32 %137, 8
  store i32 %138, ptr %12, align 4
  br label %26, !llvm.loop !6

139:                                              ; preds = %26
  br label %140

140:                                              ; preds = %156, %139
  %141 = load i32, ptr %12, align 4
  %142 = load i32, ptr %9, align 4
  %143 = icmp slt i32 %141, %142
  br i1 %143, label %144, label %159

144:                                              ; preds = %140
  %145 = load float, ptr %11, align 4
  %146 = load ptr, ptr %8, align 8
  %147 = load i32, ptr %12, align 4
  %148 = sext i32 %147 to i64
  %149 = getelementptr inbounds float, ptr %146, i64 %148
  %150 = load float, ptr %149, align 4
  %151 = fadd float %145, %150
  %152 = load ptr, ptr %7, align 8
  %153 = load i32, ptr %12, align 4
  %154 = sext i32 %153 to i64
  %155 = getelementptr inbounds float, ptr %152, i64 %154
  store float %151, ptr %155, align 4
  br label %156

156:                                              ; preds = %144
  %157 = load i32, ptr %12, align 4
  %158 = add nsw i32 %157, 1
  store i32 %158, ptr %12, align 4
  br label %140, !llvm.loop !8

159:                                              ; preds = %140
  br label %160

160:                                              ; preds = %180, %159
  %161 = load i32, ptr %12, align 4
  %162 = load i32, ptr %6, align 4
  %163 = icmp slt i32 %161, %162
  br i1 %163, label %164, label %183

164:                                              ; preds = %160
  %165 = load ptr, ptr %7, align 8
  %166 = load i32, ptr %9, align 4
  %167 = sext i32 %166 to i64
  %168 = getelementptr inbounds float, ptr %165, i64 %167
  %169 = load float, ptr %168, align 4
  %170 = load ptr, ptr %8, align 8
  %171 = load i32, ptr %12, align 4
  %172 = sext i32 %171 to i64
  %173 = getelementptr inbounds float, ptr %170, i64 %172
  %174 = load float, ptr %173, align 4
  %175 = fadd float %169, %174
  %176 = load ptr, ptr %7, align 8
  %177 = load i32, ptr %12, align 4
  %178 = sext i32 %177 to i64
  %179 = getelementptr inbounds float, ptr %176, i64 %178
  store float %175, ptr %179, align 4
  br label %180

180:                                              ; preds = %164
  %181 = load i32, ptr %12, align 4
  %182 = add nsw i32 %181, 1
  store i32 %182, ptr %12, align 4
  br label %160, !llvm.loop !9

183:                                              ; preds = %160
  br label %184

184:                                              ; preds = %183
  %185 = load i32, ptr %10, align 4
  %186 = add nsw i32 %185, 1
  store i32 %186, ptr %10, align 4
  br label %15, !llvm.loop !10

187:                                              ; preds = %15
  ret void
}

attributes #0 = { noinline nounwind optnone ssp uwtable(sync) "frame-pointer"="non-leaf" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="apple-m1" "target-features"="+aes,+altnzcv,+ccdp,+ccidx,+ccpp,+complxnum,+crc,+dit,+dotprod,+flagm,+fp-armv8,+fp16fml,+fptoint,+fullfp16,+jsconv,+lse,+neon,+pauth,+perfmon,+predres,+ras,+rcpc,+rdm,+sb,+sha2,+sha3,+specrestrict,+ssbs,+v8.1a,+v8.2a,+v8.3a,+v8.4a,+v8a,+zcm,+zcz" }

!llvm.module.flags = !{!0, !1, !2, !3, !4}
!llvm.ident = !{!5}

!0 = !{i32 2, !"SDK Version", [2 x i32] [i32 26, i32 2]}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{i32 8, !"PIC Level", i32 2}
!3 = !{i32 7, !"uwtable", i32 1}
!4 = !{i32 7, !"frame-pointer", i32 1}
!5 = !{!"clang version 20.1.8 (https://github.com/llvm/llvm-project.git 87f0227cb60147a26a1eeb4fb06e3b505e9c7261)"}
!6 = distinct !{!6, !7}
!7 = !{!"llvm.loop.mustprogress"}
!8 = distinct !{!8, !7}
!9 = distinct !{!9, !7}
!10 = distinct !{!10, !7}
