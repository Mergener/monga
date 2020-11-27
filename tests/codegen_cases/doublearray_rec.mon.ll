source_filename = "tests/codegen_cases/doublearray_rec.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare i8* @RtInternal_GetAllocSize(i64)
declare void @RtInternal_Init()

define void @printDoubleArray(double*, i32, i32) {
	%t.0 = alloca double*
	store double* %0, double** %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1
	%t.2 = alloca i32
	store i32 %2, i32* %t.2

	%t.3 = load i32, i32* %t.1
	%t.4 = icmp slt i32 %t.3, 0
	br i1 %t.4, label %l.0, label %l.1
l.0:
	ret void
l.1:
	%t.5 = load i32, i32* %t.1
	%t.6 = load i32, i32* %t.2
	%t.7 = icmp sge i32 %t.5, %t.6
	br i1 %t.7, label %l.2, label %l.3
l.2:
	ret void
l.3:
	%t.8 = load i32, i32* %t.1
	%t.9 = sext i32 %t.8 to i64
	%t.10 = load double*, double** %t.0
	%t.11 = getelementptr double, double* %t.10, i64 %t.9
	%t.12 = load double, double* %t.11
	%t.13 = fptrunc double %t.12 to float
	call void @printFloat(float %t.13)

	%t.14 = load double*, double** %t.0
	%t.15 = load i32, i32* %t.1
	%t.16 = add i32 %t.15, 1
	%t.17 = load i32, i32* %t.2
	call void @printDoubleArray(double* %t.14, i32 %t.16, i32 %t.17)

	ret void
}

define void @fillArray(double*, i32, i32, double) {
	%t.0 = alloca double*
	store double* %0, double** %t.0
	%t.1 = alloca i32
	store i32 %1, i32* %t.1
	%t.2 = alloca i32
	store i32 %2, i32* %t.2
	%t.3 = alloca double
	store double %3, double* %t.3

	%t.4 = load i32, i32* %t.1
	%t.5 = icmp slt i32 %t.4, 0
	br i1 %t.5, label %l.0, label %l.1
l.0:
	ret void
l.1:
	%t.6 = load i32, i32* %t.1
	%t.7 = load i32, i32* %t.2
	%t.8 = icmp sge i32 %t.6, %t.7
	br i1 %t.8, label %l.2, label %l.3
l.2:
	ret void
l.3:
	%t.9 = load i32, i32* %t.1
	%t.10 = sext i32 %t.9 to i64
	%t.11 = load double*, double** %t.0
	%t.12 = getelementptr double, double* %t.11, i64 %t.10
	%t.13 = load double, double* %t.3
	store double %t.13, double* %t.12

	%t.14 = load double*, double** %t.0
	%t.15 = load i32, i32* %t.1
	%t.16 = add i32 %t.15, 1
	%t.17 = load i32, i32* %t.2
	%t.18 = load double, double* %t.3
	%t.19 = fadd double %t.18, 2.000000
	call void @fillArray(double* %t.14, i32 %t.16, i32 %t.17, double %t.19)

	ret void
}

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca i32

	store i32 0, i32* %t.0
	%t.1 = alloca double*

	store double* null, double** %t.1
	store i32 12, i32* %t.0

	%t.2 = load i32, i32* %t.0
	%t.3 = sext i32 %t.2 to i64
	%t.4 = mul i64 %t.3, 8
	%t.5 = call i8* @RtInternal_GcAlloc(i64 %t.4)
	%t.6 = bitcast i8* %t.5 to double*
	store double* %t.6, double** %t.1

	%t.7 = load double*, double** %t.1
	%t.8 = load i32, i32* %t.0
	call void @fillArray(double* %t.7, i32 0, i32 %t.8, double 3.000000)

	%t.9 = load double*, double** %t.1
	%t.10 = load i32, i32* %t.0
	call void @printDoubleArray(double* %t.9, i32 0, i32 %t.10)

	%t.11 = load double*, double** %t.1
	%t.13 = sub i32 0, 3
	call void @fillArray(double* %t.11, i32 0, i32 %t.13, double 3.000000)

	%t.14 = load double*, double** %t.1
	%t.15 = load i32, i32* %t.0
	%t.16 = sub i32 %t.15, 2
	call void @printDoubleArray(double* %t.14, i32 0, i32 %t.16)

	ret void
}

declare void @printFloat(float)


