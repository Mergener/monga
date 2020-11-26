source_filename = "tests/codegen_cases/echo.mon"

%string = type { i32, i32 }

declare %string* @RtInternal_StrFromSZ(i8*)
declare i8* @RtInternal_GcAlloc(i64)
declare void @RtInternal_EchoArray(i8*)
declare void @RtInternal_EchoObject(i8*)
declare void @RtInternal_EchoInteger(i64)
declare void @RtInternal_EchoReal(double)
declare void @RtInternal_EchoChar(i8)
declare void @RtInternal_EchoString(%string*)
declare void @RtInternal_Init()

%Vec2 = type { float, float }

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca i32

	store i32 0, i32* %t.0
	store i32 5, i32* %t.0

	%t.1 = load i32, i32* %t.0
	%t.2 = sext i32 %t.1 to i64
	call void @RtInternal_EchoInteger(i64 %t.2)
	%t.3 = alloca %Vec2*

	store %Vec2* null, %Vec2** %t.3
	%t.4 = load %Vec2*, %Vec2** %t.3
	%t.5 = bitcast %Vec2* %t.4 to i8*
	call void @RtInternal_EchoObject(i8* %t.5)
	%t.6 = call i8* @RtInternal_GcAlloc(i64 8)
	%t.7 = bitcast i8* %t.6 to %Vec2*
	store %Vec2* %t.7, %Vec2** %t.3

	%t.8 = load %Vec2*, %Vec2** %t.3
	%t.9 = bitcast %Vec2* %t.8 to i8*
	call void @RtInternal_EchoObject(i8* %t.9)
	%t.10 = alloca %Vec2**

	store %Vec2** null, %Vec2*** %t.10
	%t.11 = load %Vec2**, %Vec2*** %t.10
	%t.12 = bitcast %Vec2** %t.11 to i8*
	call void @RtInternal_EchoArray(i8* %t.12)
	%t.13 = sext i32 5 to i64
	%t.14 = mul i64 %t.13, 8
	%t.15 = call i8* @RtInternal_GcAlloc(i64 %t.14)
	%t.16 = bitcast i8* %t.15 to %Vec2**
	store %Vec2** %t.16, %Vec2*** %t.10

	%t.17 = load %Vec2**, %Vec2*** %t.10
	%t.18 = bitcast %Vec2** %t.17 to i8*
	call void @RtInternal_EchoArray(i8* %t.18)
	call void @RtInternal_EchoReal(double 5.600000)
	call void @RtInternal_EchoChar(i8 100)
	call void @RtInternal_EchoChar(i8 10)
	call void @RtInternal_EchoChar(i8 101)
	%t.19 = alloca %string*

	store %string* null, %string** %t.19
	%t.20 = load %string*, %string** %t.19
	call void @RtInternal_EchoString(%string* %t.20)
	%t.21 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.0, i32 0, i32 0))
	store %string* %t.21, %string** %t.19

	%t.22 = load %string*, %string** %t.19
	call void @RtInternal_EchoString(%string* %t.22)
	ret void
}


@str.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1

