source_filename = "tests/codegen_cases/doublearray_rec.mon"

%string = type { i64, i64 }
%.array = type { i64 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare %.array* @RtInternal_GcAllocArray(i64, i64)
declare i64 @RtInternal_GetGcArrayElemCount(%.array*)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare void @RtInternal_Hash(i8*, i32)
declare void @RtInternal_Init()

define void @printDoubleArray(%.array*, i32, i32) {
	%t.0 = alloca %.array*
	store %.array* %0, %.array** %t.0
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
	%t.10 = load %.array*, %.array** %t.0
	%t.11 = getelementptr %.array, %.array* %t.10, i64 1
	%t.12 = bitcast %.array* %t.11 to double*
	%t.13 = getelementptr double, double* %t.12, i64 %t.9
	%t.14 = load double, double* %t.13
	call void @RtInternal_EchoReal(double %t.14)

	%t.15 = load %.array*, %.array** %t.0
	%t.16 = load i32, i32* %t.1
	%t.17 = add i32 %t.16, 1
	%t.18 = load i32, i32* %t.2
	call void @printDoubleArray(%.array* %t.15, i32 %t.17, i32 %t.18)

	ret void
}

define void @fillArray(%.array*, i32, i32, double) {
	%t.0 = alloca %.array*
	store %.array* %0, %.array** %t.0
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
	%t.11 = load %.array*, %.array** %t.0
	%t.12 = getelementptr %.array, %.array* %t.11, i64 1
	%t.13 = bitcast %.array* %t.12 to double*
	%t.14 = getelementptr double, double* %t.13, i64 %t.10
	%t.15 = load double, double* %t.3
	store double %t.15, double* %t.14

	%t.16 = load %.array*, %.array** %t.0
	%t.17 = load i32, i32* %t.1
	%t.18 = add i32 %t.17, 1
	%t.19 = load i32, i32* %t.2
	%t.20 = load double, double* %t.3
	%t.21 = fadd double %t.20, 2.000000
	call void @fillArray(%.array* %t.16, i32 %t.18, i32 %t.19, double %t.21)

	ret void
}

define void @main() {
	call void @RtInternal_Init()

	%t.0 = alloca i32
	store i32 0, i32* %t.0

	%t.1 = alloca %.array*
	store %.array* null, %.array** %t.1

	store i32 12, i32* %t.0

	%t.2 = load i32, i32* %t.0
	%t.3 = sext i32 %t.2 to i64
	%t.4 = call %.array* @RtInternal_GcAllocArray(i64 8, i64 %t.3)
	store %.array* %t.4, %.array** %t.1

	%t.5 = load %.array*, %.array** %t.1
	%t.6 = load i32, i32* %t.0
	call void @fillArray(%.array* %t.5, i32 0, i32 %t.6, double 3.000000)

	%t.7 = load %.array*, %.array** %t.1
	%t.8 = load i32, i32* %t.0
	call void @printDoubleArray(%.array* %t.7, i32 0, i32 %t.8)

	%t.9 = load %.array*, %.array** %t.1
	%t.11 = sub i32 0, 3
	call void @fillArray(%.array* %t.9, i32 0, i32 %t.11, double 3.000000)

	%t.12 = load %.array*, %.array** %t.1
	%t.13 = load i32, i32* %t.0
	%t.14 = sub i32 %t.13, 2
	call void @printDoubleArray(%.array* %t.12, i32 0, i32 %t.14)

	ret void
}



