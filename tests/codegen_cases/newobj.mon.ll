source_filename = "tests/codegen_cases/newobj.mon"

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

%Vec3 = type { float, float, float }

define void @main() {
	call void @RtInternal_Init()

	%t.0 = alloca %Vec3*
	store %Vec3* null, %Vec3** %t.0

	%t.1 = call i8* @RtInternal_GcAlloc(i64 12)
	%t.2 = bitcast i8* %t.1 to %Vec3*
	store %Vec3* %t.2, %Vec3** %t.0

	%t.3 = load %Vec3*, %Vec3** %t.0
	%t.4 = getelementptr inbounds %Vec3, %Vec3* %t.3, i32 0, i32 0
	%t.5 = fptrunc double 5.000000 to float
	store float %t.5, float* %t.4

	%t.6 = load %Vec3*, %Vec3** %t.0
	%t.7 = getelementptr inbounds %Vec3, %Vec3* %t.6, i32 0, i32 1
	%t.8 = fptrunc double 3.000000 to float
	store float %t.8, float* %t.7

	%t.9 = load %Vec3*, %Vec3** %t.0
	%t.10 = getelementptr inbounds %Vec3, %Vec3* %t.9, i32 0, i32 2
	%t.11 = fptrunc double 2.000000 to float
	store float %t.11, float* %t.10

	%t.12 = load %Vec3*, %Vec3** %t.0
	%t.13 = getelementptr inbounds %Vec3, %Vec3* %t.12, i32 0, i32 0
	%t.14 = load float, float* %t.13
	%t.15 = fpext float %t.14 to double
	call void @RtInternal_EchoReal(double %t.15)

	%t.16 = load %Vec3*, %Vec3** %t.0
	%t.17 = getelementptr inbounds %Vec3, %Vec3* %t.16, i32 0, i32 1
	%t.18 = load float, float* %t.17
	%t.19 = fpext float %t.18 to double
	call void @RtInternal_EchoReal(double %t.19)

	%t.20 = load %Vec3*, %Vec3** %t.0
	%t.21 = getelementptr inbounds %Vec3, %Vec3* %t.20, i32 0, i32 2
	%t.22 = load float, float* %t.21
	%t.23 = fpext float %t.22 to double
	call void @RtInternal_EchoReal(double %t.23)

	ret void
}



