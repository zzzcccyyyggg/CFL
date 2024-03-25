; ModuleID = '../test/main.c'
source_filename = "../test/main.c"
target datalayout = "e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-pc-linux-gnu"

@.str = private unnamed_addr constant [3 x i8] c"%d\00", align 1
@.str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1
@.str.2 = private unnamed_addr constant [10 x i8] c"Excellent\00", align 1
@.str.3 = private unnamed_addr constant [5 x i8] c"Good\00", align 1
@.str.4 = private unnamed_addr constant [8 x i8] c"Average\00", align 1
@.str.5 = private unnamed_addr constant [5 x i8] c"Pass\00", align 1
@.str.6 = private unnamed_addr constant [8 x i8] c"Failing\00", align 1
@.str.7 = private unnamed_addr constant [6 x i8] c"Error\00", align 1
@__cfl_area_ptr = external global i8*
@__cfl_prev_loc = external thread_local global i32

; Function Attrs: noinline nounwind optnone uwtable
define dso_local i32 @main() #0 {
  %1 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %2 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %3 = xor i32 %1, 47765
  %4 = getelementptr i8, i8* %2, i32 %3
  %5 = load i8, i8* %4, align 1, !nosanitize !2
  %6 = add i8 %5, 1
  store i8 %6, i8* %4, align 1, !nosanitize !2
  store i32 23882, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %7 = alloca i32, align 4
  %8 = alloca i32, align 4
  %9 = alloca i32, align 4
  store i32 0, i32* %7, align 4
  store i32 0, i32* %9, align 4
  br label %10

10:                                               ; preds = %225, %0
  %11 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %12 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %13 = xor i32 %11, 13208
  %14 = getelementptr i8, i8* %12, i32 %13
  %15 = load i8, i8* %14, align 1, !nosanitize !2
  %16 = add i8 %15, 1
  store i8 %16, i8* %14, align 1, !nosanitize !2
  store i32 6604, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %17 = call i32 (i8*, ...) @__isoc99_scanf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @.str, i64 0, i64 0), i32* %8)
  %18 = icmp ne i32 %17, -1
  br i1 %18, label %19, label %232

19:                                               ; preds = %10
  %20 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %21 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %22 = xor i32 %20, 44054
  %23 = getelementptr i8, i8* %21, i32 %22
  %24 = load i8, i8* %23, align 1, !nosanitize !2
  %25 = add i8 %24, 1
  store i8 %25, i8* %23, align 1, !nosanitize !2
  store i32 22027, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %26 = load i32, i32* %9, align 4
  %27 = icmp ne i32 %26, 0
  br i1 %27, label %28, label %36

28:                                               ; preds = %19
  %29 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %30 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %31 = xor i32 %29, 17445
  %32 = getelementptr i8, i8* %30, i32 %31
  %33 = load i8, i8* %32, align 1, !nosanitize !2
  %34 = add i8 %33, 1
  store i8 %34, i8* %32, align 1, !nosanitize !2
  store i32 8722, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.1, i64 0, i64 0))
  br label %59

36:                                               ; preds = %19
  %37 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %38 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %39 = xor i32 %37, 32602
  %40 = getelementptr i8, i8* %38, i32 %39
  %41 = load i8, i8* %40, align 1, !nosanitize !2
  %42 = add i8 %41, 1
  store i8 %42, i8* %40, align 1, !nosanitize !2
  store i32 16301, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %43 = load i32, i32* %9, align 4
  %44 = icmp ne i32 %43, 0
  br i1 %44, label %52, label %45

45:                                               ; preds = %36
  %46 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %47 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %48 = xor i32 %46, 61274
  %49 = getelementptr i8, i8* %47, i32 %48
  %50 = load i8, i8* %49, align 1, !nosanitize !2
  %51 = add i8 %50, 1
  store i8 %51, i8* %49, align 1, !nosanitize !2
  store i32 30637, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  store i32 1, i32* %9, align 4
  br label %52

52:                                               ; preds = %45, %36
  %53 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %54 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %55 = xor i32 %53, 60732
  %56 = getelementptr i8, i8* %54, i32 %55
  %57 = load i8, i8* %56, align 1, !nosanitize !2
  %58 = add i8 %57, 1
  store i8 %58, i8* %56, align 1, !nosanitize !2
  store i32 30366, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  br label %59

59:                                               ; preds = %52, %28
  %60 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %61 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %62 = xor i32 %60, 28792
  %63 = getelementptr i8, i8* %61, i32 %62
  %64 = load i8, i8* %63, align 1, !nosanitize !2
  %65 = add i8 %64, 1
  store i8 %65, i8* %63, align 1, !nosanitize !2
  store i32 14396, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %66 = load i32, i32* %8, align 4
  %67 = icmp sge i32 %66, 90
  br i1 %67, label %68, label %85

