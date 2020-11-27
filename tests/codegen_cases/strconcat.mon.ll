source_filename = "tests/codegen_cases/strconcat.mon"

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

define void @main() {
	call void @RtInternal_Init()
	%t.0 = alloca %string*

	store %string* null, %string** %t.0
	%t.1 = alloca %string*

	store %string* null, %string** %t.1
	%t.2 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([6 x i8], [6 x i8]* @str.0, i32 0, i32 0))
	store %string* %t.2, %string** %t.0

	%t.3 = call %string* @RtInternal_StrFromSZ(i8* getelementptr inbounds ([9 x i8], [9 x i8]* @str.1, i32 0, i32 0))
	store %string* %t.3, %string** %t.1

	%t.4 = alloca %string*

	store %string* null, %string** %t.4
	%t.5 = load %string*, %string** %t.0
	%t.6 = load %string*, %string** %t.1
	%t.7 = call %string* @Strconcat(%string* %t.5, %string* %t.6)
	store %string* %t.7, %string** %t.4

	%t.8 = load %string*, %string** %t.4
	call void @printString(%string* %t.8)

	ret void
}

declare %string* @Strconcat(%string*, %string*)
declare void @printString(%string*)

@str.0 = private unnamed_addr constant [6 x i8] c"Hello\00", align 1
@str.1 = private unnamed_addr constant [9 x i8] c", world!\00", align 1

