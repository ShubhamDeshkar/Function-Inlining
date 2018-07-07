; ModuleID = 'example5.mypass.bc'
target datalayout = "e-p:64:64:64-i1:8:8-i8:8:8-i16:16:16-i32:32:32-i64:64:64-f32:32:32-f64:64:64-v64:64:64-v128:128:128-a0:0:64-s0:64:64-f80:128:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [5 x i8] c"%d \0A\00", align 1

; Function Attrs: nounwind uwtable
define i32 @pow2(i32 %x) #0 {
  %1 = alloca i32, align 4
  %y = alloca i32, align 4
  store i32 7, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = load i32* %1, align 4
  %4 = mul nsw i32 %2, %3
  store i32 %4, i32* %y, align 4
  %5 = load i32* %y, align 4
  ret i32 %5
}

; Function Attrs: nounwind uwtable
define i32 @poly(i32 %y) #0 {
  %1 = alloca i32, align 4
  %p2 = alloca i32, align 4
  store i32 %y, i32* %1, align 4
  %2 = alloca i32, align 4
  %3 = alloca i32, align 4
  store i32 7, i32* %2, align 4
  %4 = load i32* %2, align 4
  %5 = load i32* %2, align 4
  %6 = mul nsw i32 %4, %5
  store i32 %6, i32* %3, align 4
  %7 = load i32* %3, align 4
  store i32 %7, i32* %p2, align 4
  %8 = load i32* %p2, align 4
  %9 = add nsw i32 %8, 10
  store i32 %9, i32* %p2, align 4
  %10 = load i32* %p2, align 4
  %11 = load i32* %1, align 4
  %12 = sub nsw i32 %10, %11
  store i32 %12, i32* %p2, align 4
  %13 = load i32* %p2, align 4
  ret i32 %13
}

; Function Attrs: nounwind uwtable
define void @print_int(i32 %x) #0 {
  %1 = alloca i32, align 4
  store i32 %x, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = call i32 (i8*, ...)* @printf(i8* getelementptr inbounds ([5 x i8]* @.str, i32 0, i32 0), i32 %2)
  ret void
}

declare i32 @printf(i8*, ...) #1

; Function Attrs: nounwind uwtable
define i32 @main() #0 {
  %1 = alloca i32, align 4
  %p = alloca i32, align 4
  store i32 0, i32* %1
  %2 = call i32 @poly(i32 3)
  store i32 %2, i32* %p, align 4
  %3 = load i32* %p, align 4
  call void @print_int(i32 %3)
  ret i32 0
}

attributes #0 = { nounwind uwtable "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"clang version 3.4 (tags/RELEASE_34/final)"}