68:                                               ; preds = %59
  %69 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %70 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %71 = xor i32 %69, 2101
  %72 = getelementptr i8, i8* %70, i32 %71
  %73 = load i8, i8* %72, align 1, !nosanitize !2
  %74 = add i8 %73, 1
  store i8 %74, i8* %72, align 1, !nosanitize !2
  store i32 1050, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %75 = load i32, i32* %8, align 4
  %76 = icmp sle i32 %75, 100
  br i1 %76, label %77, label %85

77:                                               ; preds = %68
  %78 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %79 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %80 = xor i32 %78, 33014
  %81 = getelementptr i8, i8* %79, i32 %80
  %82 = load i8, i8* %81, align 1, !nosanitize !2
  %83 = add i8 %82, 1
  store i8 %83, i8* %81, align 1, !nosanitize !2
  store i32 16507, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %84 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([10 x i8], [10 x i8]* @.str.2, i64 0, i64 0))
  br label %225

85:                                               ; preds = %68, %59
  %86 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %87 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %88 = xor i32 %86, 59167
  %89 = getelementptr i8, i8* %87, i32 %88
  %90 = load i8, i8* %89, align 1, !nosanitize !2
  %91 = add i8 %90, 1
  store i8 %91, i8* %89, align 1, !nosanitize !2
  store i32 29583, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %92 = load i32, i32* %8, align 4
  %93 = icmp sge i32 %92, 80
  br i1 %93, label %94, label %111

94:                                               ; preds = %85
  %95 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %96 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %97 = xor i32 %95, 32265
  %98 = getelementptr i8, i8* %96, i32 %97
  %99 = load i8, i8* %98, align 1, !nosanitize !2
  %100 = add i8 %99, 1
  store i8 %100, i8* %98, align 1, !nosanitize !2
  store i32 16132, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %101 = load i32, i32* %8, align 4
  %102 = icmp slt i32 %101, 90
  br i1 %102, label %103, label %111

103:                                              ; preds = %94
  %104 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %105 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %106 = xor i32 %104, 29910
  %107 = getelementptr i8, i8* %105, i32 %106
  %108 = load i8, i8* %107, align 1, !nosanitize !2
  %109 = add i8 %108, 1
  store i8 %109, i8* %107, align 1, !nosanitize !2
  store i32 14955, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %110 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.3, i64 0, i64 0))
  br label %218

111:                                              ; preds = %94, %85
  %112 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %113 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %114 = xor i32 %112, 27568
  %115 = getelementptr i8, i8* %113, i32 %114
  %116 = load i8, i8* %115, align 1, !nosanitize !2
  %117 = add i8 %116, 1
  store i8 %117, i8* %115, align 1, !nosanitize !2
  store i32 13784, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %118 = load i32, i32* %8, align 4
  %119 = icmp sge i32 %118, 70
  br i1 %119, label %120, label %137

120:                                              ; preds = %111
  %121 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %122 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %123 = xor i32 %121, 37485
  %124 = getelementptr i8, i8* %122, i32 %123
  %125 = load i8, i8* %124, align 1, !nosanitize !2
  %126 = add i8 %125, 1
  store i8 %126, i8* %124, align 1, !nosanitize !2
  store i32 18742, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %127 = load i32, i32* %8, align 4
  %128 = icmp slt i32 %127, 80
  br i1 %128, label %129, label %137

129:                                              ; preds = %120
  %130 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %131 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %132 = xor i32 %130, 64918
  %133 = getelementptr i8, i8* %131, i32 %132
  %134 = load i8, i8* %133, align 1, !nosanitize !2
  %135 = add i8 %134, 1
  store i8 %135, i8* %133, align 1, !nosanitize !2
  store i32 32459, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %136 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.4, i64 0, i64 0))
  br label %211

137:                                              ; preds = %120, %111
  %138 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %139 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %140 = xor i32 %138, 8364
  %141 = getelementptr i8, i8* %139, i32 %140
  %142 = load i8, i8* %141, align 1, !nosanitize !2
  %143 = add i8 %142, 1
  store i8 %143, i8* %141, align 1, !nosanitize !2
  store i32 4182, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %144 = load i32, i32* %8, align 4
  %145 = icmp sge i32 %144, 60
  br i1 %145, label %146, label %163

146:                                              ; preds = %137
  %147 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %148 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %149 = xor i32 %147, 46196
  %150 = getelementptr i8, i8* %148, i32 %149
  %151 = load i8, i8* %150, align 1, !nosanitize !2
  %152 = add i8 %151, 1
  store i8 %152, i8* %150, align 1, !nosanitize !2
  store i32 23098, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %153 = load i32, i32* %8, align 4
  %154 = icmp slt i32 %153, 70
  br i1 %154, label %155, label %163

155:                                              ; preds = %146
  %156 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %157 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %158 = xor i32 %156, 40277
  %159 = getelementptr i8, i8* %157, i32 %158
  %160 = load i8, i8* %159, align 1, !nosanitize !2
  %161 = add i8 %160, 1
  store i8 %161, i8* %159, align 1, !nosanitize !2
  store i32 20138, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %162 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str.5, i64 0, i64 0))
  br label %204

