source_filename = "tests/codegen_cases/echo.mon"

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

	%t.6 = alloca %.array*
	store %.array* null, %.array** %t.6

	%t.7 = load %.array*, %.array** %t.6
	%t.8 = bitcast %.array* %t.7 to i8*
	call void @RtInternal_EchoArray(i8* %t.8)

	call void @RtInternal_EchoReal(double 5.600000)

	call void @RtInternal_EchoChar(i8 100)

	call void @RtInternal_EchoChar(i8 10)

	call void @RtInternal_EchoChar(i8 101)

	%t.9 = alloca %string*
	store %string* null, %string** %t.9

	%t.10 = load %string*, %string** %t.9
	call void @RtInternal_EchoString(%string* %t.10)

	store %string* bitcast ({ i64, i64, [6 x i8] }* @str.0 to %string*), %string** %t.9

	%t.11 = load %string*, %string** %t.9
	call void @RtInternal_EchoString(%string* %t.11)

	ret void
}


@str.0 = private constant { i64, i64, [6 x i8] } { i64 5, i64 18201018903628563565, [6 x i8] c"Hello\00" }

