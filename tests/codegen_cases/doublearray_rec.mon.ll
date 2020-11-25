source_filename = "tests/codegen_cases/doublearray_rec.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
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
	%t.3 = mul i32 %t.2, 8
	%t.4 = call i8* @RtInternal_GcAlloc(i32 %t.3)
	%t.5 = bitcast i8* %t.4 to double*
	store double* %t.5, double** %t.1

	%t.6 = load double*, double** %t.1
	%t.7 = load i32, i32* %t.0
	call void @fillArray(double* %t.6, i32 0, i32 %t.7, double 3.000000)

	%t.8 = load double*, double** %t.1
	%t.9 = load i32, i32* %t.0
	call void @printDoubleArray(double* %t.8, i32 0, i32 %t.9)

	%t.10 = load double*, double** %t.1
	%t.12 = sub i32 0, 3
	call void @fillArray(double* %t.10, i32 0, i32 %t.12, double 3.000000)

	%t.13 = load double*, double** %t.1
	%t.14 = load i32, i32* %t.0
	%t.15 = sub i32 %t.14, 2
	call void @printDoubleArray(double* %t.13, i32 0, i32 %t.15)

	ret void
}

declare void @printFloat(float)