163:                                              ; preds = %146, %137
  %164 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %165 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %166 = xor i32 %164, 53445
  %167 = getelementptr i8, i8* %165, i32 %166
  %168 = load i8, i8* %167, align 1, !nosanitize !2
  %169 = add i8 %168, 1
  store i8 %169, i8* %167, align 1, !nosanitize !2
  store i32 26722, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %170 = load i32, i32* %8, align 4
  %171 = icmp sge i32 %170, 0
  br i1 %171, label %172, label %189

172:                                              ; preds = %163
  %173 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %174 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %175 = xor i32 %173, 35907
  %176 = getelementptr i8, i8* %174, i32 %175
  %177 = load i8, i8* %176, align 1, !nosanitize !2
  %178 = add i8 %177, 1
  store i8 %178, i8* %176, align 1, !nosanitize !2
  store i32 17953, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %179 = load i32, i32* %8, align 4
  %180 = icmp slt i32 %179, 60
  br i1 %180, label %181, label %189

181:                                              ; preds = %172
  %182 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %183 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %184 = xor i32 %182, 32102
  %185 = getelementptr i8, i8* %183, i32 %184
  %186 = load i8, i8* %185, align 1, !nosanitize !2
  %187 = add i8 %186, 1
  store i8 %187, i8* %185, align 1, !nosanitize !2
  store i32 16051, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %188 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([8 x i8], [8 x i8]* @.str.6, i64 0, i64 0))
  br label %197

189:                                              ; preds = %172, %163
  %190 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %191 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %192 = xor i32 %190, 32721
  %193 = getelementptr i8, i8* %191, i32 %192
  %194 = load i8, i8* %193, align 1, !nosanitize !2
  %195 = add i8 %194, 1
  store i8 %195, i8* %193, align 1, !nosanitize !2
  store i32 16360, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %196 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @.str.7, i64 0, i64 0))
  br label %197

197:                                              ; preds = %189, %181
  %198 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %199 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %200 = xor i32 %198, 53292
  %201 = getelementptr i8, i8* %199, i32 %200
  %202 = load i8, i8* %201, align 1, !nosanitize !2
  %203 = add i8 %202, 1
  store i8 %203, i8* %201, align 1, !nosanitize !2
  store i32 26646, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  br label %204

204:                                              ; preds = %197, %155
  %205 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %206 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %207 = xor i32 %205, 26571
  %208 = getelementptr i8, i8* %206, i32 %207
  %209 = load i8, i8* %208, align 1, !nosanitize !2
  %210 = add i8 %209, 1
  store i8 %210, i8* %208, align 1, !nosanitize !2
  store i32 13285, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  br label %211

211:                                              ; preds = %204, %129
  %212 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %213 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %214 = xor i32 %212, 17064
  %215 = getelementptr i8, i8* %213, i32 %214
  %216 = load i8, i8* %215, align 1, !nosanitize !2
  %217 = add i8 %216, 1
  store i8 %217, i8* %215, align 1, !nosanitize !2
  store i32 8532, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  br label %218

218:                                              ; preds = %211, %103
  %219 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %220 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %221 = xor i32 %219, 20693
  %222 = getelementptr i8, i8* %220, i32 %221
  %223 = load i8, i8* %222, align 1, !nosanitize !2
  %224 = add i8 %223, 1
  store i8 %224, i8* %222, align 1, !nosanitize !2
  store i32 10346, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  br label %225

225:                                              ; preds = %218, %77
  %226 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %227 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %228 = xor i32 %226, 44471
  %229 = getelementptr i8, i8* %227, i32 %228
  %230 = load i8, i8* %229, align 1, !nosanitize !2
  %231 = add i8 %230, 1
  store i8 %231, i8* %229, align 1, !nosanitize !2
  store i32 22235, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  br label %10, !llvm.loop !3

232:                                              ; preds = %10
  %233 = load i32, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  %234 = load i8*, i8** @__cfl_area_ptr, align 8, !nosanitize !2
  %235 = xor i32 %233, 45658
  %236 = getelementptr i8, i8* %234, i32 %235
  %237 = load i8, i8* %236, align 1, !nosanitize !2
  %238 = add i8 %237, 1
  store i8 %238, i8* %236, align 1, !nosanitize !2
  store i32 22829, i32* @__cfl_prev_loc, align 4, !nosanitize !2
  ret i32 0
}

declare dso_local i32 @__isoc99_scanf(i8*, ...) #1

declare dso_local i32 @printf(i8*, ...) #1

attributes #0 = { noinline nounwind optnone uwtable "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="true" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "tune-cpu"="generic" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"Ubuntu clang version 12.0.0-3ubuntu1~20.04.5"}
!2 = !{}
!3 = distinct !{!3, !4}
!4 = !{!"llvm.loop.mustprogress"}
