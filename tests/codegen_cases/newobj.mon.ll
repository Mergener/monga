source_filename = "tests/codegen_cases/newobj.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i32)
declare void @RtInternal_Init()

%Vec3 = type { float, float, float }

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca %Vec3*

	store %Vec3* null, %Vec3** %t.0
	%t.1 = call i8* @RtInternal_GcAlloc(i32 12)
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
	call void @printFloat(float %t.14)

	%t.15 = load %Vec3*, %Vec3** %t.0
	%t.16 = getelementptr inbounds %Vec3, %Vec3* %t.15, i32 0, i32 1
	%t.17 = load float, float* %t.16
	call void @printFloat(float %t.17)

	%t.18 = load %Vec3*, %Vec3** %t.0
	%t.19 = getelementptr inbounds %Vec3, %Vec3* %t.18, i32 0, i32 2
	%t.20 = load float, float* %t.19
	call void @printFloat(float %t.20)

	ret void
}

declare void @printFloat(float)


