source_filename = "tests/codegen_cases/strconcat.mon"

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

define void @main() {
	call void @RtInternal_Init()

	%t.0 = alloca %string*
	store %string* null, %string** %t.0

	%t.1 = alloca %string*
	store %string* null, %string** %t.1

	store %string* bitcast ({ i64, i64, [6 x i8] }* @str.0 to %string*), %string** %t.0

	store %string* bitcast ({ i64, i64, [9 x i8] }* @str.1 to %string*), %string** %t.1

	%t.2 = alloca %string*
	store %string* null, %string** %t.2

	%t.3 = load %string*, %string** %t.0
	%t.4 = load %string*, %string** %t.1
	%t.5 = call %string* @Strconcat(%string* %t.3, %string* %t.4)
	store %string* %t.5, %string** %t.2

	%t.6 = load %string*, %string** %t.2
	call void @RtInternal_EchoString(%string* %t.6)

	ret void
}

declare %string* @Strconcat(%string*, %string*)

@str.0 = private constant { i64, i64, [6 x i8] } { i64 5, i64 18043797133793117293, [6 x i8] c"Hello\00" }
@str.1 = private constant { i64, i64, [9 x i8] } { i64 8, i64 12654195313057627166, [9 x i8] c", world!\00" }

