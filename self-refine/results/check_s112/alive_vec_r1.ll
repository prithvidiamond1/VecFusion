; ModuleID = '/Users/torence/VecTrans/self-refine/results/check_s112/alive_vec_r1.c'
source_filename = "/Users/torence/VecTrans/self-refine/results/check_s112/alive_vec_r1.c"
target datalayout = "e-m:o-p270:32:32-p271:32:32-p272:64:64-i64:64-i128:128-n32:64-S128-Fn32"
target triple = "arm64-apple-macosx15.0.0"

; Function Attrs: noinline nounwind optnone ssp uwtable(sync)
define void @s112(ptr noundef %0, ptr noundef %1, i32 noundef %2, i32 noundef %3) #0 {
  %5 = alloca ptr, align 8
  %6 = alloca ptr, align 8
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  %10 = alloca i32, align 4
  store ptr %0, ptr %5, align 8
  store ptr %1, ptr %6, align 8
  store i32 %2, ptr %7, align 4
  store i32 %3, ptr %8, align 4
  store i32 0, ptr %9, align 4
  br label %11

11:                                               ; preds = %120, %4
  %12 = load i32, ptr %9, align 4
  %13 = load i32, ptr %7, align 4
  %14 = mul nsw i32 3, %13
  %15 = icmp slt i32 %12, %14
  br i1 %15, label %16, label %123

16:                                               ; preds = %11
  %17 = load i32, ptr %8, align 4
  %18 = sub nsw i32 %17, 2
  store i32 %18, ptr %10, align 4
  br label %19

19:                                               ; preds = %92, %16
  %20 = load i32, ptr %10, align 4
  %21 = icmp sge i32 %20, 3
  br i1 %21, label %22, label %95

22:                                               ; preds = %19
  %23 = load ptr, ptr %5, align 8
  %24 = load i32, ptr %10, align 4
  %25 = sext i32 %24 to i64
  %26 = getelementptr inbounds float, ptr %23, i64 %25
  %27 = load float, ptr %26, align 4
  %28 = load ptr, ptr %6, align 8
  %29 = load i32, ptr %10, align 4
  %30 = sext i32 %29 to i64
  %31 = getelementptr inbounds float, ptr %28, i64 %30
  %32 = load float, ptr %31, align 4
  %33 = fadd float %27, %32
  %34 = load ptr, ptr %5, align 8
  %35 = load i32, ptr %10, align 4
  %36 = add nsw i32 %35, 1
  %37 = sext i32 %36 to i64
  %38 = getelementptr inbounds float, ptr %34, i64 %37
  store float %33, ptr %38, align 4
  %39 = load ptr, ptr %5, align 8
  %40 = load i32, ptr %10, align 4
  %41 = sub nsw i32 %40, 1
  %42 = sext i32 %41 to i64
  %43 = getelementptr inbounds float, ptr %39, i64 %42
  %44 = load float, ptr %43, align 4
  %45 = load ptr, ptr %6, align 8
  %46 = load i32, ptr %10, align 4
  %47 = sub nsw i32 %46, 1
  %48 = sext i32 %47 to i64
  %49 = getelementptr inbounds float, ptr %45, i64 %48
  %50 = load float, ptr %49, align 4
  %51 = fadd float %44, %50
  %52 = load ptr, ptr %5, align 8
  %53 = load i32, ptr %10, align 4
  %54 = sext i32 %53 to i64
  %55 = getelementptr inbounds float, ptr %52, i64 %54
  store float %51, ptr %55, align 4
  %56 = load ptr, ptr %5, align 8
  %57 = load i32, ptr %10, align 4
  %58 = sub nsw i32 %57, 2
  %59 = sext i32 %58 to i64
  %60 = getelementptr inbounds float, ptr %56, i64 %59
  %61 = load float, ptr %60, align 4
  %62 = load ptr, ptr %6, align 8
  %63 = load i32, ptr %10, align 4
  %64 = sub nsw i32 %63, 2
  %65 = sext i32 %64 to i64
  %66 = getelementptr inbounds float, ptr %62, i64 %65
  %67 = load float, ptr %66, align 4
  %68 = fadd float %61, %67
  %69 = load ptr, ptr %5, align 8
  %70 = load i32, ptr %10, align 4
  %71 = sub nsw i32 %70, 1
  %72 = sext i32 %71 to i64
  %73 = getelementptr inbounds float, ptr %69, i64 %72
  store float %68, ptr %73, align 4
  %74 = load ptr, ptr %5, align 8
  %75 = load i32, ptr %10, align 4
  %76 = sub nsw i32 %75, 3
  %77 = sext i32 %76 to i64
  %78 = getelementptr inbounds float, ptr %74, i64 %77
  %79 = load float, ptr %78, align 4
  %80 = load ptr, ptr %6, align 8
  %81 = load i32, ptr %10, align 4
  %82 = sub nsw i32 %81, 3
  %83 = sext i32 %82 to i64
  %84 = getelementptr inbounds float, ptr %80, i64 %83
  %85 = load float, ptr %84, align 4
  %86 = fadd float %79, %85
  %87 = load ptr, ptr %5, align 8
  %88 = load i32, ptr %10, align 4
  %89 = sub nsw i32 %88, 2
  %90 = sext i32 %89 to i64
  %91 = getelementptr inbounds float, ptr %87, i64 %90
  store float %86, ptr %91, align 4
  br label %92

92:                                               ; preds = %22
  %93 = load i32, ptr %10, align 4
  %94 = sub nsw i32 %93, 4
  store i32 %94, ptr %10, align 4
  br label %19, !llvm.loop !6

95:                                               ; preds = %19
  br label %96

96:                                               ; preds = %116, %95
  %97 = load i32, ptr %10, align 4
  %98 = icmp sge i32 %97, 0
  br i1 %98, label %99, label %119

99:                                               ; preds = %96
  %100 = load ptr, ptr %5, align 8
  %101 = load i32, ptr %10, align 4
  %102 = sext i32 %101 to i64
  %103 = getelementptr inbounds float, ptr %100, i64 %102
  %104 = load float, ptr %103, align 4
  %105 = load ptr, ptr %6, align 8
  %106 = load i32, ptr %10, align 4
  %107 = sext i32 %106 to i64
  %108 = getelementptr inbounds float, ptr %105, i64 %107
  %109 = load float, ptr %108, align 4
  %110 = fadd float %104, %109
  %111 = load ptr, ptr %5, align 8
  %112 = load i32, ptr %10, align 4
  %113 = add nsw i32 %112, 1
  %114 = sext i32 %113 to i64
  %115 = getelementptr inbounds float, ptr %111, i64 %114
  store float %110, ptr %115, align 4
  br label %116

116:                                              ; preds = %99
  %117 = load i32, ptr %10, align 4
  %118 = add nsw i32 %117, -1
  store i32 %118, ptr %10, align 4
  br label %96, !llvm.loop !8

119:                                              ; preds = %96
  br label %120

120:                                              ; preds = %119
  %121 = load i32, ptr %9, align 4
  %122 = add nsw i32 %121, 1
  store i32 %122, ptr %9, align 4
  br label %11, !llvm.loop !9

123:                                              ; preds = %11
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
