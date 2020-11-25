source_filename = "tests/codegen_cases/conversions.mon"

declare i8* @RtInternal_GcAlloc(i32)

define void @main() {
	%t.0 = alloca i8

	%t.1 = alloca i16

	%t.2 = alloca i32

	%t.3 = alloca i64

	%t.4 = alloca float

	%t.5 = alloca double

	%t.6 = trunc i32 2 to i8
	store i8 %t.6, i8* %t.0

	%t.7 = load i8, i8* %t.0
	%t.8 = sext i8 %t.7 to i32
	call void @printInteger(i32 %t.8)

	%t.9 = trunc i32 4 to i16
	store i16 %t.9, i16* %t.1

	%t.10 = load i16, i16* %t.1
	%t.11 = sext i16 %t.10 to i32
	call void @printInteger(i32 %t.11)

	%t.12 = sext i32 2147483647 to i64
	store i64 %t.12, i64* %t.3

	%t.13 = load i64, i64* %t.3
	%t.14 = trunc i64 %t.13 to i32
	store i32 %t.14, i32* %t.2

	%t.15 = load i32, i32* %t.2
	call void @printInteger(i32 %t.15)

	%t.16 = load i32, i32* %t.2
	%t.17 = sitofp i32 %t.16 to float
	store float %t.17, float* %t.4

	%t.18 = load float, float* %t.4
	%t.19 = fpext float %t.18 to double
	store double %t.19, double* %t.5

	%t.20 = load float, float* %t.4
	call void @printFloat(float %t.20)

	ret void
}

declare void @printInteger(i32)declare void @printFloat(float)
